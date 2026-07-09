# Copy constructor

> Lưu ý: mục `sources/` của bài này hiện chưa có nội dung thật (còn để template),
> nên phần note dưới đây là kiến thức chung ngoài nguồn — chưa đối chiếu với
> tài liệu cụ thể nào của bạn.

## 1. Vấn đề
Trong C, khi copy một struct (`struct Foo b = a;` hay `memcpy`), việc copy là
bit-wise — copy y nguyên từng byte. Nếu struct chỉ chứa dữ liệu trơn (int,
float, mảng cố định...) thì không sao. Nhưng nếu struct có con trỏ tới vùng
nhớ cấp phát động (`malloc`), copy bit-wise chỉ copy **địa chỉ**, không copy
**dữ liệu được trỏ tới**. Kết quả: hai struct khác nhau cùng trỏ vào một vùng
nhớ (aliasing). Free một cái → con trỏ bên kia thành dangling. Free cả hai →
double free.

C++ cần một cơ chế để khi copy một object, có thể tùy biến: copy nông (giữ
nguyên con trỏ, chấp nhận aliasing — dùng khi biết object không sở hữu vùng
nhớ) hay copy sâu (cấp phát vùng nhớ mới, sao chép nội dung).

## 2. Đối chiếu với C
Muốn "copy đúng" một struct có con trỏ trong C, bạn phải tự viết hàm, không
có cơ chế ngôn ngữ nào tự gọi giúp:

```c
typedef struct {
    char *data;
    size_t len;
} Buffer;

Buffer buffer_copy(const Buffer *src) {
    Buffer dst;
    dst.len = src->len;
    dst.data = malloc(src->len);
    memcpy(dst.data, src->data, src->len);
    return dst;
}
```

Vấn đề: hàm này chỉ có tác dụng nếu **mọi nơi trong code đều nhớ gọi nó**
thay vì gán trực tiếp (`Buffer b = a;`). Compiler C không ép được điều đó —
sai sót là do con người, không bị bắt lỗi lúc biên dịch.

## 3. Cơ chế C++
Copy constructor là constructor có signature:

```cpp
ClassName(const ClassName& other);
```

Được compiler **tự động gọi** (không cần bạn gọi tay) trong các tình huống:
1. Khởi tạo object mới từ object khác cùng kiểu: `ClassName b = a;` hoặc
   `ClassName b(a);`
2. Truyền object theo giá trị vào hàm: `void f(ClassName x)` khi gọi `f(a)`
3. Return object theo giá trị từ hàm (trước C++17 chắc chắn gọi; từ C++17
   trở đi có thể bị *copy elision* — compiler tối ưu bỏ hẳn bước copy)

Nếu bạn **không tự viết**, compiler sinh một copy constructor mặc định —
nó làm shallow copy: copy từng member một (member nào có con trỏ thì chỉ
copy địa chỉ, giống hệt vấn đề nêu ở mục 1).

Vì vậy: **class nào quản lý tài nguyên (con trỏ cấp phát động, file handle,
socket...) gần như bắt buộc phải tự viết copy constructor** (deep copy),
nếu không sẽ dính đúng lỗi double-free/dangling pointer như bên C — chỉ
khác là lỗi này "vô hình" hơn vì bạn không thấy dòng code copy tường minh
nào cả, nó bị gọi ngầm.

### Vì sao tham số phải là `const ClassName&` (không phải `ClassName`)

Hai ký hiệu `&` và `const` trong `Buffer(const Buffer& other)` giải quyết
hai vấn đề khác nhau, không phải chỉ là quy ước:

**`&` (reference) — để tránh đệ quy vô hạn.**
Trong C, muốn "truyền một struct vào hàm mà không copy" bạn truyền con trỏ
(`const Buffer *src`). C++ reference (`Buffer&`) về bản chất là một con trỏ
được compiler tự động deref — không cấp phát object mới, không copy gì cả,
chỉ là một cái tên khác trỏ tới cùng vùng nhớ của `other`.

Nếu bạn lỡ viết tham số theo giá trị:
```cpp
Buffer(Buffer other); // SAI
```
thì để gọi hàm này, compiler phải tạo ra tham số `other` bằng cách... gọi
copy constructor của `Buffer` — chính là hàm đang được định nghĩa. Gọi copy
constructor để tạo tham số cho copy constructor → đệ quy vô hạn, chương
trình không bao giờ biên dịch xong (thực ra là lỗi biên dịch vì compiler
phát hiện được đệ quy này). Đây là lý do `&` ở đây **bắt buộc**, không phải
tối ưu.

**`const` — cam kết không sửa object nguồn.**
`other` là bản gốc (`a` trong `Buffer b = a;`), không phải bản đang được
tạo (`b`). Copy constructor chỉ có nhiệm vụ đọc dữ liệu từ `other` để tạo
`b`, không có lý do gì để sửa `other`. `const` là lời cam kết đó, được
compiler kiểm tra tĩnh: nếu code trong thân hàm lỡ gán vào `other.data`,
compiler báo lỗi ngay lúc biên dịch — giống hệt tinh thần `const char *`
trong C khi bạn muốn báo "hàm này chỉ đọc, không ghi" qua chữ ký hàm thay
vì chỉ ghi trong comment.
`const` còn cho phép truyền được **rvalue tạm** (`Buffer b = Buffer("x");`)
làm `other`, vì reference không-const không thể bind vào giá trị tạm.

### Workflow khi `Buffer b = a;` chạy

1. Compiler thấy đây là **khởi tạo** (không phải gán, vì `b` chưa tồn tại
   trước đó) → chọn overload constructor phù hợp: có 1 object cùng kiểu
   bên phải → copy constructor.
2. Cấp phát vùng nhớ cho `b` trên stack (chưa chạy constructor nào, `b`
   hiện là vùng nhớ thô).
3. Gọi `Buffer::Buffer(const Buffer& other)` với `other` là reference
   (không copy) tới `a` — bên trong hàm, `this` trỏ tới `b`.
4. Thân hàm chạy: đọc `other.len`, `other.data` (chỉ đọc, nhờ `const`),
   `new[]` cấp phát vùng nhớ **mới** cho `b.data`, `memcpy` nội dung từ
   `other.data` sang.
5. Copy constructor kết thúc → `b` là object hoàn chỉnh, độc lập hoàn
   toàn với `a` (khác vùng nhớ `data`).
6. Khi `a` và `b` ra khỏi scope, destructor được gọi cho **mỗi object**,
   mỗi lần `delete[]` một vùng nhớ khác nhau → không double-free.

So sánh với workflow tương đương bên C ở mục 2: bước 3-4 chính là những gì
`buffer_copy()` làm tay — khác biệt là ở C bạn phải nhớ gọi nó, còn ở đây
bước 1 (compiler tự chọn và tự gọi) là phần C++ thêm vào.

## 4. Thực hành

```cpp
#include <cstring>
#include <iostream>

class Buffer {
public:
    // Constructor thường
    Buffer(const char* text) {
        len = std::strlen(text) + 1;
        data = new char[len];
        std::memcpy(data, text, len);
    }

    // Copy constructor — deep copy
    Buffer(const Buffer& other) {
        len = other.len;
        data = new char[len];
        std::memcpy(data, other.data, len);
        std::cout << "Copy constructor called\n";
    }

    ~Buffer() {
        delete[] data;
    }

    const char* get() const { return data; }

private:
    char* data;
    size_t len;
};

int main() {
    Buffer a("hello");
    Buffer b = a;   // gọi copy constructor -> deep copy, b.data khác a.data

    std::cout << a.get() << " " << b.get() << "\n";
    return 0;
    // ~Buffer gọi 2 lần, mỗi lần delete[] vùng nhớ riêng -> an toàn
}
```

Tương đương bên C (nếu không có copy constructor, phải tự nhớ gọi
`buffer_copy` ở mục 2) — C++ đảm bảo `Buffer b = a;` luôn "làm đúng" vì
compiler tự chèn lời gọi.

### Đối chứng: shallow copy (xóa copy constructor tự viết) → crash

```cpp
#include <cstring>
#include <iostream>

class BufferShallow {
public:
    BufferShallow(const char* text) {
        len = std::strlen(text) + 1;
        data = new char[len];
        std::memcpy(data, text, len);
    }

    // Không viết copy constructor -> compiler tự sinh bản shallow:
    // chỉ copy con trỏ data, KHÔNG new[] vùng nhớ mới.

    ~BufferShallow() {
        delete[] data;
    }

    const char* get() const { return data; }

private:
    char* data;
    size_t len;
};

int main() {
    BufferShallow a("hello");
    BufferShallow b = a;   // shallow copy: b.data == a.data (cùng địa chỉ)

    std::cout << a.get() << " " << b.get() << "\n";
    return 0;
    // ~BufferShallow gọi 2 lần cho CÙNG một địa chỉ -> double free (UB, thường crash)
}
```

So với bản deep copy ở trên: khác biệt duy nhất là bỏ đi khối
`Buffer(const Buffer& other) {...}`. Compiler không báo lỗi biên dịch gì cả
— chương trình biên dịch sạch, chạy có thể còn in ra kết quả đúng trong
`main()`, và chỉ crash (hoặc heap corruption âm thầm) lúc chương trình kết
thúc, ở chỗ hai destructor tranh nhau free cùng một vùng nhớ. Đây là lý do
shallow-copy bug thường bị bỏ sót khi test nhanh.

## 5. Cạm bẫy / lưu ý
- **Rule of Three**: nếu class cần viết copy constructor (vì quản lý tài
  nguyên), gần như chắc chắn cũng cần viết destructor và copy assignment
  operator (`operator=`) — thiếu 1 trong 3 dễ sinh bug.
- Copy constructor mặc định (compiler-generated) là shallow copy — trông
  giống code "chạy được" lúc test nhanh, nhưng crash khi object bị copy rồi
  hủy (double free) hoặc dùng sau khi bản gốc đã hủy (dangling).
- Chi phí ẩn: mỗi lần object được truyền theo giá trị / return theo giá trị
  (trước C++17 hoặc khi elision không áp dụng), copy constructor chạy —
  với object nắm giữ vùng nhớ lớn, đây là chi phí cấp phát + memcpy ẩn mà
  code C tương đương (nếu chỉ truyền con trỏ) không hề có. Đây là lý do
  C++ khuyến khích truyền `const ClassName&` thay vì truyền theo giá trị
  khi không cần copy.
- Tham số của copy constructor luôn là `const ClassName&`, không phải
  `ClassName` — vì nếu truyền theo giá trị, gọi copy constructor sẽ đệ quy
  vô hạn (để tạo tham số lại cần gọi copy constructor).

## 6. Câu hỏi mở
- [ ] Copy assignment operator (`operator=`) khác gì với copy constructor,
      và khi nào mỗi cái được gọi?
- [ ] Move constructor (C++11) giải quyết thêm vấn đề gì so với copy
      constructor sâu ở trên?
- [ ] Copy elision (C++17 guaranteed) ảnh hưởng thế nào tới việc copy
      constructor có được gọi hay không khi return object theo giá trị?
