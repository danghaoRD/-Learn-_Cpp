
# Const Correctness trong C++

## 1. Vấn đề

> C++ giải quyết bài toán gì mà C thuần không làm được / làm được nhưng cồng kềnh, dễ lỗi?

Trong lập trình hướng đối tượng, có rất nhiều hàm chỉ dùng để **đọc trạng thái của object**.

Ví dụ:

- lấy giá trị cảm biến
- đọc trạng thái button
- lấy tên thiết bị
- kiểm tra kết nối BLE

Những hàm này **không nên làm thay đổi dữ liệu bên trong object**.

Nếu không có cơ chế ràng buộc:

- Người khác có thể vô tình sửa dữ liệu trong một hàm chỉ có nhiệm vụ đọc.
- Compiler không thể kiểm tra được điều đó.
- Khi truyền object cho các hàm chỉ đọc, compiler cũng không biết hàm nào an toàn để gọi.

C++ giải quyết bằng **const-correctness**.

Ý tưởng:

> Nếu một hàm không thay đổi trạng thái của object thì hãy khai báo `const`.

Compiler sẽ kiểm tra và đảm bảo lời cam kết này.

---

## 2. Đối chiếu với C

Trong C thường viết:

```c
typedef struct
{
    int age;
} Person;

int Person_GetAge(Person *p)
{
    return p->age;
}
```

Vấn đề:

Hàm trên hoàn toàn có thể sửa dữ liệu.

```c
int Person_GetAge(Person *p)
{
    p->age = 100;

    return p->age;
}
```

Tên hàm là `GetAge()` nhưng vẫn sửa dữ liệu.

Muốn ngăn điều đó phải tự dùng

```c
const Person *
```

```c
int Person_GetAge(const Person *p)
{
    return p->age;
}
```

Trong C++, ý tưởng này được mở rộng trực tiếp lên member function.

---

## 3. Cơ chế C++

### 3.1 Const member function

```cpp
class Person
{
public:

    int getAge() const
    {
        return age;
    }

private:

    int age = 20;
};
```

Từ khóa `const` ở cuối hàm có nghĩa:

> Hàm này cam kết không thay đổi trạng thái của object.

---

### 3.2 Compiler thực hiện điều gì?

Compiler ngầm biến

```cpp
int getAge() const;
```

thành

```cpp
int getAge(const Person* this);
```

Trong khi

```cpp
int getAge();
```

tương đương

```cpp
int getAge(Person* this);
```

Do `this` là `const Person*`, compiler sẽ không cho phép sửa member.

Ví dụ

```cpp
int getAge() const
{
    age = 100;      // Compile Error

    return age;
}
```

---

### 3.3 Const object

```cpp
const Person p;
```

Lúc này chỉ được gọi các hàm `const`.

```cpp
p.getAge();      // OK

p.setAge(20);    // Error
```

Compiler biết rằng `setAge()` có thể thay đổi object nên không cho phép.

---

### 3.4 Object thường

```cpp
Person p;
```

Có thể gọi cả hai.

```cpp
p.getAge();

p.setAge(20);
```

---

### 3.5 Quan hệ giữa object const và method const

Đây là điểm rất dễ nhầm.

Method `const`

> Chỉ nói về chính method đó.

Object `const`

> Nói về toàn bộ object.

Ví dụ

```cpp
Person p;

p.getAge();
```

và

```cpp
const Person p;

p.getAge();
```

Đều giống nhau.

Sự khác biệt chỉ xuất hiện khi gọi các hàm có thể thay đổi object.

---

### 3.6 Const Reference

Đây là cách truyền tham số phổ biến nhất trong C++.

```cpp
void print(const Person& p)
{
    std::cout << p.getAge();
}
```

Lợi ích:

- Không copy object.
- Đảm bảo hàm không sửa object.
- Có thể truyền cả object thường và object const.

---

## 4. Thực hành

```cpp
#include <iostream>

class Person
{
public:

    Person(int age)
        : age(age)
    {
    }

    int getAge() const
    {
        return age;
    }

    void setAge(int value)
    {
        age = value;
    }

private:

    int age;
};

int main()
{
    Person p(20);

    std::cout << p.getAge() << std::endl;

    p.setAge(30);

    std::cout << p.getAge() << std::endl;

    const Person p2(40);

    std::cout << p2.getAge() << std::endl;

    // p2.setAge(50);      // Compile Error
}
```

---

## 5. Cạm bẫy / lưu ý

### Getter nên là const

```cpp
int getValue() const;
```

Đây là cách viết chuẩn trong hầu hết thư viện C++.

---

### Setter không được là const

Sai

```cpp
void setValue(int v) const;
```

Vì setter luôn thay đổi trạng thái object.

---

### Method const chỉ gọi được method const

Ví dụ

```cpp
class A
{
public:

    void foo() const
    {
        bar();      // bar cũng phải là const
    }

    void bar() const
    {
    }
};
```

Nếu `bar()` không phải `const` thì sẽ lỗi compile.

---

### Không lạm dụng mutable

`mutable` cho phép sửa member ngay cả trong hàm const.

Chỉ nên dùng cho:

- cache
- bộ đếm thống kê
- mutex

Không nên dùng để phá bỏ ý nghĩa của `const`.

---

### Truyền object lớn bằng const reference

Ưu tiên

```cpp
void print(const Person& p);
```

Thay vì

```cpp
void print(Person p);
```

để tránh copy object.

---

### Const-correctness là thói quen tốt

Các thư viện như:

- STL
- Qt
- Boost
- Eigen

đều áp dụng rất triệt để.

Nhiều coding guideline như:

- C++ Core Guidelines
- Google C++ Style
- AUTOSAR C++
- MISRA C++

đều khuyến khích:

> Hàm không thay đổi trạng thái object nên khai báo `const`.

Không phải mọi dự án đều tuân thủ tuyệt đối, đặc biệt là các codebase cũ hoặc dự án chuyển từ C sang C++, nhưng đây vẫn được xem là thực hành tốt trong C++ hiện đại.

---

## 6. Câu hỏi mở

- `const` và `constexpr` khác nhau như thế nào?
- `const` và `#define` nên dùng khi nào?
- `mutable` hoạt động ra sao?
- Tại sao STL thường overload cả phiên bản `const` và non-const` của `operator[]` hoặc `begin()`?
- `const` với con trỏ (`const T*`, `T* const`, `const T* const`) khác nhau như thế nào?
- Khi nào nên trả về `const T&` thay vì `T`?
- `const_cast` là gì và tại sao rất ít khi nên dùng?