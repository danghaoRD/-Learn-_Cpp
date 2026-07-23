// Bài tập: Operator Overloading (member function)
// Dựa trên note: ../01_Overload_operator.md
//
// Nhắc lại: overload operator KHÔNG tạo toán tử mới, chỉ định nghĩa ý nghĩa
// cho toán tử đã tồn tại khi áp dụng lên kiểu tự tạo. Viết dưới dạng member
// thì toán hạng trái luôn là *this (ngầm), toán hạng phải là tham số.

#include <iostream>

// ===== Bài 1 (dễ): Vector2D — operator+ và operator- (member) =====
// TODO: viết operator+ trả về Vector2D mới (x+rhs.x, y+rhs.y), không sửa
// *this (hàm phải là const vì chỉ đọc).
// TODO: viết operator- tương tự, trừ từng thành phần.
class Vector2D {
public:
    Vector2D(double x, double y) : x(x), y(y) {}

    // TODO: Vector2D operator+(const Vector2D& rhs) const { ... }
    Vector2D operator+(const Vector2D& rhs) const {
        return Vector2D(x + rhs.x, y + rhs.y);
    }
    // TODO: Vector2D operator-(const Vector2D& rhs) const { ... }
    Vector2D operator-(const Vector2D& rhs) const {
        return Vector2D(x - rhs.x, y - rhs.y);
    }

    void print() const { std::cout << "(" << x << ", " << y << ")"; }

private:
    double x, y;
};

// ===== Bài 2 (trung bình): Fraction — operator== và operator+= (member) =====
// Phân số a/b. operator== so sánh bằng cross-multiplication (a1*b2 == a2*b1)
// để tránh phải quy đồng. operator+= sửa trực tiếp *this rồi trả về *this&
// (để hỗ trợ gán liên tiếp / dùng lại trong operator+ nếu cần).
// TODO: operator== trả về bool.
// TODO: operator+= cộng phân số rhs vào *this: a = a*rhs.b + rhs.a*b; b = b*rhs.b;
class Fraction {
public:
    Fraction(int num, int den) : num_(num), den_(den) {}

    // TODO: bool operator==(const Fraction& rhs) const { ... }
    // TODO: Fraction& operator+=(const Fraction& rhs) { ... return *this; }
    bool operator==(const Fraction& rhs) const {
        return num_ * rhs.den_ == rhs.num_ * den_;
    }
    Fraction& operator+=(const Fraction& rhs) {
        num_ = num_ * rhs.den_ + rhs.num_ * den_;
        den_ = den_ * rhs.den_;
        return *this;
    }
    void print() const { std::cout << num_ << "/" << den_; }

private:
    int num_, den_;
};

int main() {
    std::cout << "[Bai 1] Vector2D operator+ / operator-\n";
    Vector2D a(1.0, 2.0);
    Vector2D b(3.0, 4.0);
    Vector2D sum = a + b;
    Vector2D diff = a - b;
    std::cout << "a + b = "; sum.print(); std::cout << " (ky vong (4, 6))\n";
    std::cout << "a - b = "; diff.print(); std::cout << " (ky vong (-2, -2))\n";

    std::cout << "\n[Bai 2] Fraction operator== / operator+=\n";
    Fraction f1(1, 2);
    Fraction f2(2, 4);
    Fraction f3(1, 3);
    std::cout << "f1 == f2 (ky vong 1): " << (f1 == f2) << "\n";
    std::cout << "f1 == f3 (ky vong 0): " << (f1 == f3) << "\n";

    f1 += f3;   // 1/2 + 1/3 = 5/6 (chua rut gon, chi can dung cong thuc tren)
    std::cout << "f1 sau += f3 = "; f1.print(); std::cout << " (ky vong 5/6)\n";

    return 0;
}

/* ===== LỜI GIẢI =====

#include <iostream>

class Vector2D {
public:
    Vector2D(double x, double y) : x(x), y(y) {}

    Vector2D operator+(const Vector2D& rhs) const {
        return Vector2D(x + rhs.x, y + rhs.y);
    }

    Vector2D operator-(const Vector2D& rhs) const {
        return Vector2D(x - rhs.x, y - rhs.y);
    }

    void print() const { std::cout << "(" << x << ", " << y << ")"; }

private:
    double x, y;
};

class Fraction {
public:
    Fraction(int num, int den) : num_(num), den_(den) {}

    bool operator==(const Fraction& rhs) const {
        return num_ * rhs.den_ == rhs.num_ * den_;
    }

    Fraction& operator+=(const Fraction& rhs) {
        num_ = num_ * rhs.den_ + rhs.num_ * den_;
        den_ = den_ * rhs.den_;
        return *this;
    }

    void print() const { std::cout << num_ << "/" << den_; }

private:
    int num_, den_;
};

int main() {
    std::cout << "[Bai 1] Vector2D operator+ / operator-\n";
    Vector2D a(1.0, 2.0);
    Vector2D b(3.0, 4.0);
    Vector2D sum = a + b;
    Vector2D diff = a - b;
    std::cout << "a + b = "; sum.print(); std::cout << "\n";
    std::cout << "a - b = "; diff.print(); std::cout << "\n";

    std::cout << "\n[Bai 2] Fraction operator== / operator+=\n";
    Fraction f1(1, 2);
    Fraction f2(2, 4);
    Fraction f3(1, 3);
    std::cout << "f1 == f2: " << (f1 == f2) << "\n";
    std::cout << "f1 == f3: " << (f1 == f3) << "\n";

    f1 += f3;
    std::cout << "f1 sau += f3 = "; f1.print(); std::cout << "\n";

    return 0;
}

===================== */
