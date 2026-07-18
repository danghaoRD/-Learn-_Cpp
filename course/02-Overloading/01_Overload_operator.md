# Operator Overloading (Nạp chồng toán tử) GPT

## 1. Vấn đề
> C++ giải quyết bài toán gì mà C thuần không làm được / làm được nhưng cồng kềnh, dễ lỗi?

Trong C, các toán tử (`+`, `-`, `*`, `==`, `[]`...) chỉ hoạt động với **kiểu dữ liệu built-in** như `int`, `float`, `double`,...

Ví dụ:

```c
int a = 5;
int b = 8;

int c = a + b;
```

Nhưng nếu ta định nghĩa một kiểu dữ liệu mới:

```cpp
class Point
{
public:
    int x;
    int y;
};
```

thì phép toán sau sẽ không hợp lệ:

```cpp
Point p3 = p1 + p2;
```

Bởi vì compiler không biết:

- Cộng tọa độ?
- Cộng khoảng cách?
- Lấy trung bình?
- Hay một ý nghĩa khác?

Chỉ người thiết kế class mới biết.

C++ cho phép lập trình viên **định nghĩa ý nghĩa của các toán tử** đối với kiểu dữ liệu tự tạo thông qua **Operator Overloading**.

Mục tiêu:

- Code tự nhiên hơn.
- API dễ đọc hơn.
- Object có thể sử dụng giống các kiểu dữ liệu cơ bản.

---

## 2. Đối chiếu với C

### C

Muốn cộng hai Point phải viết:

```c
typedef struct
{
    int x;
    int y;
} Point;

Point Point_Add(Point a, Point b)
{
    Point result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;

    return result;
}

Point p3 = Point_Add(p1, p2);
```

Tên hàm thường dài.

Không tự nhiên.

---

### C++

```cpp
Point p3 = p1 + p2;
```

Nhìn giống như cộng hai số nguyên.

Ý nghĩa của toán tử được định nghĩa trong class.

---

## 3. Cơ chế C++

### Bản chất

Operator Overloading **không tạo ra toán tử mới**.

Nó chỉ định nghĩa cách hoạt động của **những toán tử đã tồn tại trong C++**.

Ví dụ:

```cpp
Point p3 = p1 + p2;
```

Compiler sẽ hiểu gần giống:

```cpp
Point p3 = p1.operator+(p2);
```

hoặc

```cpp
Point p3 = operator+(p1, p2);
```

nếu overload bằng non-member function.

Nói cách khác:

> **Toán tử thực chất chỉ là một hàm có tên đặc biệt.**

---

### Member Operator

Ví dụ:

```cpp
class Point
{
public:

    Point operator+(const Point& rhs);
};
```

Lời gọi:

```cpp
p1 + p2;
```

Compiler gần như dịch thành:

```cpp
p1.operator+(p2);
```

---

### Non-member Operator

```cpp
Point operator+(const Point& lhs,
                const Point& rhs);
```

Compiler sẽ dịch:

```cpp
operator+(p1, p2);
```

---

### Compiler làm gì?

Khi gặp:

```cpp
p1 + p2;
```

Compiler thực hiện:

```
Tìm operator+

↓

Overload Resolution

↓

Chọn operator phù hợp

↓

Sinh lời gọi hàm

↓

Linker nối symbol

↓

CPU thực thi
```

Sau khi compiler chọn được đúng hàm, operator không còn gì đặc biệt nữa.

Nó được xử lý giống hệt mọi function khác.

---

### Name Mangling

Ví dụ:

```cpp
Point Point::operator+(const Point&);
```

Compiler sẽ đổi thành symbol nội bộ (ví dụ):

```
_ZN5PointplERKS_
```

Linker chỉ làm việc với symbol này.

Không có khái niệm "operator" ở giai đoạn linker.

---

### Built-in Type khác User-defined Type

Đối với:

```cpp
5 + 8
```

Compiler sinh trực tiếp lệnh CPU:

```
ADD
```

Không hề tồn tại hàm:

```cpp
int operator+(int, int);
```

Đối với object:

```cpp
p1 + p2;
```

Compiler phải tìm:

```cpp
operator+(...)
```

do lập trình viên định nghĩa.

---

### Runtime Cost

Operator Overloading là **Compile-time Polymorphism**.

Sau khi compile:

```
p1 + p2

↓

Point::operator+(...)

↓

call

hoặc

inline
```

Không có:

- virtual table
- dynamic dispatch
- runtime lookup

Nếu compiler inline được thì gần như không còn chi phí gọi hàm.

Chi phí thực tế nằm ở logic bên trong operator chứ không phải ở cơ chế overloading.

---

## 4. Thực hành

```cpp
#include <iostream>

class Point
{
public:

    int x;
    int y;

    Point(int x, int y)
        : x(x), y(y)
    {
    }

    Point operator+(const Point& rhs) const
    {
        return Point(
            x + rhs.x,
            y + rhs.y
        );
    }
};

int main()
{
    Point p1(1, 2);
    Point p2(3, 4);

    Point p3 = p1 + p2;

    std::cout
        << p3.x
        << ", "
        << p3.y
        << std::endl;

    return 0;
}
```

Output:

```
4,6
```

Có thể hình dung compiler thực hiện:

```cpp
Point p3 = p1.operator+(p2);
```

---

Ví dụ non-member:

```cpp
class Point
{
public:

    int x;
    int y;
};

Point operator+(const Point& lhs,
                const Point& rhs)
{
    return
    {
        lhs.x + rhs.x,
        lhs.y + rhs.y
    };
}
```

---

## 5. Cạm bẫy / lưu ý

### Operator Overloading ≠ Tạo toán tử mới

Không thể tạo:

```text
***
<+>
%%%
```

Chỉ overload được các toán tử đã tồn tại trong C++.

---

### Không phải toán tử nào cũng overload được

Ví dụ không overload được:

```
.
.*
::
?:
sizeof
```

---

### Không thay đổi được precedence

Ví dụ:

```cpp
a + b * c;
```

vẫn luôn được hiểu là:

```cpp
a + (b * c);
```

Operator Overloading chỉ thay đổi **ý nghĩa**, không thay đổi **độ ưu tiên**.

---

### Không nên lạm dụng

Ví dụ:

```cpp
Money total = a + b;
```

Rất tự nhiên.

Nhưng:

```cpp
User c = a + b;
```

Người đọc sẽ không biết:

- Ghép user?
- Kết bạn?
- Gộp dữ liệu?

Nếu ý nghĩa không trực quan, nên dùng hàm thông thường.

---

### Member hay Non-member?

Thông thường:

Member:

```
+=
-=
*=
/=
[]
()
=
```

Non-member:

```
+
-
*
/
==
!=
<
>
```

Lý do:

Hai toán hạng bình đẳng thì thường nên dùng non-member.

---

### Không có overhead đặc biệt

Operator Overloading chỉ là:

```
Toán tử

↓

Function

↓

Assembly
```

Nếu compiler inline được thì gần như không còn lời gọi hàm.

---

### Không nhầm với Overriding

Operator **Overloading**:

- Compile-time
- Chọn hàm khi compile

Virtual **Overriding**:

- Runtime
- Chọn hàm qua vtable

Đây là hai khái niệm hoàn toàn khác nhau.

---

## 6. Câu hỏi mở

- [ ] Khi nào nên dùng member operator và khi nào nên dùng non-member operator?
- [ ] Tại sao `operator+` thường được cài đặt dựa trên `operator+=`?
- [ ] `operator=` (copy assignment) và Copy Constructor khác nhau như thế nào?
- [ ] Có thể overload `new`, `delete`, `()`, `[]`, `->` không?
- [ ] Compiler thực hiện Overload Resolution cho operator như thế nào?
- [ ] Assembly sinh ra từ `p1 + p2` khác gì so với `Point_Add(p1, p2)` trong C?
- [ ] Khi bật `-O2` hoặc `-O3`, compiler inline `operator+` trong những trường hợp nào?