// Bài tập: Constructor Overloading
// Dựa trên note: ../02_Overload_constructor.md
//
// Nhắc lại: constructor overloading dùng đúng cơ chế Overload Resolution
// như function overloading, chỉ khác ở chỗ constructor là special member
// function (không tên trả về, tên trùng class). Nếu khai báo bất kỳ
// constructor nào, compiler KHÔNG tự sinh default constructor nữa.

#include <iostream>

// ===== Bài 1 (dễ): Rectangle — 3 constructor =====
// TODO: Rectangle() — hình vuông cạnh mặc định 1x1
// TODO: Rectangle(int side) — hình vuông cạnh "side"
// TODO: Rectangle(int width, int height) — chữ nhật width x height
class Rectangle {
public:
    // TODO: Rectangle() : width_(1), height_(1) {}
    // TODO: Rectangle(int side) : width_(side), height_(side) {}
    // TODO: Rectangle(int width, int height) : width_(width), height_(height) {}
    Rectangle() : width_(1), height_(1) {}
    Rectangle(int side) : width_(side), height_(side) {}
    Rectangle(int width, int height) : width_(width), height_(height) {}
    int area() const { return width_ * height_; }

private:
    int width_;
    int height_;
};

// ===== Bài 2 (trung bình): LogEntry — Delegating Constructor =====
// LogEntry lưu message và level (mặc định "INFO" nếu không truyền).
// Thay vì lặp code khởi tạo ở 2 constructor, constructor 1 tham số nên
// "delegate" sang constructor 2 tham số bằng cú pháp:
//   LogEntry(const std::string& msg) : LogEntry(msg, "INFO") {}
// TODO: viết delegating constructor như trên.
#include <string>
class LogEntry {
public:
    LogEntry(const std::string& msg, const std::string& level)
        : msg_(msg), level_(level) {}

    // TODO: LogEntry(const std::string& msg) : LogEntry(msg, "INFO") {}
    LogEntry(const std::string& msg) : LogEntry(msg, "INFO") {}
    void print() const {
        std::cout << "[" << level_ << "] " << msg_;
    }

private:
    std::string msg_;
    std::string level_;
};

int main() {
    std::cout << "[Bai 1] Rectangle - 3 constructor\n";
    Rectangle r1;
    Rectangle r2(4);
    Rectangle r3(3, 5);
    std::cout << "r1.area() = " << r1.area() << " (ky vong 1)\n";
    std::cout << "r2.area() = " << r2.area() << " (ky vong 16)\n";
    std::cout << "r3.area() = " << r3.area() << " (ky vong 15)\n";

    std::cout << "\n[Bai 2] LogEntry - delegating constructor\n";
    LogEntry e1("Disk full", "ERROR");
    LogEntry e2("Boot xong");   // dung delegating constructor -> level = INFO
    e1.print(); std::cout << " (ky vong [ERROR] Disk full)\n";
    e2.print(); std::cout << " (ky vong [INFO] Boot xong)\n";

    return 0;
}

/* ===== LỜI GIẢI =====

#include <iostream>
#include <string>

class Rectangle {
public:
    Rectangle() : width_(1), height_(1) {}
    Rectangle(int side) : width_(side), height_(side) {}
    Rectangle(int width, int height) : width_(width), height_(height) {}

    int area() const { return width_ * height_; }

private:
    int width_;
    int height_;
};

class LogEntry {
public:
    LogEntry(const std::string& msg, const std::string& level)
        : msg_(msg), level_(level) {}

    LogEntry(const std::string& msg) : LogEntry(msg, "INFO") {}

    void print() const {
        std::cout << "[" << level_ << "] " << msg_;
    }

private:
    std::string msg_;
    std::string level_;
};

int main() {
    std::cout << "[Bai 1] Rectangle - 3 constructor\n";
    Rectangle r1;
    Rectangle r2(4);
    Rectangle r3(3, 5);
    std::cout << "r1.area() = " << r1.area() << "\n";
    std::cout << "r2.area() = " << r2.area() << "\n";
    std::cout << "r3.area() = " << r3.area() << "\n";

    std::cout << "\n[Bai 2] LogEntry - delegating constructor\n";
    LogEntry e1("Disk full", "ERROR");
    LogEntry e2("Boot xong");
    e1.print(); std::cout << "\n";
    e2.print(); std::cout << "\n";

    return 0;
}

===================== */
