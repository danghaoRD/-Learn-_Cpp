// Bài tập: Static Class Members
// Dựa trên note: ../04_Static_Class_members.md
//
// Nhắc lại: static data member = 1 bản dùng chung cho cả class (không nằm
// trong từng object). Khai báo trong class KHÁC định nghĩa (phải định nghĩa
// ngoài class, trừ khi dùng inline static từ C++17). Static method không có
// this -> không đụng được non-static member.

#include <iostream>

// ===== Bài 1 (dễ): Đếm số object đã tạo =====
// TODO: khai báo static int count trong class (chỉ khai báo).
// TODO: constructor tăng count lên 1.
// TODO: static method getCount() trả về count.
// TODO: định nghĩa "int Device::count = 0;" ở ngoài class, TRƯỚC main()
//       (nếu quên bước này chương trình sẽ lỗi linker "undefined reference").
class Device {
public:
    Device() { /* TODO: count++; */ }
    // TODO: static int getCount() { return count; }

private:
    // TODO: static int count;
};

// TODO: định nghĩa static member ở đây, ngoài class:
// int Device::count = 0;

// ===== Bài 2 (trung bình): static method thuần túy (không cần static data) =====
// MathUtil chỉ chứa các hàm tiện ích, không có state -> không cần tạo object.
// TODO: viết 2 static method: square(int) và isEven(int), không dùng member
// nào cả (static method không bắt buộc phải đụng static data).
class MathUtil {
public:
    // TODO: static int square(int x) { ... }
    // TODO: static bool isEven(int x) { ... }
};

// ===== Bài 3 (khó hơn): Local static object — lazy init =====
// TODO: viết hàm getLogPrefix() trả về const std::string& , bên trong dùng
// một "static const std::string prefix = ...;" khai báo cục bộ trong hàm
// (local static) — chỉ khởi tạo đúng 1 lần ở lần gọi đầu tiên, các lần gọi
// sau dùng lại bản đã có, không chạy lại constructor của std::string.
#include <string>
// TODO: const std::string& getLogPrefix() {
//     static const std::string prefix = "[LOG] ";
//     std::cout << "  (getLogPrefix duoc goi)\n";
//     return prefix;
// }

int main() {
    Device a, b, c;
    std::cout << "Total devices: " << Device::getCount() << " (ky vong 3)\n";

    std::cout << "square(5) = " << MathUtil::square(5) << " (ky vong 25)\n";
    std::cout << "isEven(4) = " << MathUtil::isEven(4) << " (ky vong 1)\n";

    std::cout << getLogPrefix() << "lan 1\n";
    std::cout << getLogPrefix() << "lan 2\n"; // "(getLogPrefix duoc goi)" chi in 1 lan duy nhat khi chay -O0?
    // Luu y: dong "(getLogPrefix duoc goi)" van in moi lan ham chay, nhung
    // dong khoi tao string chi chay 1 lan (kho quan sat truc tiep bang cout,
    // day la diem ban co the thao luan them / dat breakpoint de kiem chung).

    return 0;
}

/* ===== LỜI GIẢI =====

#include <iostream>
#include <string>

class Device {
public:
    Device() { count++; }
    static int getCount() { return count; }

private:
    static int count;
};

int Device::count = 0;

class MathUtil {
public:
    static int square(int x) { return x * x; }
    static bool isEven(int x) { return x % 2 == 0; }
};

const std::string& getLogPrefix() {
    static const std::string prefix = "[LOG] ";
    std::cout << "  (getLogPrefix duoc goi)\n";
    return prefix;
}

int main() {
    Device a, b, c;
    std::cout << "Total devices: " << Device::getCount() << "\n";

    std::cout << "square(5) = " << MathUtil::square(5) << "\n";
    std::cout << "isEven(4) = " << MathUtil::isEven(4) << "\n";

    std::cout << getLogPrefix() << "lan 1\n";
    std::cout << getLogPrefix() << "lan 2\n";

    return 0;
}

===================== */
