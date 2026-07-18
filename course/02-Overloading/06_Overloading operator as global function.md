# Overloading Operator as Global Function (Nạp chồng toán tử bằng hàm global)

## 1. Vấn đề
> C++ giải quyết bài toán gì mà C thuần không làm được / làm được nhưng cồng kềnh, dễ lỗi?

Khi overload operator bằng **member function** (`Point::operator+`), toán hạng bên trái
luôn bị ép buộc phải là chính kiểu class đó, vì nó được truyền ngầm qua `this`:

```cpp
class Point
{
public:
    Point operator+(const Point& rhs) const;
};

p1 + p2;   // OK: dịch thành p1.operator+(p2)
5 + p1;    // LỖI: int không có method operator+
```

Nếu muốn `p1 + p2` và `5 + p1` đều hợp lệ (hai toán hạng "bình đẳng", được phép
convert ngầm ở cả hai vế), member function không làm được — cần một dạng khác:
overload operator bằng **hàm global (non-member, free function)**, đứng độc lập
ngoài class.

## 2. Đối chiếu với C

Trong C, cách duy nhất để "cộng" hai `Point` là viết một hàm độc lập:

```c
typedef struct { int x, y; } Point;

Point Point_Add(Point a, Point b)
{
    Point result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

Point p3 = Point_Add(p1, p2);
```

Hàm này không thuộc về `struct` nào cả — nó nhận cả hai toán hạng làm tham số,
đối xứng hoàn toàn. **Overload operator dạng global function trong C++ chính là
hàm này**, chỉ khác là compiler cho phép gọi bằng cú pháp `p1 + p2` thay vì
`Point_Add(p1, p2)`.

## 3. Cơ chế C++

### Khai báo

Hàm operator được viết **bên ngoài class**, nhận đủ toán hạng làm tham số
(khác với member: chỉ nhận toán hạng phải, toán hạng trái là `this` ngầm):

```cpp
class Point
{
public:
    int x, y;
};

Point operator+(const Point& lhs, const Point& rhs)
{
    return { lhs.x + rhs.x, lhs.y + rhs.y };
}
```

Khi gặp `p1 + p2`, compiler tìm và dịch thành:

```cpp
operator+(p1, p2);
```

Không có `this`, không có gì "thuộc về" object nào — đây là một hàm bình thường,
chỉ được compiler ưu ái gọi bằng cú pháp toán tử.

### Truy cập private member: cần `friend`

Nếu hàm global cần đọc field private, phải khai `friend` bên trong class để
"mở khóa" quyền truy cập — giống cách bạn cấp quyền đặc biệt cho một hàm C
được đọc thẳng vào struct nội bộ của module khác:

```cpp
class Point
{
    int x, y;   // private

public:
    Point(int x, int y) : x(x), y(y) {}

    friend Point operator+(const Point& lhs, const Point& rhs);
};

Point operator+(const Point& lhs, const Point& rhs)
{
    return { lhs.x + rhs.x, lhs.y + rhs.y };   // OK vì friend
}
```

`friend` không phải là một phần của Operator Overloading — nó chỉ là cơ chế
cấp quyền truy cập private, dùng vì hàm nằm ngoài class.

### Vì sao cần đối xứng cho một số toán tử

Overload resolution cho phép implicit conversion ở **tham số**, nhưng member
function không cho phép convert **toán hạng trái** (vì nó là `this`, không
phải tham số thường). Global function coi cả hai toán hạng đều là tham số,
nên cả hai đều được phép convert ngầm:

```cpp
class Point
{
public:
    Point(int x, int y) : x(x), y(y) {}
    int x, y;
};

Point operator+(const Point& lhs, const Point& rhs)
{
    return { lhs.x + rhs.x, lhs.y + rhs.y };
}

// Point có constructor 2 tham số, không tự convert từ 1 giá trị -> ví dụ dưới
// chỉ minh hoạ nguyên tắc, không compile trực tiếp với class Point ở trên
```

Nguyên tắc: nếu class có constructor 1-tham số (hoặc `explicit` không chặn),
compiler có thể convert ngầm kiểu built-in thành object đó — điều này chỉ
hoạt động ở vị trí *tham số*, nên chỉ global function mới tận dụng được cho
cả hai vế.

## 4. Thực hành

```cpp
#include <iostream>

class Point
{
    int x, y;

public:
    Point(int x, int y) : x(x), y(y) {}

    void print() const
    {
        std::cout << x << ", " << y << std::endl;
    }

    friend Point operator+(const Point& lhs, const Point& rhs);
    friend bool operator==(const Point& lhs, const Point& rhs);
};

// Global function: cả 2 toán hạng đều là tham số tường minh
Point operator+(const Point& lhs, const Point& rhs)
{
    return Point(lhs.x + rhs.x, lhs.y + rhs.y);
}

bool operator==(const Point& lhs, const Point& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

int main()
{
    Point p1(1, 2);
    Point p2(3, 4);

    Point p3 = p1 + p2;   // -> operator+(p1, p2)
    p3.print();           // 4, 6

    std::cout << (p1 == p1) << std::endl;   // -> operator==(p1, p1) -> 1

    return 0;
}
```

So sánh với bản C (`Point_Add(p1, p2)`), bản C++ chỉ khác cú pháp gọi;
bản chất — một hàm nhận 2 tham số, trả về giá trị — giống hệt nhau.

## 5. Cạm bẫy / lưu ý

- **Vẫn cần ít nhất 1 tham số là kiểu tự định nghĩa.** Không thể viết
  `operator+(int, int)` để đổi nghĩa `5 + 8` — compiler chặn overload
  operator thuần built-in type.
- **Quên `friend` → lỗi truy cập, không phải lỗi cú pháp.** Nếu hàm global
  cần đọc private member mà không khai `friend`, compiler báo lỗi "member is
  private" — dễ nhầm tưởng operator viết sai cú pháp.
- **`friend` phá vỡ encapsulation ở mức độ nhỏ.** Nó cho một hàm ngoài class
  toàn quyền truy cập private — nên chỉ dùng khi thực sự cần đối xứng
  (`+ - * / == != < >`), không lạm dụng cho mọi operator.
- **Không có chi phí runtime khác biệt so với member.** Cả hai đều resolve ở
  compile-time; khác nhau duy nhất là cách truyền toán hạng trái (`this` ngầm
  vs tham số tường minh).
- **Đối với `+=`, `-=`, `=`, `[]`, `()`: nên dùng member, không dùng global.**
  Các toán tử này về bản chất sửa đổi/truy cập trực tiếp toán hạng trái, không
  có nhu cầu đối xứng hay implicit conversion ở vế trái.

## 6. Câu hỏi mở

- [ ] `friend` có phải là cách duy nhất để global operator truy cập private
      member, hay có thể dùng getter/public interface thay thế?
- [ ] Khi nào implicit conversion ở tham số của global operator gây ra bug
      khó phát hiện (ví dụ: constructor không đánh dấu `explicit`)?
- [ ] `operator<<` cho `std::ostream` (dùng để `std::cout << p1`) — tại sao
      bắt buộc phải là global function chứ không thể là member?
