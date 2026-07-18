# Constructor Overloading trong C++

## 1. Vấn đề
> C++ giải quyết bài toán gì mà C thuần không làm được / làm được nhưng cồng kềnh, dễ lỗi?

Khi tạo một đối tượng, ta thường có nhiều cách khởi tạo khác nhau.

Ví dụ với một lớp `Point`:

- Tạo điểm tại gốc tọa độ `(0, 0)`
- Tạo điểm với tọa độ bất kỳ `(x, y)`
- Sao chép từ một đối tượng khác

Nếu chỉ có một constructor, người lập trình phải truyền các giá trị giả hoặc viết nhiều hàm khởi tạo riêng, làm code kém tự nhiên.

Constructor Overloading cho phép một class có nhiều constructor với các danh sách tham số khác nhau, giúp việc tạo object linh hoạt và rõ ràng hơn.

---

## 2. Đối chiếu với C
> Trên C tôi phải làm tương đương thế nào?

Trong C không có constructor.

Ta thường làm như sau:

```c
typedef struct
{
    int x;
    int y;
} Point;

void Point_Init(Point* p)
{
    p->x = 0;
    p->y = 0;
}

void Point_InitXY(Point* p, int x, int y)
{
    p->x = x;
    p->y = y;
}

void Point_Copy(Point* dest, const Point* src)
{
    dest->x = src->x;
    dest->y = src->y;
}
```

Sử dụng:

```c
Point p1;
Point_Init(&p1);

Point p2;
Point_InitXY(&p2, 10, 20);

Point p3;
Point_Copy(&p3, &p2);
```

Mỗi cách khởi tạo tương ứng với một hàm khác nhau.

Trong C++, tất cả được gom thành các constructor của cùng một class.

---

## 3. Cơ chế C++
> Cú pháp, quy tắc hoạt động, những gì trình biên dịch làm ngầm?

### Khái niệm

Một class có thể có nhiều constructor miễn là **khác danh sách tham số**.

```cpp
class Point
{
public:
    Point();
    Point(int x);
    Point(int x, int y);
};
```

### Constructor Overloading có phải cơ chế mới?

Không.

Về bản chất, **constructor overloading chính là function overloading** áp dụng cho constructor.

Compiler sử dụng cùng một cơ chế:

1. Thu thập tất cả constructor có cùng tên.
2. So khớp tham số.
3. Chọn constructor phù hợp nhất (Overload Resolution).
4. Nếu không chọn được ⇒ báo lỗi **ambiguous**.

Ví dụ:

```cpp
Point p;
```

Compiler sẽ chọn

```cpp
Point();
```

---

```cpp
Point p(10);
```

Compiler sẽ chọn

```cpp
Point(int);
```

---

```cpp
Point p(10, 20);
```

Compiler sẽ chọn

```cpp
Point(int, int);
```

### Khác gì Function Overloading?

Về **cơ chế overload**, hoàn toàn giống Function Overloading.

Điểm khác là constructor là **special member function**, nên có thêm các quy tắc riêng:

- Không có kiểu trả về.
- Tên phải trùng tên class.
- Được gọi khi object được tạo.
- Có Initializer List.
- Có Delegating Constructor.
- Có Default / Copy / Move Constructor.
- Compiler có thể tự sinh constructor.

Vì constructor liên quan trực tiếp đến **quá trình khởi tạo object**, nên tài liệu C++ thường tách Constructor Overloading thành một chủ đề riêng mặc dù cơ chế overload là giống nhau.

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
    Point()
        : x(0), y(0)
    {
        std::cout << "Default constructor\n";
    }

    Point(int value)
        : x(value), y(0)
    {
        std::cout << "One parameter constructor\n";
    }

    Point(int x, int y)
        : x(x), y(y)
    {
        std::cout << "Two parameter constructor\n";
    }

    void print() const
    {
        std::cout << "(" << x << ", " << y << ")\n";
    }
};

int main()
{
    Point p1;
    Point p2(5);
    Point p3(10, 20);

    p1.print();
    p2.print();
    p3.print();

    return 0;
}
```

Kết quả:

```
Default constructor
One parameter constructor
Two parameter constructor

(0, 0)
(5, 0)
(10, 20)
```

Compiler tự chọn constructor phù hợp dựa vào danh sách tham số.

---

## 5. Cạm bẫy / lưu ý

- Constructor Overloading **không phải là một cơ chế overload mới**, mà sử dụng cùng cơ chế Overload Resolution như Function Overloading.

- Chỉ được overload bằng **danh sách tham số**, không phải kiểu trả về.

- Cẩn thận với **default parameter**, vì rất dễ gây lỗi ambiguous.

Ví dụ:

```cpp
class Demo
{
public:
    Demo();
    Demo(int x = 0);
};

Demo d;      // Lỗi: compiler không biết chọn constructor nào
```

- Nếu khai báo bất kỳ constructor nào, compiler sẽ **không tự sinh Default Constructor** nữa.

Ví dụ:

```cpp
class Point
{
public:
    Point(int x);
};

Point p;      // Lỗi
```

- Nên dùng **Initializer List** thay vì gán trong thân constructor.

- Khi nhiều constructor có logic giống nhau, nên dùng **Delegating Constructor** để tránh lặp code.

---

## 6. Câu hỏi mở

- [ ] Compiler thực hiện Overload Resolution như thế nào?
- [ ] Khi nào constructor bị đánh giá là ambiguous?
- [ ] Delegating Constructor hoạt động ra sao?
- [ ] Constructor Overloading khác gì Default Constructor?
- [ ] Constructor Overloading liên quan thế nào tới Copy Constructor và Move Constructor?# Constructor Overloading trong C++

## 1. Vấn đề
> C++ giải quyết bài toán gì mà C thuần không làm được / làm được nhưng cồng kềnh, dễ lỗi?

Khi tạo một đối tượng, ta thường có nhiều cách khởi tạo khác nhau.

Ví dụ với một lớp `Point`:

- Tạo điểm tại gốc tọa độ `(0, 0)`
- Tạo điểm với tọa độ bất kỳ `(x, y)`
- Sao chép từ một đối tượng khác

Nếu chỉ có một constructor, người lập trình phải truyền các giá trị giả hoặc viết nhiều hàm khởi tạo riêng, làm code kém tự nhiên.

Constructor Overloading cho phép một class có nhiều constructor với các danh sách tham số khác nhau, giúp việc tạo object linh hoạt và rõ ràng hơn.

---

## 2. Đối chiếu với C
> Trên C tôi phải làm tương đương thế nào?

Trong C không có constructor.

Ta thường làm như sau:

```c
typedef struct
{
    int x;
    int y;
} Point;

void Point_Init(Point* p)
{
    p->x = 0;
    p->y = 0;
}

void Point_InitXY(Point* p, int x, int y)
{
    p->x = x;
    p->y = y;
}

void Point_Copy(Point* dest, const Point* src)
{
    dest->x = src->x;
    dest->y = src->y;
}
```

Sử dụng:

```c
Point p1;
Point_Init(&p1);

Point p2;
Point_InitXY(&p2, 10, 20);

Point p3;
Point_Copy(&p3, &p2);
```

Mỗi cách khởi tạo tương ứng với một hàm khác nhau.

Trong C++, tất cả được gom thành các constructor của cùng một class.

---

## 3. Cơ chế C++
> Cú pháp, quy tắc hoạt động, những gì trình biên dịch làm ngầm?

### Khái niệm

Một class có thể có nhiều constructor miễn là **khác danh sách tham số**.

```cpp
class Point
{
public:
    Point();
    Point(int x);
    Point(int x, int y);
};
```

### Constructor Overloading có phải cơ chế mới?

Không.

Về bản chất, **constructor overloading chính là function overloading** áp dụng cho constructor.

Compiler sử dụng cùng một cơ chế:

1. Thu thập tất cả constructor có cùng tên.
2. So khớp tham số.
3. Chọn constructor phù hợp nhất (Overload Resolution).
4. Nếu không chọn được ⇒ báo lỗi **ambiguous**.

Ví dụ:

```cpp
Point p;
```

Compiler sẽ chọn

```cpp
Point();
```

---

```cpp
Point p(10);
```

Compiler sẽ chọn

```cpp
Point(int);
```

---

```cpp
Point p(10, 20);
```

Compiler sẽ chọn

```cpp
Point(int, int);
```

### Khác gì Function Overloading?

Về **cơ chế overload**, hoàn toàn giống Function Overloading.

Điểm khác là constructor là **special member function**, nên có thêm các quy tắc riêng:

- Không có kiểu trả về.
- Tên phải trùng tên class.
- Được gọi khi object được tạo.
- Có Initializer List.
- Có Delegating Constructor.
- Có Default / Copy / Move Constructor.
- Compiler có thể tự sinh constructor.

Vì constructor liên quan trực tiếp đến **quá trình khởi tạo object**, nên tài liệu C++ thường tách Constructor Overloading thành một chủ đề riêng mặc dù cơ chế overload là giống nhau.

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
    Point()
        : x(0), y(0)
    {
        std::cout << "Default constructor\n";
    }

    Point(int value)
        : x(value), y(0)
    {
        std::cout << "One parameter constructor\n";
    }

    Point(int x, int y)
        : x(x), y(y)
    {
        std::cout << "Two parameter constructor\n";
    }

    void print() const
    {
        std::cout << "(" << x << ", " << y << ")\n";
    }
};

int main()
{
    Point p1;
    Point p2(5);
    Point p3(10, 20);

    p1.print();
    p2.print();
    p3.print();

    return 0;
}
```

Kết quả:

```
Default constructor
One parameter constructor
Two parameter constructor

(0, 0)
(5, 0)
(10, 20)
```

Compiler tự chọn constructor phù hợp dựa vào danh sách tham số.

---

## 5. Cạm bẫy / lưu ý

- Constructor Overloading **không phải là một cơ chế overload mới**, mà sử dụng cùng cơ chế Overload Resolution như Function Overloading.

- Chỉ được overload bằng **danh sách tham số**, không phải kiểu trả về.

- Cẩn thận với **default parameter**, vì rất dễ gây lỗi ambiguous.

Ví dụ:

```cpp
class Demo
{
public:
    Demo();
    Demo(int x = 0);
};

Demo d;      // Lỗi: compiler không biết chọn constructor nào
```

- Nếu khai báo bất kỳ constructor nào, compiler sẽ **không tự sinh Default Constructor** nữa.

Ví dụ:

```cpp
class Point
{
public:
    Point(int x);
};
# Constructor Overloading trong C++

## 1. Vấn đề
> C++ giải quyết bài toán gì mà C thuần không làm được / làm được nhưng cồng kềnh, dễ lỗi?

Khi tạo một đối tượng, ta thường có nhiều cách khởi tạo khác nhau.

Ví dụ với một lớp `Point`:

- Tạo điểm tại gốc tọa độ `(0, 0)`
- Tạo điểm với tọa độ bất kỳ `(x, y)`
- Sao chép từ một đối tượng khác

Nếu chỉ có một constructor, người lập trình phải truyền các giá trị giả hoặc viết nhiều hàm khởi tạo riêng, làm code kém tự nhiên.

Constructor Overloading cho phép một class có nhiều constructor với các danh sách tham số khác nhau, giúp việc tạo object linh hoạt và rõ ràng hơn.

---

## 2. Đối chiếu với C
> Trên C tôi phải làm tương đương thế nào?

Trong C không có constructor.

Ta thường làm như sau:

```c
typedef struct
{
    int x;
    int y;
} Point;

void Point_Init(Point* p)
{
    p->x = 0;
    p->y = 0;
}

void Point_InitXY(Point* p, int x, int y)
{
    p->x = x;
    p->y = y;
}

void Point_Copy(Point* dest, const Point* src)
{
    dest->x = src->x;
    dest->y = src->y;
}
```

Sử dụng:

```c
Point p1;
Point_Init(&p1);

Point p2;
Point_InitXY(&p2, 10, 20);

Point p3;
Point_Copy(&p3, &p2);
```

Mỗi cách khởi tạo tương ứng với một hàm khác nhau.

Trong C++, tất cả được gom thành các constructor của cùng một class.

---

## 3. Cơ chế C++
> Cú pháp, quy tắc hoạt động, những gì trình biên dịch làm ngầm?

### Khái niệm

Một class có thể có nhiều constructor miễn là **khác danh sách tham số**.

```cpp
class Point
{
public:
    Point();
    Point(int x);
    Point(int x, int y);
};
```

### Constructor Overloading có phải cơ chế mới?

Không.

Về bản chất, **constructor overloading chính là function overloading** áp dụng cho constructor.

Compiler sử dụng cùng một cơ chế:

1. Thu thập tất cả constructor có cùng tên.
2. So khớp tham số.
3. Chọn constructor phù hợp nhất (Overload Resolution).
4. Nếu không chọn được ⇒ báo lỗi **ambiguous**.

Ví dụ:

```cpp
Point p;
```

Compiler sẽ chọn

```cpp
Point();
```

---

```cpp
Point p(10);
```

Compiler sẽ chọn

```cpp
Point(int);
```

---

```cpp
Point p(10, 20);
```

Compiler sẽ chọn

```cpp
Point(int, int);
```

### Khác gì Function Overloading?

Về **cơ chế overload**, hoàn toàn giống Function Overloading.

Điểm khác là constructor là **special member function**, nên có thêm các quy tắc riêng:

- Không có kiểu trả về.
- Tên phải trùng tên class.
- Được gọi khi object được tạo.
- Có Initializer List.
- Có Delegating Constructor.
- Có Default / Copy / Move Constructor.
- Compiler có thể tự sinh constructor.

Vì constructor liên quan trực tiếp đến **quá trình khởi tạo object**, nên tài liệu C++ thường tách Constructor Overloading thành một chủ đề riêng mặc dù cơ chế overload là giống nhau.

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
    Point()
        : x(0), y(0)
    {
        std::cout << "Default constructor\n";
    }

    Point(int value)
        : x(value), y(0)
    {
        std::cout << "One parameter constructor\n";
    }

    Point(int x, int y)
        : x(x), y(y)
    {
        std::cout << "Two parameter constructor\n";
    }

    void print() const
    {
        std::cout << "(" << x << ", " << y << ")\n";
    }
};

int main()
{
    Point p1;
    Point p2(5);
    Point p3(10, 20);

    p1.print();
    p2.print();
    p3.print();

    return 0;
}
```

Kết quả:

```
Default constructor
One parameter constructor
Two parameter constructor

(0, 0)
(5, 0)
(10, 20)
```

Compiler tự chọn constructor phù hợp dựa vào danh sách tham số.

---

## 5. Cạm bẫy / lưu ý

- Constructor Overloading **không phải là một cơ chế overload mới**, mà sử dụng cùng cơ chế Overload Resolution như Function Overloading.

- Chỉ được overload bằng **danh sách tham số**, không phải kiểu trả về.

- Cẩn thận với **default parameter**, vì rất dễ gây lỗi ambiguous.

Ví dụ:

```cpp
class Demo
{
public:
    Demo();
    Demo(int x = 0);
};

Demo d;      // Lỗi: compiler không biết chọn constructor nào
```

- Nếu khai báo bất kỳ constructor nào, compiler sẽ **không tự sinh Default Constructor** nữa.

Ví dụ:

```cpp
class Point
{
public:
    Point(int x);
};

Point p;      // Lỗi
```

- Nên dùng **Initializer List** thay vì gán trong thân constructor.

- Khi nhiều constructor có logic giống nhau, nên dùng **Delegating Constructor** để tránh lặp code.

---

## 6. Câu hỏi mở

- [ ] Compiler thực hiện Overload Resolution như thế nào?
- [ ] Khi nào constructor bị đánh giá là ambiguous?
- [ ] Delegating Constructor hoạt động ra sao?
- [ ] Constructor Overloading khác gì Default Constructor?
- [ ] Constructor Overloading liên quan thế nào tới Copy Constructor và Move Constructor?
Point p;      // Lỗi
```

- Nên dùng **Initializer List** thay vì gán trong thân constructor.

- Khi nhiều constructor có logic giống nhau, nên dùng **Delegating Constructor** để tránh lặp code.

---

## 6. Câu hỏi mở

- [ ] Compiler thực hiện Overload Resolution như thế nào?
- [ ] Khi nào constructor bị đánh giá là ambiguous?
- [ ] Delegating Constructor hoạt động ra sao?
- [ ] Constructor Overloading khác gì Default Constructor?
- [ ] Constructor Overloading liên quan thế nào tới Copy Constructor và Move Constructor?