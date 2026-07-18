# Function Overloading

## 1. Vấn đề
Trong C, mỗi hàm phải có tên riêng biệt vì C chỉ dùng **tên hàm** để phân biệt (linker match theo tên, không quan tâm tham số). Muốn "làm cùng một việc" với nhiều kiểu dữ liệu khác nhau, bạn phải đặt tên khác nhau:

```c
int add_int(int a, int b);
float add_float(float a, float b);
double add_double(double a, double b);
```

Cồng kềnh, và người gọi phải nhớ đúng tên hàm ứng với đúng kiểu — dễ gọi nhầm, dễ quên cập nhật khi thêm kiểu mới.

## 2. Đối chiếu với C
Cách người viết C hay làm để "giả lập" polymorphism theo tham số:
- **_Generic (C11)**: macro dispatch theo kiểu tại compile-time, cú pháp xấu, giới hạn số kiểu liệt kê sẵn.
- **void\* + tham số kiểu (kiểu tag)**: dispatch tại runtime, mất type-safety, dễ crash nếu truyền sai tag.
- **Macro đơn giản**: `#define ADD(a,b) ((a)+(b))` — hoạt động nhờ C không type-check macro, nhưng không debug được (macro expand rồi mới thấy lỗi), không có scope, không tương thích tool phân tích.

## 3. Cơ chế C++
C++ cho phép nhiều hàm **cùng tên**, miễn **signature khác nhau** (khác về số lượng, kiểu, hoặc thứ tự tham số — **không tính kiểu trả về**).

```cpp
int add(int a, int b);
double add(double a, double b);
int add(int a, int b, int c);
```

Trình biên dịch làm gì ngầm:
- **Name mangling**: mỗi overload được đổi tên nội bộ (mangled name) mã hoá cả tham số, ví dụ `add(int,int)` → `_Z3addii`. Đây là lý do C++ không link trực tiếp được với object file C (`extern "C"` để tắt mangling khi cần interop).
- **Overload resolution**: tại mỗi lời gọi, compiler tìm hàm khớp nhất dựa trên tham số truyền vào, theo thứ tự ưu tiên: match chính xác > promotion (int→long) > standard conversion (int→double) > user-defined conversion. Nếu có nhiều hàm khớp ngang nhau → lỗi **ambiguous call** tại compile-time (không phải runtime).
- Tất cả xảy ra ở **compile-time**, không có chi phí runtime (khác với virtual function dùng vtable).

## 4. Thực hành

```cpp
#include <cstdio>

int add(int a, int b) {
    printf("int version\n");
    return a + b;
}

double add(double a, double b) {
    printf("double version\n");
    return a + b;
}

int add(int a, int b, int c) {
    printf("3-arg version\n");
    return a + b + c;
}

int main() {
    add(1, 2);        // int version
    add(1.5, 2.5);     // double version
    add(1, 2, 3);      // 3-arg version
}
```

So với C, bạn không cần `add_int`, `add_double` nữa — cùng một tên `add`, compiler tự chọn đúng overload dựa trên kiểu tham số lúc gọi.

## 5. Cạm bẫy / lưu ý
- **Chi phí ẩn**: gần như bằng 0 lúc runtime — resolution xảy ra tại compile-time. Cái phải trả là *binary size* (mỗi overload là một hàm riêng, không share code trừ khi bạn tự refactor) và *thời gian compile* (đặc biệt khi overload kết hợp với template → có thể bùng nổ số instantiation).
- **Ambiguity dễ gặp khi mang tư duy C sang**:
  ```cpp
  void f(int a);
  void f(long a);
  f(3.0f); // float -> có thể convert lên cả int lẫn double rules, dễ ambiguous nếu thêm overload double
  ```
- **Overload theo kiểu trả về KHÔNG hợp lệ** — khác với một số ngôn ngữ khác:
  ```cpp
  int get();
  double get(); // lỗi compile: không phân biệt được nếu không dùng tham số
  ```
- **const/reference qualifiers cũng tính vào signature** — dễ bug tinh vi khi overload theo `T&` vs `const T&` vs `T&&` (rvalue reference, dùng cho move semantics — chủ đề riêng).
- Overload resolution có thể chọn nhầm ý bạn khi có **implicit conversion** (đặc biệt constructor 1 tham số không đánh dấu `explicit`) — dễ tạo bug im lặng, không giống C nơi mọi conversion đều tường minh hoặc do bạn tự viết.

## 6. Câu hỏi mở
- [ ] Overload resolution xử lý thế nào khi kết hợp với default argument?
- [ ] Khi nào nên dùng overload thay vì template (generic thật sự)?
