# Function Overloading trong C++

## 1. Vấn đề
> C++ giải quyết bài toán gì mà C thuần không làm được / làm được nhưng cồng kềnh, dễ lỗi?

Trong thực tế, nhiều hàm thực hiện **cùng một công việc** nhưng chỉ khác kiểu dữ liệu hoặc số lượng tham số.

Ví dụ hàm tìm giá trị lớn nhất:

- So sánh hai số nguyên
- So sánh hai số thực
- So sánh ba số nguyên

Trong C, vì mỗi hàm phải có tên duy nhất nên thường phải đặt nhiều tên khác nhau:

- `max_int()`
- `max_float()`
- `max_double()`

Người lập trình phải nhớ nhiều tên hàm mặc dù bản chất chúng đều thực hiện cùng một chức năng.

C++ giải quyết vấn đề này bằng **Function Overloading**, cho phép nhiều hàm cùng tên nhưng có danh sách tham số khác nhau.

---

## 2. Đối chiếu với C
> Trên C tôi phải làm tương đương thế nào?

Trong C:

```c
#include <stdio.h>

int max_int(int a, int b)
{
    return (a > b) ? a : b;
}

float max_float(float a, float b)
{
    return (a > b) ? a : b;
}

int max3_int(int a, int b, int c)
{
    int m = (a > b) ? a : b;
    return (m > c) ? m : c;
}

int main()
{
    printf("%d\n", max_int(3, 5));
    printf("%.1f\n", max_float(3.5f, 2.1f));
    printf("%d\n", max3_int(1, 4, 2));

    return 0;
}
```

Mỗi phiên bản phải dùng một tên hàm khác nhau.

Trong C++, tất cả có thể dùng chung một tên:

```cpp
max(...);
```

Compiler sẽ tự chọn phiên bản phù hợp.

---

## 3. Cơ chế C++
> Cú pháp, quy tắc hoạt động, những gì trình biên dịch làm ngầm?

### Khái niệm

Một chương trình có thể có nhiều hàm cùng tên nếu **khác danh sách tham số**.

Ví dụ:

```cpp
int max(int a, int b);

double max(double a, double b);

int max(int a, int b, int c);
```

---

### Function Signature

Compiler không phân biệt các hàm bằng kiểu trả về mà bằng **Function Signature**.

Signature bao gồm:

- Tên hàm
- Số lượng tham số
- Kiểu từng tham số
- Thứ tự tham số

Ví dụ:

```cpp
void print(int, double);
```

Signature là

```text
print(int, double)
```

---

### Điều kiện để overload

Có thể overload bằng:

#### Khác số lượng tham số

```cpp
void print();
void print(int);
void print(int, int);
```

---

#### Khác kiểu dữ liệu

```cpp
void print(int);
void print(double);
void print(char);
```

---

#### Khác thứ tự tham số

```cpp
void func(int, double);
void func(double, int);
```

---

#### Khác const qualifier

```cpp
void func(std::string&);
void func(const std::string&);
```

---

### Không được overload bằng

#### Chỉ khác kiểu trả về

```cpp
int sum(int, int);

double sum(int, int);     // Lỗi
```

Compiler không thể biết cần chọn phiên bản nào từ lời gọi:

```cpp
sum(1, 2);
```

---

#### Chỉ khác tên biến

```cpp
void add(int x);

void add(int y);          // Lỗi
```

Tên biến không thuộc Function Signature.

---

### Compiler làm gì?

Compiler sử dụng cơ chế gọi là **Overload Resolution**.

Quy trình:

1. Thu thập tất cả các hàm cùng tên.
2. Loại bỏ các hàm không phù hợp.
3. So sánh mức độ khớp của từng hàm.
4. Chọn hàm phù hợp nhất.
5. Nếu không chọn được → báo lỗi **ambiguous**.

Ví dụ:

```cpp
void print(int);

void print(double);
```

Khi gọi

```cpp
print(10);
```

Compiler chọn

```cpp
print(int);
```

---

Khi gọi

```cpp
print(3.14);
```

Compiler chọn

```cpp
print(double);
```

---

### Chuyển đổi kiểu

Compiler có thể tự chuyển đổi kiểu nếu cần.

Ví dụ:

```cpp
void print(double);
```

Khi gọi

```cpp
print(10);
```

Compiler chuyển

```text
int -> double
```

rồi gọi

```cpp
print(double);
```

Tuy nhiên nếu tồn tại cả

```cpp
void print(int);

void print(double);
```

thì

```cpp
print(10);
```

sẽ ưu tiên

```cpp
print(int);
```

vì không cần chuyển đổi kiểu.

---

## 4. Thực hành

```cpp
#include <iostream>

void print(int value)
{
    std::cout << "int: " << value << '\n';
}

void print(double value)
{
    std::cout << "double: " << value << '\n';
}

void print(const char* text)
{
    std::cout << "string: " << text << '\n';
}

int main()
{
    print(10);

    print(3.14);

    print("Hello");

    return 0;
}
```

Kết quả:

```text
int: 10
double: 3.14
string: Hello
```

Compiler tự chọn phiên bản phù hợp dựa trên danh sách tham số.

---

## 5. Cạm bẫy / lưu ý

- Function Overloading chỉ phân biệt bằng **Function Signature**, không phải kiểu trả về.

- Không thể overload chỉ bằng kiểu trả về.

```cpp
int func(int);

double func(int);      // Lỗi
```

- Không thể overload chỉ bằng tên biến.

```cpp
void func(int x);

void func(int y);      // Lỗi
```

- Compiler luôn ưu tiên phiên bản **không cần chuyển đổi kiểu**.

Ví dụ:

```cpp
void func(int);

void func(double);

func(5);          // Chọn func(int)
```

- Cẩn thận với **default parameter**, rất dễ gây lỗi ambiguous.

Ví dụ:

```cpp
void func();

void func(int x = 0);

func();      // Lỗi
```

- Quá nhiều overload gần giống nhau có thể khiến lời gọi trở nên khó đoán và dễ phát sinh lỗi ambiguous.

Ví dụ:

```cpp
void func(short);
void func(int);
void func(long);
void func(float);
void func(double);
```

---

## 6. Câu hỏi mở

- [ ] Compiler thực hiện Overload Resolution chi tiết như thế nào?
- [ ] Thứ tự ưu tiên giữa Exact Match, Promotion và Conversion là gì?
- [ ] Khi nào compiler báo lỗi ambiguous?
- [ ] Constructor Overloading và Operator Overloading sử dụng Function Overloading như thế nào?
- [ ] Function Template và Function Overloading phối hợp với nhau ra sao?