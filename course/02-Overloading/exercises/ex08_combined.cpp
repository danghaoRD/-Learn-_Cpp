// Bài tập kết hợp — dùng nhiều khái niệm Overloading cùng lúc trong 1 class.
// Tham khảo lại: ../02_Overload_constructor.md, ../04_Copy Assignment Operator.md,
//                ../05_move_Assignment Operator, ../06_Overloading operator as global function.md,
//                ../07_Insertion_Extraction_Overloading.md

#include <cstring>
#include <iostream>
#include <sstream>

// ===================================================================
// Bài A: Fraction — Constructor Overloading + Rule of Five (rút gọn cho
// kiểu không quản lý tài nguyên động, nên chỉ cần operator+ Rule of Three)
// + operator toán học global + stream insertion.
// ===================================================================
//
// Bối cảnh: Fraction (phân số) là kiểu giá trị đơn giản (chỉ 2 int, không
// cấp phát động) nhưng vẫn cần: nhiều cách khởi tạo (constructor overload),
// phép cộng đối xứng cả 2 vế (global operator+, hữu ích nếu sau này thêm
// implicit conversion từ int), và in ra màn hình tự nhiên (operator<<).
class Fraction {
public:
    // TODO: Fraction() — mac dinh 0/1
    Fraction() : num_(0), den_(1) {}

    // TODO: Fraction(int whole) — so nguyen, tuong duong whole/1
    Fraction(int whole) : num_(whole), den_(1) {}

    // TODO: Fraction(int num, int den) — phan so num/den
    Fraction(int num, int den) : num_(num), den_(den) {}

    // TODO: friend Fraction operator+(const Fraction& lhs, const Fraction& rhs);
    friend Fraction operator+(const Fraction& lhs, const Fraction& rhs);

    // TODO: friend std::ostream& operator<<(std::ostream& os, const Fraction& f);
    friend std::ostream& operator<<(std::ostream& os, const Fraction& f);

private:
    int num_, den_;
};

// TODO: operator+ cong 2 phan so: a/b + c/d = (a*d + c*b) / (b*d)
Fraction operator+(const Fraction& lhs, const Fraction& rhs) {
    return Fraction(0, 1); // TODO: sua lai cho dung
}

// TODO: operator<< in dang "num/den"
std::ostream& operator<<(std::ostream& os, const Fraction& f) {
    os << f.num_ << "?" << f.den_; // TODO: sua lai cho dung dinh dang num/den
    return os;
}

// ===================================================================
// Bài B: DynamicArray — Copy Assignment + Move Assignment (Rule of Five
// đầy đủ cho kiểu quản lý tài nguyên động) + operator<< để debug.
// ===================================================================
//
// Bối cảnh: buffer chứa dữ liệu double cấp phát động, cần deep copy khi
// copy assignment, "cướp" con trỏ khi move assignment (tránh copy thừa),
// và in nội dung qua operator<< để debug khi cần.
class DynamicArray {
public:
    DynamicArray(size_t n) : len_(n) {
        data_ = new double[len_];
        for (size_t i = 0; i < len_; ++i) data_[i] = 0.0;
    }

    ~DynamicArray() { delete[] data_; }

    DynamicArray(const DynamicArray& other) : len_(other.len_) {
        data_ = new double[len_];
        std::memcpy(data_, other.data_, len_ * sizeof(double));
    }

    // TODO: copy assignment — self-assignment guard, giai phong data_ cu,
    // deep copy tu other
    DynamicArray& operator=(const DynamicArray& other) {
        return *this; // TODO: sua lai cho dung
    }

    // TODO: move assignment noexcept — self-assignment guard, giai phong
    // data_ cu, cuop con tro cua other, dua other ve trang thai rong (nullptr, 0)
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        return *this; // TODO: sua lai cho dung
    }

    void set(size_t i, double v) { data_[i] = v; }
    double get(size_t i) const { return data_[i]; }
    size_t size() const { return len_; }

    friend std::ostream& operator<<(std::ostream& os, const DynamicArray& a);

private:
    double* data_ = nullptr;
    size_t len_ = 0;
};

std::ostream& operator<<(std::ostream& os, const DynamicArray& a) {
    os << "[";
    for (size_t i = 0; i < a.len_; ++i) {
        os << a.data_[i];
        if (i + 1 < a.len_) os << ", ";
    }
    os << "]";
    return os;
}

int main() {
    std::cout << "[Bai A] Fraction - constructor overload + global operator+ + operator<<\n";
    Fraction f1(1, 2);
    Fraction f2(1, 3);
    Fraction f3 = 5;              // constructor 1 tham so
    Fraction sum = f1 + f2;        // 1/2 + 1/3 = 5/6

    std::cout << "f1 = " << f1 << " (ky vong 1/2)\n";
    std::cout << "f3 = " << f3 << " (ky vong 5/1)\n";
    std::cout << "f1 + f2 = " << sum << " (ky vong 5/6)\n";

    std::cout << "\n[Bai B] DynamicArray - copy assignment vs move assignment\n";
    DynamicArray a(3);
    a.set(0, 1.5); a.set(1, 2.5); a.set(2, 3.5);

    DynamicArray b(1);
    b = a;                          // copy assignment (lvalue)
    b.set(0, 999.0);
    std::cout << "a = " << a << " (ky vong [1.5, 2.5, 3.5], khong doi)\n";
    std::cout << "b = " << b << " (ky vong [999, 2.5, 3.5])\n";

    DynamicArray c(1);
    c = DynamicArray(2);            // rvalue -> move assignment
    std::cout << "c.size() sau move = " << c.size() << " (ky vong 2)\n";

    return 0;
}

/* ===== LỜI GIẢI =====

#include <cstring>
#include <iostream>
#include <sstream>

class Fraction {
public:
    Fraction() : num_(0), den_(1) {}
    Fraction(int whole) : num_(whole), den_(1) {}
    Fraction(int num, int den) : num_(num), den_(den) {}

    friend Fraction operator+(const Fraction& lhs, const Fraction& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Fraction& f);

private:
    int num_, den_;
};

Fraction operator+(const Fraction& lhs, const Fraction& rhs) {
    return Fraction(lhs.num_ * rhs.den_ + rhs.num_ * lhs.den_,
                     lhs.den_ * rhs.den_);
}

std::ostream& operator<<(std::ostream& os, const Fraction& f) {
    os << f.num_ << "/" << f.den_;
    return os;
}

class DynamicArray {
public:
    DynamicArray(size_t n) : len_(n) {
        data_ = new double[len_];
        for (size_t i = 0; i < len_; ++i) data_[i] = 0.0;
    }

    ~DynamicArray() { delete[] data_; }

    DynamicArray(const DynamicArray& other) : len_(other.len_) {
        data_ = new double[len_];
        std::memcpy(data_, other.data_, len_ * sizeof(double));
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) return *this;
        delete[] data_;
        len_ = other.len_;
        data_ = new double[len_];
        std::memcpy(data_, other.data_, len_ * sizeof(double));
        return *this;
    }

    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this == &other) return *this;
        delete[] data_;
        data_ = other.data_;
        len_ = other.len_;
        other.data_ = nullptr;
        other.len_ = 0;
        return *this;
    }

    void set(size_t i, double v) { data_[i] = v; }
    double get(size_t i) const { return data_[i]; }
    size_t size() const { return len_; }

    friend std::ostream& operator<<(std::ostream& os, const DynamicArray& a);

private:
    double* data_ = nullptr;
    size_t len_ = 0;
};

std::ostream& operator<<(std::ostream& os, const DynamicArray& a) {
    os << "[";
    for (size_t i = 0; i < a.len_; ++i) {
        os << a.data_[i];
        if (i + 1 < a.len_) os << ", ";
    }
    os << "]";
    return os;
}

int main() {
    std::cout << "[Bai A] Fraction - constructor overload + global operator+ + operator<<\n";
    Fraction f1(1, 2);
    Fraction f2(1, 3);
    Fraction f3 = 5;
    Fraction sum = f1 + f2;

    std::cout << "f1 = " << f1 << "\n";
    std::cout << "f3 = " << f3 << "\n";
    std::cout << "f1 + f2 = " << sum << "\n";

    std::cout << "\n[Bai B] DynamicArray - copy assignment vs move assignment\n";
    DynamicArray a(3);
    a.set(0, 1.5); a.set(1, 2.5); a.set(2, 3.5);

    DynamicArray b(1);
    b = a;
    b.set(0, 999.0);
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";

    DynamicArray c(1);
    c = DynamicArray(2);
    std::cout << "c.size() sau move = " << c.size() << "\n";

    return 0;
}

===================== */
