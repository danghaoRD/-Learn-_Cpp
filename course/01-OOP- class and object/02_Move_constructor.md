# Move constructor

> Lưu ý: mục `sources/` của bài này hiện chưa có nội dung thật (còn để template),
> và `questions.md` cũng chưa có câu hỏi cụ thể, nên phần note dưới đây là kiến
> thức chung ngoài nguồn — chưa đối chiếu với tài liệu cụ thể nào của bạn. Bài
> này nối tiếp trực tiếp [[01_Copy_Constructor.md]] — nên đọc bài đó trước.

## 1. Vấn đề

Copy constructor (bài 01) giải quyết đúng bài toán "đừng để hai object cùng
trỏ vào một vùng nhớ" bằng deep copy: cấp phát vùng nhớ **mới**, `memcpy`
từng byte sang. Nhưng deep copy trả giá bằng cấp phát + sao chép, kể cả khi
object nguồn **sắp bị hủy ngay sau đó** — lúc đó công sao chép là lãng phí
thuần túy, vì thay vì "sao chép rồi hủy nguồn", ta có thể "cướp luôn tài
nguyên của nguồn rồi khỏi cần hủy tay".

Tình huống này xảy ra thường xuyên hơn tưởng tượng:

```cpp
Buffer make_buffer() {
    Buffer tmp("hello");
    return tmp;              // tmp sắp bị hủy khi hàm return
}

Buffer b = make_buffer();    // deep copy toàn bộ nội dung của tmp vào b,
                              // rồi tmp bị destructor free ngay sau đó
```

```cpp
std::vector<Buffer> v;
v.push_back(Buffer("x"));    // vector resize -> copy TOÀN BỘ phần tử cũ
                              // sang vùng nhớ mới, rồi hủy vùng nhớ cũ
```

Cả hai trường hợp: object nguồn (`tmp`, các phần tử cũ trong buffer cũ của
`vector`) là **rvalue** — giá trị tạm, không ai còn giữ tham chiếu tới nó sau
biểu thức hiện tại. Copy constructor không phân biệt được "nguồn còn sống lâu
dài, phải copy an toàn" với "nguồn sắp chết, copy là phí công" — nó luôn
copy sâu như nhau. C++11 thêm move constructor để xử lý riêng trường hợp thứ
hai.

## 2. Đối chiếu với C

Trong C, "tối ưu hóa" tương tự luôn phải làm **thủ công và có ý thức**:
tự tay chuyển quyền sở hữu con trỏ thay vì `malloc` + `memcpy` lại:

```c
Buffer buffer_move(Buffer *src) {
    Buffer dst;
    dst.data = src->data;   // cướp con trỏ, không malloc mới
    dst.len  = src->len;
    src->data = NULL;       // nguồn không còn sở hữu gì -> free(NULL) an toàn
    src->len  = 0;
    return dst;
}
```

Vấn đề y hệt copy constructor ở bài 01: hàm này chỉ có tác dụng nếu **lập
trình viên nhớ gọi nó** đúng chỗ, đúng lúc nguồn thực sự sắp bị bỏ. Gọi nhầm
`buffer_move` khi nguồn còn cần dùng tiếp → nguồn thành con trỏ NULL bất ngờ.
Không có gì trong ngôn ngữ C phân biệt được "con trỏ này còn sống" với "con
trỏ này sắp chết" để chọn hàm phù hợp — đó hoàn toàn là quy ước giữa người
với người, compiler không kiểm tra được.

## 3. Cơ chế C++

### rvalue reference (`&&`) — cách compiler nhận diện "nguồn sắp chết"

C++11 thêm một loại tham chiếu mới, **rvalue reference**, ký hiệu `&&`, chỉ
bind được vào rvalue (giá trị tạm, không có tên, không ai giữ được sau biểu
thức hiện tại) — không bind được vào biến có tên (lvalue):

```cpp
Buffer&  lref = a;              // OK: & bind vào lvalue
Buffer&& rref = Buffer("x");    // OK: && bind vào rvalue (object tạm)
Buffer&& rref2 = a;             // LỖI biên dịch: a là lvalue, không phải rvalue
```

Move constructor là constructor nhận tham số theo kiểu này:

```cpp
ClassName(ClassName&& other) noexcept;
```

Compiler **tự chọn** overload này thay vì copy constructor bất cứ khi nào vế
phải là rvalue — tức là compiler tĩnh biết trước "nguồn này sắp chết", không
cần lập trình viên nhớ gọi hàm nào như bên C. Các tình huống rvalue tự nhiên:
giá trị tạm (`Buffer("x")`), return theo giá trị (`return tmp;` khi không bị
copy elision), hoặc kết quả của `std::move()`.

### `std::move()` — ép một lvalue thành rvalue

`a` trong `Buffer b = a;` là lvalue (có tên, còn sống) → compiler chọn copy
constructor, kể cả khi lập trình viên biết rõ sẽ không dùng `a` nữa. Muốn ép
compiler chọn move constructor với một lvalue, dùng `std::move(a)` —
**không di chuyển gì cả**, chỉ là một cast: đổi kiểu của biểu thức từ
`Buffer&` thành `Buffer&&` để overload resolution chọn đúng nhánh move.
Bản thân việc "di chuyển" (cướp con trỏ) là do thân hàm move constructor làm,
không phải `std::move()`.

```cpp
Buffer b = std::move(a);   // ép a thành rvalue -> gọi move constructor
                            // sau dòng này, a coi như "rỗng" (moved-from),
                            // KHÔNG được dùng lại nội dung của a nữa
```

### Thân hàm move constructor — bản C++ hóa của `buffer_move`

```cpp
Buffer(Buffer&& other) noexcept
    : data(other.data), len(other.len) {
    other.data = nullptr;   // y hệt src->data = NULL bên C
    other.len  = 0;
}
```

So với move constructor, thân hàm này chính là `buffer_move()` ở mục 2:
cướp con trỏ (không `new[]`), rồi đặt nguồn về trạng thái rỗng để destructor
của nguồn không free nhầm vùng nhớ đã bị cướp. Khác biệt với C: compiler
**tự động chọn đúng lúc** nhờ rvalue reference, thay vì con người phải nhớ.

`noexcept` gần như bắt buộc trên move constructor/move assignment: các
container chuẩn (`std::vector`...) khi resize sẽ *ưu tiên gọi move* thay vì
copy, nhưng chỉ khi move được đảm bảo không ném exception — nếu move constructor
có thể ném, container phải quay lại dùng copy constructor (an toàn hơn khi
resize giữa chừng bị lỗi) để tránh mất dữ liệu nếu exception xảy ra nửa
chừng khi đã hủy một phần phần tử cũ.

### Move assignment operator — cùng ý tưởng, cho phép gán (`=`)

Song song với move constructor (khởi tạo object mới), còn có move assignment
cho trường hợp gán vào object **đã tồn tại**:

```cpp
ClassName& operator=(ClassName&& other) noexcept;
```

Khác move constructor ở chỗ `*this` đã có tài nguyên cũ từ trước — phải:
1. Kiểm tra self-move (`s = std::move(s);`) — nếu không, bước 2 có thể xóa
   nhầm tài nguyên rồi mới đọc lại từ `other` (đã bị dùng chung địa chỉ với
   `*this`) → đọc dữ liệu đã bị free.
2. Giải phóng tài nguyên cũ của `*this` (giống destructor).
3. Cướp tài nguyên của `other`, đặt `other` về rỗng.

### Rule of Five

Bài 01 nêu Rule of Three (destructor + copy constructor + copy assignment).
C++11 mở rộng thành **Rule of Five**: nếu một class cần tự viết 1 trong 5 hàm
sau, gần như chắc chắn cần cả 5 (trừ khi cố tình cấm move/copy):
destructor, copy constructor, copy assignment, move constructor, move
assignment. Lý do: nếu bạn tự viết destructor/copy (báo hiệu class quản lý
tài nguyên thủ công), compiler sẽ **không tự sinh** move constructor/move
assignment nữa — mọi thao tác "tưởng là move" (`return tmp;`, `push_back`...)
sẽ âm thầm rơi về copy constructor, mất hẳn lợi ích hiệu năng của move mà
không có lỗi biên dịch nào cảnh báo.

## 4. Thực hành

```cpp
#include <cstring>
#include <iostream>
#include <utility>   // std::move

class Buffer {
public:
    Buffer(const char* text) {
        len = std::strlen(text) + 1;
        data = new char[len];
        std::memcpy(data, text, len);
    }

    // Copy constructor — deep copy (bài 01)
    Buffer(const Buffer& other) : len(other.len) {
        data = new char[len];
        std::memcpy(data, other.data, len);
        std::cout << "copy ctor\n";
    }

    // Move constructor — cướp con trỏ, KHÔNG cấp phát mới
    Buffer(Buffer&& other) noexcept : data(other.data), len(other.len) {
        other.data = nullptr;
        other.len  = 0;
        std::cout << "move ctor\n";
    }

    ~Buffer() { delete[] data; }

    const char* get() const { return data ? data : "(moved-from)"; }

private:
    char* data;
    size_t len;
};

int main() {
    Buffer a("hello");
    Buffer b = a;               // lvalue -> copy ctor (deep copy)
    Buffer c = std::move(a);    // std::move ép a thành rvalue -> move ctor

    std::cout << b.get() << " " << c.get() << "\n";
    // a.get() ở đây không còn ý nghĩa (moved-from), không nên dùng
    return 0;
}
```

So với `buffer_move()` bên C ở mục 2: thân hàm giống hệt nhau (gán con trỏ,
đặt NULL), khác biệt duy nhất là **ai quyết định gọi nó** — bên C là con
người tại chỗ gọi hàm, bên C++ là compiler tại chỗ overload resolution dựa
trên rvalue/lvalue.

## 5. Cạm bẫy / lưu ý

- **Object sau khi bị move (`moved-from`) vẫn phải ở trạng thái hợp lệ để
  hủy an toàn** (destructor gọi được, không free vùng nhớ đã mất), nhưng
  **nội dung của nó không còn xác định** — chuẩn C++ chỉ đảm bảo "valid but
  unspecified state". Đừng đọc giá trị của object đã bị move trừ khi tự class
  đó tài liệu hóa rõ (như `data ? data : "(moved-from)"` ở ví dụ trên).
- **`std::move()` không di chuyển gì cả** — nó chỉ là `static_cast<T&&>`,
  một phép ép kiểu để chọn overload. Nếu class không có move constructor,
  `std::move(a)` vẫn biên dịch được nhưng âm thầm rơi về copy constructor
  (vì `const T&` cũng bind được vào `T&&`) — không lỗi, không cảnh báo, chỉ
  mất lợi ích hiệu năng.
- **Quên đặt `other.data = nullptr`** trong move constructor là lỗi kinh
  điển: cả `*this` và `other` cùng trỏ một vùng nhớ, khi cả hai destructor
  chạy → double-free, y hệt lỗi shallow-copy ở bài 01 nhưng khó phát hiện
  hơn vì code "trông như" đã xử lý move đúng.
- **Move constructor nên `noexcept`** — nếu không, `std::vector` và các
  container chuẩn khác có thể từ chối dùng move khi resize (quay về copy)
  để đảm bảo an toàn ngoại lệ (strong exception guarantee), làm mất hẳn lý
  do bạn viết move constructor.
- Chi phí ẩn theo chiều ngược lại: nếu class chỉ chứa dữ liệu nhỏ, phẳng
  (vài `int`, không con trỏ), move và copy làm cùng một việc (copy bit-wise)
  — viết move constructor riêng không lợi gì, compiler-generated move đã đủ.
  Move chỉ thực sự lợi khi class sở hữu tài nguyên cấp phát động (con trỏ,
  handle) mà việc "cướp" rẻ hơn hẳn "sao chép".

## 6. Câu hỏi mở

- [ ] Copy elision (C++17 guaranteed) có làm move constructor không được gọi
      khi `return tmp;` không, và khác gì với move constructor được gọi
      tường minh?
- [ ] `std::move` vs `std::forward` — khi nào dùng cái nào (liên quan tới
      template + universal reference, ngoài phạm vi bài này)?
- [ ] Vì sao compiler tự sinh move constructor/move assignment bị **vô hiệu
      hóa** ngay khi class tự viết destructor hoặc copy constructor — cơ chế
      chính xác đứng sau quy tắc "Rule of Five" này là gì?
