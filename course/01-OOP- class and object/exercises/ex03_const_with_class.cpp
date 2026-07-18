// Bài tập: Const Correctness
// Dựa trên note: ../03_Const_with_Class.md
//
// Nhắc lại: const member function cam kết không sửa trạng thái object
// (this là const ClassName*). Object const chỉ được gọi method const.
// mutable cho phép ngoại lệ (cache, counter, mutex) nhưng không nên lạm dụng.

#include <iostream>

// ===== Bài 1 (dễ): Const-correctness cơ bản =====
// Class Point có getX()/getY() (chỉ đọc) và setX()/setY() (có sửa).
// TODO: thêm const vào đúng những hàm không sửa dữ liệu (getX, getY).
// Đừng thêm const vào setX/setY (sẽ không biên dịch được vì chúng có sửa).
class Point {
public:
    Point(int x, int y) : x_(x), y_(y) {}

    int getX() /* TODO: const */ { return x_; }
    int getY() /* TODO: const */ { return y_; }
    void setX(int v) { x_ = v; }
    void setY(int v) { y_ = v; }

private:
    int x_, y_;
};

// TODO: viết hàm tự do print(const Point& p) chỉ gọi getX()/getY().
// Nếu getX()/getY() chưa const, hàm này sẽ KHÔNG biên dịch được vì p là
// const Point& — đây chính là lý do vì sao getter cần const.
void print(const Point& p) {
    std::cout << "Point(" << p.getX() << ", " << p.getY() << ")\n";
}

// ===== Bài 2 (trung bình): mutable cho cache =====
// ExpensiveCalc giả lập một phép tính tốn kém. compute() là hàm chỉ đọc
// input (const) nhưng muốn cache kết quả lần tính đầu tiên để lần gọi sau
// không tính lại. TODO: dùng `mutable` cho 2 member cache để compute() vẫn
// khai báo được là const.
class ExpensiveCalc {
public:
    ExpensiveCalc(int input) : input_(input) /*, cached_(false), cache_(0) */ {}

    int compute() const {
        // TODO: nếu đã cache (cached_ == true) thì trả về cache_ luôn,
        // không tính lại. Nếu chưa, tính rồi lưu vào cache_, đặt cached_ = true.
        std::cout << "  (dang tinh...)\n";
        return input_ * input_;
    }

private:
    int input_;
    // TODO: mutable bool cached_;
    // TODO: mutable int cache_;
};

int main() {
    Point p(3, 4);
    print(p);
    p.setX(10);
    print(p);

    const Point cp(1, 2);
    // cp.setX(5); // phải là lỗi biên dịch nếu bỏ comment — object const
    print(cp);

    ExpensiveCalc calc(7);
    std::cout << "Lan 1: " << calc.compute() << "\n"; // tinh that
    std::cout << "Lan 2: " << calc.compute() << "\n"; // lay tu cache, khong in "dang tinh..."

    return 0;
}

/* ===== LỜI GIẢI =====

#include <iostream>

class Point {
public:
    Point(int x, int y) : x_(x), y_(y) {}

    int getX() const { return x_; }
    int getY() const { return y_; }
    void setX(int v) { x_ = v; }
    void setY(int v) { y_ = v; }

private:
    int x_, y_;
};

void print(const Point& p) {
    std::cout << "Point(" << p.getX() << ", " << p.getY() << ")\n";
}

class ExpensiveCalc {
public:
    ExpensiveCalc(int input) : input_(input), cached_(false), cache_(0) {}

    int compute() const {
        if (cached_) return cache_;
        std::cout << "  (dang tinh...)\n";
        cache_ = input_ * input_;
        cached_ = true;
        return cache_;
    }

private:
    int input_;
    mutable bool cached_;
    mutable int cache_;
};

int main() {
    Point p(3, 4);
    print(p);
    p.setX(10);
    print(p);

    const Point cp(1, 2);
    print(cp);

    ExpensiveCalc calc(7);
    std::cout << "Lan 1: " << calc.compute() << "\n";
    std::cout << "Lan 2: " << calc.compute() << "\n";

    return 0;
}

===================== */
