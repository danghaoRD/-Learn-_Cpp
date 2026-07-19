// Bài tập: Friend of Class
// Dựa trên note: ../06_Friend_of_Class.md
//
// Nhắc lại: friend là ngoại lệ tường minh cho phép 1 hàm/class bên ngoài
// truy cập private/protected — không đối xứng, không kế thừa, không bắc cầu.
// Không tốn chi phí runtime (chỉ là whitelist lúc compile-time).

#include <iostream>

// ===== Bài 1 (dễ): Friend function cho operator<< =====
// Vector2D có x, y private. Muốn in ra std::cout bằng operator<< nhưng
// operator<< phải là hàm tự do (vì tham số đầu là std::ostream&, không phải
// Vector2D) -> cần friend để đọc được x, y private.
class Vector2D {
public:
    Vector2D(double x, double y) : x_(x), y_(y) {}

    // TODO: khai báo friend cho operator<<
    // friend std::ostream& operator<<(std::ostream& os, const Vector2D& v);

private:
    double x_, y_;
};

// TODO: định nghĩa operator<< bên ngoài class, truy cập trực tiếp v.x_, v.y_
// std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
//     os << "(" << v.x_ << ", " << v.y_ << ")";
//     return os;
// }

// ===== Bài 2 (trung bình): Friend class — Engine/Car/CarFactory =====
// Car sở hữu con trỏ Engine* private. Chỉ CarFactory được phép gán engine
// trực tiếp (mọi code khác phải qua constructor hợp lệ, không có setter công khai).
class Engine {
public:
    Engine(int hp) : horsepower(hp) {}
    int horsepower;
};

class Car {
public:
    void drive() const {
        if (engine_) std::cout << "Driving with " << engine_->horsepower << " HP\n";
        else std::cout << "No engine installed\n";
    }

    // TODO: khai báo friend class CarFactory;

private:
    Engine* engine_ = nullptr;
};

class CarFactory {
public:
    void installEngine(Car& c, Engine* e) {
        // TODO: gán trực tiếp c.engine_ = e; (chỉ hợp lệ sau khi Car khai bao
        // friend class CarFactory)
    }
};

int main() {
    std::cout << "[Bai 1] Friend function cho operator<<\n";
    Vector2D v(3.0, 4.0);
    // std::cout << v << "\n"; // TODO: bo comment sau khi lam xong Bai 1

    std::cout << "\n[Bai 2] Friend class - Engine/Car/CarFactory\n";
    Car car;
    Engine engine(150);
    CarFactory factory;
    factory.installEngine(car, &engine);
    car.drive(); // ky vong: "Driving with 150 HP"

    return 0;
}

/* ===== LỜI GIẢI =====

#include <iostream>

class Vector2D {
public:
    Vector2D(double x, double y) : x_(x), y_(y) {}

    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v);

private:
    double x_, y_;
};

std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
    os << "(" << v.x_ << ", " << v.y_ << ")";
    return os;
}

class Engine {
public:
    Engine(int hp) : horsepower(hp) {}
    int horsepower;
};

class Car {
public:
    void drive() const {
        if (engine_) std::cout << "Driving with " << engine_->horsepower << " HP\n";
        else std::cout << "No engine installed\n";
    }

    friend class CarFactory;

private:
    Engine* engine_ = nullptr;
};

class CarFactory {
public:
    void installEngine(Car& c, Engine* e) {
        c.engine_ = e;
    }
};

int main() {
    std::cout << "[Bai 1] Friend function cho operator<<\n";
    Vector2D v(3.0, 4.0);
    std::cout << v << "\n";

    std::cout << "\n[Bai 2] Friend class - Engine/Car/CarFactory\n";
    Car car;
    Engine engine(150);
    CarFactory factory;
    factory.installEngine(car, &engine);
    car.drive();

    return 0;
}

===================== */
