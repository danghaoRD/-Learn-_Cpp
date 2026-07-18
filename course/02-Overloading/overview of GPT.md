# Overloading (Function Overloading, Constructor Overloading, Operator Overloading)

## 1. Vấn đề
> C++ giải quyết bài toán gì mà C thuần không làm được / làm được nhưng cồng kềnh, dễ lỗi?

Trong C, nếu cùng một thao tác nhưng áp dụng cho nhiều kiểu dữ liệu khác nhau, ta phải đặt nhiều tên hàm khác nhau.

Ví dụ:

```c
print_int(10);
print_float(3.14);
print_string("Hello");
```

Điều này dẫn đến:

- Khó nhớ tên hàm.
- API dài và thiếu nhất quán.
- Khi số lượng kiểu dữ liệu tăng, số lượng tên hàm cũng tăng theo.

C++ giải quyết vấn đề này bằng **Overloading (nạp chồng)**.

Ý tưởng rất đơn giản:

> **Một hành động → Một tên hàm → Nhiều cách thực hiện tùy theo tham số truyền vào.**

---

## 2. Đối chiếu với C

### C

```c
void print_int(int x);
void print_float(float x);
void print_string(char *s);

print_int(10);
print_float(3.14f);
print_string("Hello");
```

Mỗi kiểu dữ liệu phải có một tên hàm riêng.

---

### C++

```cpp
void print(int x);
void print(float x);
void print(const std::string& s);

print(10);
print(3.14f);
print("Hello");
```

Người dùng chỉ cần nhớ duy nhất một tên hàm.

Compiler sẽ tự quyết định gọi phiên bản phù hợp.

---

## 3. Cơ chế C++

### Khái niệm

Overloading là việc nhiều hàm có cùng tên nhưng khác **parameter list**.

Parameter list bao gồm:

- Số lượng tham số
- Kiểu tham số
- Thứ tự tham số

Ví dụ:

```cpp
void foo(int);

void foo(float);

void foo(int, int);

void foo(float, int);
```

Đều hợp lệ.

---

### Compiler phân biệt overload như thế nào?

Compiler **không** dựa vào:

- tên biến
- kiểu trả về

Compiler **chỉ dựa vào parameter list**.

Ví dụ:

```cpp
void foo(int);

void foo(float);

foo(5);
```

Compiler sẽ chọn

```cpp
foo(int)
```

ngay trong lúc biên dịch.

---

### Name Mangling

Trong C:

```cpp
print
```

chỉ tồn tại một symbol.

Trong C++:

```cpp
void print(int);
void print(float);
```

compiler sẽ đổi thành các symbol khác nhau (name mangling), ví dụ:

```
_Z5printi

_Z5printf
```

Do đó linker vẫn phân biệt được các hàm này.

Đây cũng là lý do khi gọi thư viện C từ C++ phải dùng:

```cpp
extern "C"
{
    void foo();
}
```

để tắt name mangling.

---

### Compile-time Polymorphism

Overloading được quyết định hoàn toàn trong lúc compile.

Không có virtual.

Không có vtable.

Không có dynamic dispatch.

Đây là một dạng:

> **Static (Compile-time) Polymorphism**

---

### Các loại Overloading

#### Function Overloading

```cpp
void print(int);

void print(float);

void print(std::string);
```

---

#### Constructor Overloading

```cpp
class Person
{
public:

    Person();

    Person(std::string name);

    Person(std::string name, int age);
};
```

---

#### Operator Overloading

```cpp
Point p3 = p1 + p2;
```

Toán tử `+` thực chất là một hàm được lập trình viên định nghĩa.

---

## 4. Thực hành

```cpp
#include <iostream>
#include <string>

using namespace std;

void print(int value)
{
    cout << "int: " << value << endl;
}

void print(double value)
{
    cout << "double: " << value << endl;
}

void print(const string& value)
{
    cout << "string: " << value << endl;
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

```
int: 10
double: 3.14
string: Hello
```

---

Ví dụ constructor overloading:

```cpp
class Person
{
public:

    Person()
    {
        cout << "Default\n";
    }

    Person(string name)
    {
        cout << name << endl;
    }

    Person(string name, int age)
    {
        cout << name << " " << age << endl;
    }
};

int main()
{
    Person p1;

    Person p2("Tom");

    Person p3("Tom", 20);
}
```

---

## 5. Cạm bẫy / lưu ý

### Không được overload chỉ khác kiểu trả về

Sai:

```cpp
int foo();

float foo();
```

Compiler không thể biết cần gọi hàm nào khi:

```cpp
foo();
```

---

### Không được chỉ khác tên biến

Sai:

```cpp
void foo(int x);

void foo(int y);
```

Tên biến không phải là một phần của chữ ký hàm (function signature).

---

### Default Argument có thể gây mơ hồ

Ví dụ:

```cpp
void foo(int);

void foo(int, int = 0);

foo(10);
```

Compiler không biết nên gọi hàm nào.

Đây là một lỗi rất phổ biến khi kết hợp **Overloading** và **Default Argument**.

---

### Implicit Conversion có thể chọn hàm ngoài mong muốn

Ví dụ:

```cpp
void foo(int);

void foo(double);

foo('A');
```

`char` có thể chuyển thành `int`, nên compiler sẽ gọi:

```cpp
foo(int)
```

Hiểu cơ chế **Overload Resolution** rất quan trọng để tránh các trường hợp khó đoán.

---

### Overloading ≠ Overriding

Overloading:

- Cùng tên
- Khác tham số
- Thường trong cùng một phạm vi (class hoặc namespace)
- Compile-time

Overriding:

- Class con ghi đè hàm virtual của class cha
- Runtime

Đây là hai khái niệm hoàn toàn khác nhau.

---

## 6. Câu hỏi mở

- [ ] Compiler quyết định chọn overload theo quy tắc nào (Overload Resolution)?
- [ ] Khi có implicit conversion, compiler ưu tiên hàm nào?
- [ ] Template và Overloading khác nhau ở điểm nào?
- [ ] Có thể overload những toán tử nào? Những toán tử nào không được overload?
- [ ] Constructor Overloading liên quan gì đến Copy Constructor và Move Constructor?
- [ ] Tại sao `extern "C"` lại vô hiệu hóa Name Mangling?