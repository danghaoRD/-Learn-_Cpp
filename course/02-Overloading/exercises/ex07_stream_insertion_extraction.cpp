// Bài tập: Stream Insertion / Extraction Operator Overloading
// Dựa trên note: ../07_Insertion_Extraction_Overloading.md
//
// Nhắc lại: operator<< và operator>> gần như luôn là global function (vì
// toán hạng trái là std::ostream/std::istream, không sửa được thư viện
// chuẩn để thêm member). Phải trả về ostream&/istream& để hỗ trợ chain
// (cout << a << b). Bên trong operator<< chỉ dùng "os <<", không dùng
// "std::cout <<" trực tiếp — để operator hoạt động với mọi loại stream.

#include <iostream>
#include <sstream>

// ===== Bài 1: Point — operator<< và operator>> dạng friend global =====
// TODO: khai báo friend cho operator<< và operator>> bên trong class.
// TODO: operator<<(ostream&, const Point&) in dạng "(x, y)", dùng "os <<",
// trả về os.
// TODO: operator>>(istream&, Point&) đọc x rồi y bằng "is >>", trả về is.
class Point {
public:
    Point(int x = 0, int y = 0) : x_(x), y_(y) {}

    // TODO: friend std::ostream& operator<<(std::ostream& os, const Point& p);
    // TODO: friend std::istream& operator>>(std::istream& is, Point& p);

private:
    int x_, y_;
};

// TODO: std::ostream& operator<<(std::ostream& os, const Point& p) { ... }
// TODO: std::istream& operator>>(std::istream& is, Point& p) { ... }

int main() {
    std::cout << "[Bai 1] Point - operator<< / operator>>\n";

    Point p1(10, 20);
    std::cout << "p1 = " << p1 << " (ky vong (10, 20))\n";

    Point p2(1, 1), p3(2, 2);
    std::cout << "chain: " << p2 << " " << p3 << " (ky vong (1, 1) (2, 2))\n";

    // Doc tu chuoi gia lap bang istringstream thay vi cin, de test khong
    // can nhap tay khi chay.
    std::istringstream iss("99 88");
    Point p4;
    iss >> p4;
    std::cout << "p4 sau khi doc \"99 88\" = " << p4 << " (ky vong (99, 88))\n";

    return 0;
}

/* ===== LỜI GIẢI =====

#include <iostream>
#include <sstream>

class Point {
public:
    Point(int x = 0, int y = 0) : x_(x), y_(y) {}

    friend std::ostream& operator<<(std::ostream& os, const Point& p);
    friend std::istream& operator>>(std::istream& is, Point& p);

private:
    int x_, y_;
};

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x_ << ", " << p.y_ << ")";
    return os;
}

std::istream& operator>>(std::istream& is, Point& p) {
    is >> p.x_ >> p.y_;
    return is;
}

int main() {
    std::cout << "[Bai 1] Point - operator<< / operator>>\n";

    Point p1(10, 20);
    std::cout << "p1 = " << p1 << "\n";

    Point p2(1, 1), p3(2, 2);
    std::cout << "chain: " << p2 << " " << p3 << "\n";

    std::istringstream iss("99 88");
    Point p4;
    iss >> p4;
    std::cout << "p4 sau khi doc \"99 88\" = " << p4 << "\n";

    return 0;
}

===================== */
