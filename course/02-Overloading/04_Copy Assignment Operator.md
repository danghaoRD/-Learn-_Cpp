# Copy Assignment Operator (Operator=) trong C++

## 1. Vấn đề
> C++ giải quyết bài toán gì mà C thuần không làm được / làm được nhưng cồng kềnh, dễ lỗi?

Khi một object đã được tạo, đôi khi ta muốn sao chép nội dung từ một object khác.

Ví dụ:

```cpp
String s1("Hello");
String s2("World");

s2 = s1;
```

Nếu compiler chỉ sao chép từng member (memberwise copy), các thành viên là con trỏ sẽ cùng trỏ đến một vùng nhớ.

```
s1 -------> "Hello"
              ^
              |
s2 -----------+
```

Hậu quả:

- Hai object cùng quản lý một tài nguyên.
- Thay đổi dữ liệu của object này ảnh hưởng object kia.
- Destructor có thể giải phóng cùng một vùng nhớ hai lần (double free).
- Dễ gây Undefined Behavior.

Để giải quyết, C++ cho phép **overload toán tử gán (`=`)**, giúp class tự định nghĩa cách sao chép dữ liệu giữa hai object đã tồn tại.

---

## 2. Đối chiếu với C
> Trên C tôi phải làm tương đương thế nào?

Trong C không có object và cũng không có toán tử `=` có thể overload.

Ta thường phải viết một hàm copy riêng.

```c
typedef struct
{
    char* data;
} String;

void String_Copy(String* dest, const String* src)
{
    free(dest->data);

    dest->data = malloc(strlen(src->data) + 1);
    strcpy(dest->data, src->data);
}
```

Sử dụng

```c
String_Copy(&s2, &s1);
```

Trong C++, logic này được đặt trong `operator=`.

---

## 3. Cơ chế C++
> Cú pháp, quy tắc hoạt động, những gì trình biên dịch làm ngầm?

### Khái niệm

Copy Assignment Operator có dạng

```cpp
ClassName& operator=(const ClassName& other);
```

Ví dụ

```cpp
class String
{
public:
    String& operator=(const String& other);
};
```

Khi viết

```cpp
a = b;
```

Compiler chuyển thành

```cpp
a.operator=(b);
```

---

### Khi nào Copy Assignment được gọi?

Copy Assignment chỉ được gọi khi **object đích đã tồn tại**.

```cpp
Point p1(10, 20);
Point p2(0, 0);

p2 = p1;
```

Compiler gọi

```cpp
p2.operator=(p1);
```

---

### Phân biệt với Copy Constructor

Đây là điểm rất dễ nhầm.

#### Copy Constructor

Được gọi khi tạo object mới.

```cpp
Point p2 = p1;
```

hoặc

```cpp
Point p2(p1);
```

Compiler gọi

```cpp
Point(const Point&);
```

---

#### Copy Assignment

Được gọi khi object đã tồn tại.

```cpp
Point p1;
Point p2;

p2 = p1;
```

Compiler gọi

```cpp
Point::operator=(const Point&);
```

---

### Compiler tự sinh Copy Assignment

Nếu không tự định nghĩa

```cpp
class Point
{
    int x;
    int y;
};
```

Compiler sẽ tự sinh

```cpp
Point& operator=(const Point&);
```

với hành vi tương đương

```cpp
x = other.x;
y = other.y;
```

Đây gọi là **memberwise copy**.

Nếu class chứa con trỏ hoặc tài nguyên, memberwise copy thường dẫn đến **shallow copy** và gây lỗi.

---

### Vì sao trả về `ClassName&`

Thông thường ta viết

```cpp
Point& operator=(const Point& other);
```

để hỗ trợ phép gán liên tiếp.

```cpp
a = b = c;
```

Compiler thực hiện

```
b = c

↓

trả về b

↓

a = b
```

Nếu trả về `void` thì biểu thức trên sẽ không hợp lệ.

---

### Self Assignment

Một trường hợp cần chú ý

```cpp
a = a;
```

Nếu class quản lý tài nguyên

```cpp
String& operator=(const String& other)
{
    delete[] data;

    data = new char[strlen(other.data) + 1);

    strcpy(data, other.data);

    return *this;
}
```

thì khi

```cpp
a = a;
```

`other.data` cũng chính là `data`.

Sau khi `delete[] data`, `other.data` trở thành con trỏ không hợp lệ.

Vì vậy thường kiểm tra

```cpp
if (this == &other)
{
    return *this;
}
```

---

### Deep Copy

Mục tiêu chính của Copy Assignment là tạo bản sao độc lập.

```cpp
String& operator=(const String& other)
{
    if (this == &other)
    {
        return *this;
    }

    delete[] data;

    data = new char[strlen(other.data) + 1);

    strcpy(data, other.data);

    return *this;
}
```

Sau khi gán

```
s1 -------> "Hello"

s2 -------> "Hello"
```

Hai object sở hữu hai vùng nhớ khác nhau.

Đây gọi là **Deep Copy**.

---

## 4. Thực hành

```cpp
#include <cstring>
#include <iostream>

class String
{
private:
    char* data;

public:
    String(const char* str)
    {
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }

    ~String()
    {
        delete[] data;
    }

    String& operator=(const String& other)
    {
        if (this == &other)
        {
            return *this;
        }

        delete[] data;

        data = new char[strlen(other.data) + 1];
        strcpy(data, other.data);

        return *this;
    }

    void print() const
    {
        std::cout << data << '\n';
    }
};

int main()
{
    String s1("Hello");
    String s2("World");

    s2 = s1;

    s1.print();
    s2.print();

    return 0;
}
```

Kết quả

```
Hello
Hello
```

Sau phép gán, `s1` và `s2` chứa cùng nội dung nhưng quản lý hai vùng nhớ khác nhau.

---

## 5. Cạm bẫy / lưu ý

- Copy Assignment chỉ được gọi khi object đích **đã tồn tại**.

- `Point p2 = p1;` gọi **Copy Constructor**, không phải Copy Assignment.

- Compiler sẽ tự sinh Copy Assignment nếu người lập trình không định nghĩa.

- Copy Assignment mặc định thực hiện **memberwise copy**, có thể gây **shallow copy** nếu class quản lý tài nguyên.

- Luôn trả về `ClassName&` để hỗ trợ phép gán liên tiếp.

- Nên kiểm tra **self assignment** bằng

```cpp
if (this == &other)
```

- Nếu class tự quản lý tài nguyên (bộ nhớ động, file, socket...), thường phải tự viết Copy Assignment.

- Nếu đã tự viết Destructor thì thường cũng nên tự viết Copy Constructor và Copy Assignment (**Rule of Three**).

---

## 6. Câu hỏi mở

- [ ] Memberwise Copy và Deep Copy khác nhau như thế nào?
- [ ] Khi nào compiler không tự sinh Copy Assignment?
- [ ] Copy Assignment và Move Assignment khác nhau ở đâu?
- [ ] Rule of Three và Rule of Five liên quan thế nào tới Copy Assignment?
- [ ] Có nên dùng kỹ thuật Copy-and-Swap để cài đặt Copy Assignment không?