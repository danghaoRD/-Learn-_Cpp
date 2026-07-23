// Bài tập: Function Overloading
// Dựa trên note: ../03_Overload_function.md
//
// Nhắc lại: overload phân biệt bằng Function Signature (số lượng + kiểu +
// thứ tự tham số), KHÔNG phân biệt bằng kiểu trả về. Compiler luôn ưu tiên
// phiên bản khớp mà không cần convert kiểu (exact match).
//
// Lưu ý: main() ở dưới đã gọi sẵn các hàm cần viết -> file này CHƯA compile
// được cho tới khi bạn viết xong thân các hàm TODO. Đây là chủ đích.

#include <iostream>

// ===== Bài 1 (dễ): describe — overload theo kiểu tham số =====
// TODO: describe(int) in "so nguyen: <value>\n"
void describe(int value);
// TODO: describe(double) in "so thuc: <value>\n"
void describe(double value);
// TODO: describe(const char*) in "chuoi: <value>\n"
void describe(const char* text);

// ===== Bài 2 (trung bình): area — overload theo số lượng tham số =====
// TODO: area(int side) — dien tich hinh vuong canh "side"
int area(int side);
// TODO: area(int width, int height) — dien tich hinh chu nhat
int area(int width, int height);

int main() {
    std::cout << "[Bai 1] describe - overload theo kieu tham so\n";
    describe(10);
    describe(3.14);
    describe("Hello");

    std::cout << "\n[Bai 2] area - overload theo so luong tham so\n";
    std::cout << "area(4) = " << area(4) << " (ky vong 16)\n";
    std::cout << "area(3, 5) = " << area(3, 5) << " (ky vong 15)\n";

    return 0;
}

// TODO: viết thân các hàm khai báo ở trên, tại đây, phía dưới main()
// hoặc phía trên main() đều được (miễn khớp signature đã khai báo).

/* ===== LỜI GIẢI =====

#include <iostream>

void describe(int value) {
    std::cout << "so nguyen: " << value << "\n";
}

void describe(double value) {
    std::cout << "so thuc: " << value << "\n";
}

void describe(const char* text) {
    std::cout << "chuoi: " << text << "\n";
}

int area(int side) {
    return side * side;
}

int area(int width, int height) {
    return width * height;
}

int main() {
    std::cout << "[Bai 1] describe - overload theo kieu tham so\n";
    describe(10);
    describe(3.14);
    describe("Hello");

    std::cout << "\n[Bai 2] area - overload theo so luong tham so\n";
    std::cout << "area(4) = " << area(4) << "\n";
    std::cout << "area(3, 5) = " << area(3, 5) << "\n";

    return 0;
}

===================== */
