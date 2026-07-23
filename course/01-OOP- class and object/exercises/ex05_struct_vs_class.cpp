// Bài tập: Struct và Class
// Dựa trên note: ../05_Struct_and_Class.md
//
// Nhắc lại: struct/class trong C++ giống nhau về khả năng (data, method,
// constructor, inheritance...). Khác biệt DUY NHẤT là access specifier mặc
// định: struct = public, class = private.

#include <iostream>
#include <string>

// ===== Bài 1 (dễ): Chuyển POD struct thành class có encapsulation =====
// RawPoint là struct kiểu C thuần (chỉ dữ liệu, ai cũng sửa trực tiếp được).
struct RawPoint {
    int x;
    int y;
};

// TODO: viết class Point2D tương đương nhưng đóng gói: x,y là private,
// có constructor Point2D(int x, int y), getX()/getY() const, và setX()/setY()
// có kiểm tra đơn giản (ví dụ không cho phép âm -> clamp về 0). Đây là điểm
// class hơn struct: có thể ràng buộc invariant ngay trong setter, struct
// public thuần không làm được (ai cũng gán trực tiếp, bỏ qua ràng buộc).
class Point2D {
    public:
    Point2D(int x , int y) : x(x), y(y) {}
    int getX() const { return x; }
    int getY() const { return y; }

    void setX(int setX) { x = (setX < 0) ? 0 : setX; }
    void setY(int setY) { y = (setY < 0) ? 0 : setY; }
    
    private:
        int x;
        int y;
    // TODO
};

// ===== Bài 2 (trung bình): Quan sát default access =====
// TODO: khai báo 1 struct tên `Config` với 2 member: bool verbose, int level.
//       Không viết access specifier nào cả -> mặc định public.
// TODO: khai báo 1 class tên `Secret` với 2 member giống hệt: bool verbose,
//       int level. Không viết access specifier nào cả -> mặc định private.
// Sau khi viết xong, thử truy cập secretObj.verbose trực tiếp trong main() —
// quan sát lỗi biên dịch, rồi comment dòng đó lại để build tiếp.

struct Config {
    bool verbose;
    int level;
};
class Secret
{
private:
    bool verbose;
    int level;
public:
};



int main() {
    std::cout << "[Bai 1] Chuyen POD struct thanh class co encapsulation\n";
    RawPoint rp{1, 2};
    rp.x = -5; // struct C-style: không ai ngăn được giá trị vô lý

    Point2D p(3, 4);
    p.setX(-10); // TODO: sau khi cài clamp, x phải thành 0 chứ không phải -10
    std::cout << "p.x = " << p.getX() << " (ky vong 0 neu co clamp am)\n";

    std::cout << "\n[Bai 2] Quan sat default access\n";
     Config cfg{true, 2};
     std::cout << cfg.verbose << "\n"; // OK vi struct mac dinh public

     //Secret s{true, 2};       // se loi neu dung aggregate-init voi private member
    // std::cout << s.verbose << "\n"; // TODO: bo comment de thay loi bien dich,
                                        // roi comment lai truoc khi build ban cuoi.

    return 0;
}

/* ===== LỜI GIẢI =====

#include <iostream>
#include <string>

struct RawPoint {
    int x;
    int y;
};

class Point2D {
public:
    Point2D(int x, int y) : x_(x), y_(y) {}

    int getX() const { return x_; }
    int getY() const { return y_; }

    void setX(int v) { x_ = (v < 0) ? 0 : v; }
    void setY(int v) { y_ = (v < 0) ? 0 : v; }

private:
    int x_, y_;
};

struct Config {
    bool verbose;
    int level;
};

class Secret {
    bool verbose;
    int level;
};

int main() {
    std::cout << "[Bai 1] Chuyen POD struct thanh class co encapsulation\n";
    RawPoint rp{1, 2};
    rp.x = -5;

    Point2D p(3, 4);
    p.setX(-10);
    std::cout << "p.x = " << p.getX() << "\n";

    std::cout << "\n[Bai 2] Quan sat default access\n";
    Config cfg{true, 2};
    std::cout << cfg.verbose << "\n"; // OK: struct mac dinh public

    // Secret s{true, 2};
    // std::cout << s.verbose << "\n"; // Loi bien dich: verbose la private

    return 0;
}

===================== */
