# Struct và Class trong C++

## 1. Vấn đề
> C++ giải quyết bài toán gì mà C thuần không làm được / làm được nhưng cồng kềnh, dễ lỗi?

Trong C, `struct` chỉ là một cách đóng gói dữ liệu. Nếu muốn biểu diễn một “đối tượng” có cả dữ liệu lẫn hành vi, C phải dùng `struct` kết hợp với các hàm và con trỏ hàm, rất dễ sai và khó bảo trì.

C++ mở rộng `struct` và `class` để trở thành công cụ lập trình hướng đối tượng. Hai từ khóa này có thể chứa:

- dữ liệu thành viên
- hàm thành viên
- constructor/destructor
- access specifier (`public`, `private`, `protected`)
- inheritance

Vấn đề là nhiều người mới học vẫn tưởng `struct` và `class` là hoàn toàn khác nhau. Trong C++, chúng thực ra rất gần nhau.

---

## 2. Đối chiếu với C

Trong C, nếu muốn mô tả một người, thường làm như sau:

```c
struct Person
{
    int age;
    char name[32];
};

void Person_Init(struct Person* p, int age, const char* name)
{
    p->age = age;
    strcpy(p->name, name);
}
```

Trong C++, bạn có thể viết:

```cpp
struct Person
{
    int age;
    std::string name;

    Person(int a, std::string n) : age(a), name(std::move(n)) {}
};
```

Điểm khác biệt lớn là:

- C++ cho phép `struct` có hàm thành viên và constructor.
- Không cần phải viết hàm khởi tạo riêng bằng tay như trong C.
- Có cơ chế kiểm soát truy cập dữ liệu, giúp bảo vệ trạng thái của object.

---

## 3. Cơ chế C++

### 3.1 Điểm chung

`struct` và `class` trong C++ về bản chất là giống nhau. Cả hai đều có thể:

- chứa dữ liệu
- chứa hàm
- có constructor/destructor
- có inheritance
- có polymorphism

Ví dụ sau là tương đương:

```cpp
struct A
{
    void hello() { std::cout << "hello\n"; }
};

class B
{
public:
    void hello() { std::cout << "hello\n"; }
};
```

### 3.2 Điểm khác biệt duy nhất về mặc định

Đây là quy tắc quan trọng nhất:

- `struct`: thành viên mặc định là `public`
- `class`: thành viên mặc định là `private`

```cpp
struct Person1
{
    int age;   // public mặc định
};

class Person2
{
    int age;   // private mặc định
};
```

Nếu muốn truy cập trong trường hợp `class`, phải khai báo `public`:

```cpp
class Person2
{
public:
    int age;
};
```

### 3.3 Vì sao có sự khác biệt này?

Lịch sử của C++ bắt nguồn từ C. `struct` vốn đã tồn tại trong C, nên C++ giữ ý nghĩa cũ là “đóng gói dữ liệu đơn giản”. Còn `class` được thiết kế để biểu diễn lập trình hướng đối tượng với tính đóng gói chặt chẽ hơn.

Nói cách khác:

- dùng `struct` khi muốn kiểu dữ liệu “nhẹ”, gần dữ liệu thuần túy
- dùng `class` khi muốn ưu tiên tính đóng gói và abstraction

### 3.4 Về inheritance

`struct` và `class` khi kế thừa cũng khác nhau ở default access:

```cpp
struct Base {}
struct Derived : Base {};

class Base2 {}
class Derived2 : Base2 {};
```

- `struct` kế thừa mặc định là `public`
- `class` kế thừa mặc định là `private`

---

## 4. Thực hành

```cpp
#include <iostream>
#include <string>

struct StudentStruct
{
    std::string name;

    void print() const
    {
        std::cout << "StudentStruct: " << name << "\n";
    }
};

class StudentClass
{
public:
    std::string name;

    void print() const
    {
        std::cout << "StudentClass: " << name << "\n";
    }
};

int main()
{
    StudentStruct s1;
    s1.name = "An";
    s1.print();

    StudentClass s2;
    s2.name = "Binh";
    s2.print();
}
```

Trong ví dụ này:

- cả hai đều có dữ liệu và hàm thành viên
- cả hai đều có thể dùng như một object
- khác biệt duy nhất là cách truy cập thành viên mặc định

Nếu đổi `class StudentClass` thành `struct StudentClass`, code vẫn chạy được, chỉ khác là `name` mặc định là `public` rồi.

---

## 5. Cạm bẫy / lưu ý

- Đừng nghĩ `struct` và `class` là “hai thế giới khác nhau”. Trong C++, chúng gần như tương đương về khả năng.
- Điểm khác biệt thực sự quan trọng là access control mặc định.
- Khi chuyển từ C sang C++, nếu bạn dùng `struct` chỉ để chứa dữ liệu thuần, điều đó vẫn hợp lệ, nhưng nếu muốn đóng gói tốt hơn thì nên dùng `class`.
- Với người mới học, dễ nhầm lẫn khi thấy code dùng `struct` nhưng lại có hàm thành viên, constructor, hay inheritance. Điều đó là hoàn toàn bình thường.
- Trong code C++ hiện đại, `struct` thường dùng cho kiểu dữ liệu đơn giản, POD-like, hoặc khi muốn giữ giao diện public mặc định. `class` thường dùng cho các kiểu có logic phức tạp hơn.

---

## 6. Câu hỏi mở
- [ ] Khi nào nên dùng `struct` thay vì `class` trong một dự án C++ thực tế?
- [ ] Nếu không có `public`/`private`, thì `struct` và `class` khác nhau thế nào?
- [ ] Trong thiết kế hệ thống embedded, có nên dùng `struct` cho dữ liệu thuần và `class` cho logic không?
