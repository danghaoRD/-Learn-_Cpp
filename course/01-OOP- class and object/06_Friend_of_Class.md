# Friend of Class

## 1. Vấn đề
> C++ giải quyết bài toán gì mà C thuần không làm được / làm được nhưng cồng kềnh, dễ lỗi?

`private`/`protected` tồn tại để đóng gói (encapsulation) — chỉ member function của
class mới được đụng vào dữ liệu nội bộ. Nhưng có những trường hợp một hàm hoặc class
*bên ngoài* cần truy cập trực tiếp phần private đó, mà:

- Không muốn làm nó thành `public` (phá vỡ encapsulation cho toàn bộ thế giới, không
  chỉ riêng cái hàm/class cần dùng).
- Không muốn viết hàng loạt getter/setter chỉ để phục vụ một trường hợp đặc biệt
  (ví dụ overload `operator<<` cho `std::cout`, hoặc hai class hợp tác chặt như
  container và iterator).

`friend` là cách C++ cấp ngoại lệ tường minh: "class/hàm này được phép vào, còn lại
vẫn đóng kín."

## 2. Đối chiếu với C
> Trên C tôi phải làm tương đương thế nào?

C không có access control thật sự ở compile-time. Khi có `struct` với field "nội bộ",
cách giả lập encapsulation là **opaque pointer**: header chỉ forward-declare
(`typedef struct Foo Foo;`), còn định nghĩa struct đầy đủ nằm trong file `.c`.

Bất kỳ hàm nào thấy được định nghĩa struct đầy đủ (cùng file, hoặc include đúng header
nội bộ) đều đụng thẳng vào field — không có cơ chế phân quyền ở mức ngôn ngữ, chỉ dựa
vào *quy ước tổ chức file*.

Nói cách khác: trong C, "friend" là **mặc định** — mọi hàm nhìn thấy struct đều là
friend. C++ đảo ngược: mặc định *không ai* là friend ngoài chính class, và phải khai
báo tường minh ai được đặc quyền. `friend` không phải feature mới, mà là cơ chế bù lại
access-control mà C++ vừa thêm so với C.

## 3. Cơ chế C++
> Cú pháp, quy tắc hoạt động, những gì trình biên dịch làm ngầm?

- Khai báo `friend` nằm **trong** class muốn cấp quyền, không phải trong hàm/class
  được cấp quyền.
- Ba dạng:
  1. **Friend function** (hàm tự do, không phải member):
     ```cpp
     class Box {
         friend void printVolume(const Box& b);
     private:
         double w, h, d;
     };
     ```
  2. **Friend member function của class khác**:
     ```cpp
     class Other { void access(Box& b); };
     class Box {
         friend void Other::access(Box& b);
         // ...
     };
     ```
  3. **Friend class** (toàn bộ class kia được quyền vào private/protected):
     ```cpp
     class Box {
         friend class BoxBuilder;
         // ...
     };
     ```
- `friend` **không đối xứng**: A cấp friend cho B không có nghĩa B cấp lại cho A.
- `friend` **không kế thừa**: `Box` friend với `Other`, có `Derived : public Box` thì
  `Other` không tự động là friend của `Derived`.
- `friend` **không bắc cầu**: A friend B, B friend C ⇒ A không tự động friend C.
- Compiler xử lý thuần túy ở compile-time — friend chỉ là danh sách trắng trong
  access-checking, không sinh thêm code, không tốn runtime cost (khác `virtual`, RTTI).

## 4. Thực hành
```cpp
#include <iostream>

class Box {
private:
    double width, height;

public:
    Box(double w, double h) : width(w), height(h) {}

    // khai báo quyền: hàm tự do này được đọc private
    friend std::ostream& operator<<(std::ostream& os, const Box& b);
};

// định nghĩa hàm tự do, không phải member của Box
std::ostream& operator<<(std::ostream& os, const Box& b) {
    os << "Box(" << b.width << " x " << b.height << ")"; // truy cập private trực tiếp
    return os;
}

int main() {
    Box b(3.0, 4.0);
    std::cout << b << std::endl; // Box(3 x 4)
}
```

So với C: viết `void box_print(const struct Box* b)` nhìn thấy field trực tiếp vì
struct định nghĩa full trong cùng file — không cần khai báo gì đặc biệt, vì C không
chặn từ đầu.

Ví dụ friend class (hai class hợp tác chặt, kiểu container + builder):

```cpp
class Engine;

class Car {
private:
    Engine* engine = nullptr;
    friend class CarFactory; // CarFactory được vào thẳng engine
public:
    void drive() { /* ... */ }
};

class CarFactory {
public:
    void installEngine(Car& c, Engine* e) {
        c.engine = e; // truy cập private của Car
    }
};
```

## 5. Cạm bẫy / lưu ý
- **Phá vỡ encapsulation nếu lạm dụng**: `friend` là "cửa hậu" hợp pháp — dùng quá
  nhiều thì class không còn thực sự đóng gói, lợi ích OOP bị xói mòn. Chỉ dùng cho
  cặp quan hệ thực sự chặt (operator overload, iterator-container, factory-product).
- **Không có chi phí ẩn runtime** như virtual/exception — điểm khác với đa số feature
  C++ khác. Nhưng có **chi phí thiết kế**: tăng coupling giữa hai class, sửa `Box` có
  thể ảnh hưởng `CarFactory` mà không thấy rõ nếu chỉ đọc code của `CarFactory`.
- **Dễ nhầm với inheritance**: friend không phải kiểu "class con truy cập private của
  cha" (đó là chuyện `protected`). Friend là quan hệ ngang hàng, tường minh, không tự
  lan qua kế thừa.
- **Friend declaration không phải forward declaration đầy đủ**: `friend class
  BoxBuilder;` trong `Box` vừa khai báo quyền, vừa forward-declare `BoxBuilder` nếu
  chưa có — nhưng vẫn cần định nghĩa `BoxBuilder` đầy đủ ở đâu đó để dùng nó.

## 6. Câu hỏi mở
- [ ] Chưa có source cụ thể (video/bài viết) cho bài này — nếu có, dán vào
      `sources/` để đối chiếu và bổ sung đúng phần đã học.
