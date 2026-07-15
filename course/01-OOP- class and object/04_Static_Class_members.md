# Static Class Members

## 1. Vấn đề

Khi cần một dữ liệu hoặc hành vi **dùng chung cho mọi instance** của một kiểu (bộ đếm số object đã tạo, config chung...), C không có cách gắn nó vào struct — buộc phải khai báo biến/hàm tách rời ở phạm vi file, không có ràng buộc ngữ nghĩa hay kiểm soát truy cập nào với struct liên quan, dễ đụng độ tên và dễ bị code không liên quan sửa nhầm.

## 2. Đối chiếu với C

Static member **không phải khái niệm bộ nhớ mới** — nó chính là biến global/static file-scope mà C vẫn dùng. Cái C++ thêm vào là **đóng gói + namespace**: biến/hàm đó được gắn vào class, truy cập qua `ClassName::member`, áp dụng được `private/public`.

```c
// C: tay làm bằng biến global + convention đặt tên
static int Device_count = 0;

Device device_create(void) {
    Device d;
    d.id = Device_count++;
    return d;
}
```

```cpp
// C++: gắn vào class, có kiểm soát truy cập
class Device {
    static int count;   // khai báo — chưa cấp bộ nhớ
public:
    Device() { count++; }
    static int getCount() { return count; }
};
int Device::count = 0;   // định nghĩa — cấp bộ nhớ thật, bắt buộc (trước C++17)
```

## 3. Cơ chế C++

### Static data member
- Chỉ tồn tại **một bản duy nhất** cho cả class, không nằm trong từng instance (không cộng vào `sizeof`), nằm ở static storage duration giống hệt biến static của C.
- **Khai báo trong class ≠ định nghĩa.** Trong class chỉ là lời tuyên bố; phải định nghĩa (cấp bộ nhớ) ở đúng 1 file `.cpp`, nếu không → lỗi linker `undefined reference`. Từ C++17 có thể dùng `inline static int count = 0;` ngay trong class để khỏi định nghĩa riêng.

### Static member function
- Không có `this` → **chỉ được truy cập static member khác**, không truy cập được non-static member/method (vì không biết member đó thuộc object nào).
- **Không bắt buộc phải dùng static variable.** `static` ở method chỉ nghĩa là "không gắn với object cụ thể" — nó có thể là hàm thuần túy không đụng gì tới static data:

```cpp
class MathUtil {
public:
    static int square(int x) { return x * x; }   // hợp lệ, không cần static member nào
};
```

| | static method | non-static method |
|---|---|---|
| gọi static member | ✅ | ✅ |
| gọi non-static member | ❌ (không có `this`) | ✅ |

### Static object — thời điểm gọi constructor

Static **variable** (kiểu nguyên thủy) của C chỉ là cấp bộ nhớ + giá trị ghi sẵn, không có code nào chạy. Static **object** của C++ (có constructor) thì khác — trình biên dịch phải chèn code thực thi constructor tại thời điểm xác định rõ ràng theo chuẩn, **không phải compiler tùy ý chọn**:

| Loại | Constructor chạy khi nào | Compiler có tùy chọn không |
|---|---|---|
| Global/static, initializer là hằng số (constant expression) | Không cần chạy code — giá trị ghi sẵn vào binary ("static initialization") | Không |
| Global/static, initializer cần constructor thật | Trước `main()` ("dynamic initialization") | Chỉ tùy chọn **thứ tự giữa các file** khác nhau |
| Local static (khai báo trong hàm) | Lazy — lần đầu luồng thực thi đi qua dòng khai báo đó | Không |

## 4. Thực hành

```cpp
#include <iostream>

class Device {
    static int count;        // khai báo trong class
    int id;
public:
    Device() : id(count++) {}
    int getId() const { return id; }
    static int getCount() { return count; }   // static method, không có this
};

int Device::count = 0;       // định nghĩa ngoài class — bắt buộc

int main() {
    Device a, b, c;
    std::cout << "Total devices: " << Device::getCount() << "\n"; // 3
    std::cout << "c.id = " << c.getId() << "\n";                  // 2
}
```

Local static object — constructor chạy lazy, chỉ 1 lần:

```cpp
class Logger {
public:
    Logger()  { std::cout << "Logger start\n"; }
    ~Logger() { std::cout << "Logger end\n"; }
};

void foo() {
    static Logger log;   // constructor chỉ chạy ở lần gọi foo() đầu tiên
}
```

## 5. Cạm bẫy / lưu ý

- **Lỗi linker kinh điển**: khai báo static member trong class mà quên định nghĩa ngoài class → `undefined reference to Device::count`. Trước C++17 đây là nguồn lỗi phổ biến nhất khi mới học static member.
- **Static Initialization Order Fiasco**: nếu static/global object ở file A dùng static/global object ở file B trong constructor, thứ tự constructor nào chạy trước giữa 2 file là *unspecified* — C không gặp vấn đề này vì static init của C không chạy code.
- **Nhầm lẫn 2 nghĩa của `static`**: `static` local variable trong C (một bản, giữ giá trị qua các lần gọi hàm) khác với static class member (một bản, dùng chung giữa các object) — cùng từ khóa, khác ngữ cảnh.
- **Chi phí ẩn của local static object (từ C++11)**: trình biên dịch chèn thêm *guard variable* để kiểm tra "đã init chưa" mỗi lần vào hàm, đảm bảo thread-safe — tốn một nhánh rẽ runtime nhỏ dù chỉ init đúng 1 lần. Static data member kiểu nguyên thủy (`int`, `double`...) không có chi phí này, chỉ là global variable thuần túy.
- Truy cập static member qua object (`d.count`) hợp lệ nhưng dễ gây hiểu lầm "mỗi object có bản riêng" — nên luôn dùng `ClassName::member` cho rõ ràng.

## 6. Câu hỏi mở
- [ ] Static member + đa luồng (thread safety khi nhiều thread cùng tăng static counter) cần xử lý gì thêm (atomic, mutex)?
- [ ] `inline static` (C++17) khác gì cách khai báo+định nghĩa truyền thống ngoài việc gộp thành 1 dòng — có ảnh hưởng gì đến static init order fiasco không?
