# Stream Insertion và Stream Extraction Operator Overloading trong C++

## 1. Vấn đề
> C++ giải quyết bài toán gì mà C thuần không làm được / làm được nhưng cồng kềnh, dễ lỗi?

Các kiểu dữ liệu có sẵn trong C++ như `int`, `double`, `std::string` có thể làm việc trực tiếp với stream.

Ví dụ:

```cpp
int age = 20;

std::cout << age;
std::cin >> age;
```

Nhưng với một class tự định nghĩa:

```cpp
Point p(10, 20);

std::cout << p;
```

Compiler sẽ báo lỗi vì không biết phải chuyển object thành dữ liệu để ghi ra stream như thế nào.

Tương tự,

```cpp
Point p;

std::cin >> p;
```

cũng không hoạt động vì compiler không biết cách đọc dữ liệu để gán vào object.

Trong C++, ta có thể overload `operator<<` và `operator>>` để object hoạt động giống như các kiểu dữ liệu có sẵn.

---

## 2. Đối chiếu với C
> Trên C tôi phải làm tương đương thế nào?

Trong C không có operator overloading.

Thông thường ta phải tự viết các hàm:

```c
typedef struct
{
    int x;
    int y;
} Point;

void Point_Print(const Point* p)
{
    printf("(%d, %d)\n", p->x, p->y);
}

void Point_Input(Point* p)
{
    scanf("%d%d", &p->x, &p->y);
}
```

Sử dụng:

```c
Point p;

Point_Input(&p);
Point_Print(&p);
```

Trong C++, việc này trở nên tự nhiên hơn:

```cpp
Point p;

std::cin >> p;
std::cout << p;
```

---

## 3. Cơ chế C++
> Cú pháp, quy tắc hoạt động, những gì trình biên dịch làm ngầm?

### Stream Insertion Operator (`<<`)

```cpp
std::ostream&
operator<<(std::ostream& os,
           const Point& p)
{
    os << "("
       << p.x
       << ", "
       << p.y
       << ")";

    return os;
}
```

Khi viết

```cpp
std::cout << p;
```

Compiler chuyển thành

```cpp
operator<<(std::cout, p);
```

---

### Stream Extraction Operator (`>>`)

```cpp
std::istream&
operator>>(std::istream& is,
           Point& p)
{
    is >> p.x >> p.y;

    return is;
}
```

Khi viết

```cpp
std::cin >> p;
```

Compiler chuyển thành

```cpp
operator>>(std::cin, p);
```

---

### Có phải đây là một cơ chế overload mới?

Không.

Đây vẫn là **Operator Overloading**.

Compiler xử lý giống mọi toán tử khác.

Ví dụ:

```cpp
a + b;
```

↓

```cpp
operator+(a, b);
```

và

```cpp
std::cout << p;
```

↓

```cpp
operator<<(std::cout, p);
```

Chỉ khác ở chỗ toán hạng bên trái là một đối tượng `std::ostream`.

---

### Vì sao phải là Global Function?

Đây là điểm quan trọng nhất của `operator<<` và `operator>>`.

Nếu viết dưới dạng member function:

```cpp
class Point
{
public:
    std::ostream&
    operator<<(std::ostream&);
};
```

thì lời gọi sẽ là

```cpp
p << std::cout;
```

tương đương

```cpp
p.operator<<(std::cout);
```

Trong khi điều chúng ta cần là

```cpp
std::cout << p;
```

Mà toán hạng bên trái là

```cpp
std::ostream
```

Ta không thể sửa lớp `std::ostream` của thư viện chuẩn để thêm member function mới.

Do đó `operator<<` và `operator>>` gần như luôn được cài đặt dưới dạng **non-member (global) function**.

---

### Friend có bắt buộc không?

Không.

Nếu class cung cấp getter:

```cpp
class Point
{
public:
    int getX() const;
    int getY() const;
};
```

thì hoàn toàn có thể viết:

```cpp
std::ostream&
operator<<(std::ostream& os,
           const Point& p)
{
    os << "("
       << p.getX()
       << ", "
       << p.getY()
       << ")";

    return os;
}
```

Nếu muốn truy cập trực tiếp vào thành viên private thì mới cần:

```cpp
friend std::ostream&
operator<<(std::ostream&, const Point&);

friend std::istream&
operator>>(std::istream&, Point&);
```

---

### Vì sao phải trả về `ostream&` và `istream&`?

Để hỗ trợ chain operation.

Ví dụ:

```cpp
std::cout << p1 << p2 << p3;
```

Compiler thực hiện:

```cpp
operator<<(
    operator<<(
        operator<<(std::cout, p1),
        p2),
    p3);
```

Nếu không trả về `std::ostream&` thì lời gọi tiếp theo sẽ không có stream để tiếp tục.

Điều tương tự cũng áp dụng cho

```cpp
std::cin >> p1 >> p2;
```

---

## 4. Thực hành

```cpp
#include <iostream>

class Point
{
private:
    int x;
    int y;

public:
    Point(int x = 0, int y = 0)
        : x(x), y(y)
    {
    }

    friend std::ostream&
    operator<<(std::ostream& os, const Point& p);

    friend std::istream&
    operator>>(std::istream& is, Point& p);
};

std::ostream&
operator<<(std::ostream& os, const Point& p)
{
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

std::istream&
operator>>(std::istream& is, Point& p)
{
    is >> p.x >> p.y;
    return is;
}

int main()
{
    Point p;

    std::cout << "Nhap x y: ";

    std::cin >> p;

    std::cout << "Point = " << p << '\n';

    return 0;
}
```

Ví dụ nhập:

```
10 20
```

Kết quả:

```
Point = (10, 20)
```

---

## 5. Cạm bẫy / lưu ý

- `operator<<` và `operator>>` **không phải là cơ chế overload mới**, mà vẫn sử dụng cơ chế Operator Overloading thông thường.

- Hai operator này gần như luôn được cài đặt dưới dạng **non-member (global) function**, vì toán hạng bên trái là `std::ostream` hoặc `std::istream`.

- Chỉ nên khai báo `friend` nếu thực sự cần truy cập trực tiếp vào thành viên private.

- Luôn trả về `std::ostream&` hoặc `std::istream&` để hỗ trợ chain operation.

- Tham số object của `operator<<` nên truyền bằng **const reference** vì chỉ đọc dữ liệu.

```cpp
const Point&
```

- Tham số object của `operator>>` **không thể là const** vì cần ghi dữ liệu vào object.

```cpp
Point&
```

- Không nên in trực tiếp bằng `std::cout` bên trong `operator<<`.

Sai:

```cpp
std::ostream&
operator<<(std::ostream& os, const Point& p)
{
    std::cout << p.x;
    return os;
}
```

Đúng:

```cpp
os << p.x;
```

Làm như vậy operator sẽ hoạt động với mọi loại stream:

- `std::cout`
- `std::ofstream`
- `std::ostringstream`

---

## 6. Câu hỏi mở

- [ ] `std::ofstream << object` hoạt động như thế nào?
- [ ] `std::stringstream >> object` có dùng cùng `operator>>` không?
- [ ] Có nên overload `operator<<` dưới dạng member function không?
- [ ] Khi nào nên dùng getter thay cho `friend`?
- [ ] Có thể overload `operator<<` cho nhiều định dạng hiển thị khác nhau không?