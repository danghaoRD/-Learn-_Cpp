// Bài tập: Overloading Operator as Global Function
// Dựa trên note: ../06_Overloading operator as global function.md
//
// Nhắc lại: member operator ép toán hạng trái phải đúng là kiểu class (vì
// nó được truyền ngầm qua this). Muốn hai toán hạng "bình đẳng" (cho phép
// implicit conversion ở CẢ hai vế, ví dụ 5 + p cũng hợp lệ như p + 5), phải
// viết operator dạng global (non-member). Nếu cần đọc private member thì
// phải khai báo friend trong class.

#include <iostream>

// ===== Bài 1 (dễ): Point — operator+ và operator== dạng global =====
// TODO: khai báo friend cho 2 hàm operator+ và operator== bên trong class.
// TODO: định nghĩa operator+(const Point&, const Point&) — cộng từng thành
// phần, trả về Point mới.
// TODO: định nghĩa operator==(const Point&, const Point&) — so sánh x và y.
class Point {
public:
    Point(int x, int y) : x_(x), y_(y) {}

    void print() const { std::cout << "(" << x_ << ", " << y_ << ")"; }

    // TODO: friend Point operator+(const Point& lhs, const Point& rhs);
    // TODO: friend bool operator==(const Point& lhs, const Point& rhs);
    friend Point operator+(const Point& lhs, const Point& rhs);
    friend bool operator==(const Point& lhs, const Point& rhs);

private:
    int x_, y_;
};

// TODO: Point operator+(const Point& lhs, const Point& rhs) { ... }
// TODO: bool operator==(const Point& lhs, const Point& rhs) { ... }
Point operator+(const Point& lhs, const Point& rhs) {
    return Point(lhs.x_ + rhs.x_, lhs.y_ + rhs.y_);
}
bool operator==(const Point& lhs, const Point& rhs) {
    return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_;
}
// ===== Bài 2 (trung bình): Money — global operator+ tận dụng implicit
// conversion ở CẢ hai vế =====
// Money có constructor 1 tham số (không explicit) -> compiler được phép
// convert ngầm int -> Money ở vị trí THAM SỐ. Vì global operator+ coi cả
// 2 toán hạng đều là tham số, nên cả "m + 5" lẫn "5 + m" đều hợp lệ (khác
// với member operator, chỉ "m + 5" hợp lệ vì toán hạng trái member operator
// không phải tham số).
// TODO: Money(int cents) — constructor 1 tham số, không explicit.
// TODO: friend operator+(const Money&, const Money&) trong class.
// TODO: định nghĩa global operator+ cộng cents.
class Money {
public:
    // TODO: Money(int cents) : cents_(cents) {}
    Money(int cents) : cents_(cents) {}

    void print() const { std::cout << cents_ << " cents"; }

    // TODO: friend Money operator+(const Money& lhs, const Money& rhs);
    friend Money operator+(const Money& lhs, const Money& rhs);
private:
    int cents_ = 0;
};

// TODO: Money operator+(const Money& lhs, const Money& rhs) { ... }
  Money operator+(const Money& lhs, const Money& rhs) {
    return Money(lhs.cents_ + rhs.cents_);
  }
int main() {
    std::cout << "[Bai 1] Point - global operator+ / operator==\n";
    Point p1(1, 2);
    Point p2(3, 4);
    Point p3 = p1 + p2;
    p3.print(); std::cout << " (ky vong (4, 6))\n";
    std::cout << "p1 == p1 (ky vong 1): " << (p1 == p1) << "\n";
    std::cout << "p1 == p2 (ky vong 0): " << (p1 == p2) << "\n";

    std::cout << "\n[Bai 2] Money - operator+ doi xung ca hai ve\n";
    Money m1(100);
    Money total1 = m1 + 50;   // 50 (int) convert ngam -> Money
    Money total2 = 50 + m1;   // chi hoat dong duoc vi la global function
    total1.print(); std::cout << " (ky vong 150 cents)\n";
    total2.print(); std::cout << " (ky vong 150 cents)\n";

    return 0;
}

/* ===== LỜI GIẢI =====

#include <iostream>

class Point {
public:
    Point(int x, int y) : x_(x), y_(y) {}

    void print() const { std::cout << "(" << x_ << ", " << y_ << ")"; }

    friend Point operator+(const Point& lhs, const Point& rhs);
    friend bool operator==(const Point& lhs, const Point& rhs);

private:
    int x_, y_;
};

Point operator+(const Point& lhs, const Point& rhs) {
    return Point(lhs.x_ + rhs.x_, lhs.y_ + rhs.y_);
}

bool operator==(const Point& lhs, const Point& rhs) {
    return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_;
}

class Money {
public:
    Money(int cents) : cents_(cents) {}

    void print() const { std::cout << cents_ << " cents"; }

    friend Money operator+(const Money& lhs, const Money& rhs);

private:
    int cents_ = 0;
};

Money operator+(const Money& lhs, const Money& rhs) {
    return Money(lhs.cents_ + rhs.cents_);
}

int main() {
    std::cout << "[Bai 1] Point - global operator+ / operator==\n";
    Point p1(1, 2);
    Point p2(3, 4);
    Point p3 = p1 + p2;
    p3.print(); std::cout << "\n";
    std::cout << "p1 == p1: " << (p1 == p1) << "\n";
    std::cout << "p1 == p2: " << (p1 == p2) << "\n";

    std::cout << "\n[Bai 2] Money - operator+ doi xung ca hai ve\n";
    Money m1(100);
    Money total1 = m1 + 50;
    Money total2 = 50 + m1;
    total1.print(); std::cout << "\n";
    total2.print(); std::cout << "\n";

    return 0;
}

===================== */
