// Bài tập: Move Assignment Operator
// Dựa trên note: ../05_move_Assignment Operator
//
// Nhắc lại: Move Assignment (operator=(T&&)) và Copy Assignment
// (operator=(const T&)) là 2 overload của cùng operator=, compiler chọn
// dựa vào lvalue/rvalue của đối số (giống Function Overloading). Move
// "cướp" tài nguyên của nguồn (steal con trỏ) thay vì cấp phát + copy,
// rồi phải đưa nguồn về trạng thái an toàn (thường là nullptr) để destructor
// của nguồn không delete nhầm vùng nhớ vừa bị cướp.

#include <cstring>
#include <iostream>

// ===== Bài 1: MyString — copy assignment + move assignment song song =====
// TODO: operator=(const MyString&) — copy assignment, deep copy (giống bài
// Copy Assignment ở ex04).
// TODO: operator=(MyString&&) noexcept — move assignment:
//   1. Kiểm tra self-assignment (this == &other)
//   2. delete[] data_ cũ của *this
//   3. "Cướp" con trỏ: data_ = other.data_; len_ = other.len_;
//   4. Đưa other về trạng thái rỗng an toàn: other.data_ = nullptr; other.len_ = 0;
//   5. return *this;
class MyString {
public:
    MyString(const char* text) {
        len_ = std::strlen(text) + 1;
        data_ = new char[len_];
        std::memcpy(data_, text, len_);
    }

    ~MyString() { delete[] data_; }

    // TODO: MyString& operator=(const MyString& other) { ... }

    // TODO: MyString& operator=(MyString&& other) noexcept { ... }

    const char* c_str() const {
        return data_ ? data_ : "";
    }

private:
    char* data_ = nullptr;
    size_t len_ = 0;
};

int main() {
    std::cout << "[Bai 1] MyString - copy assignment vs move assignment\n";

    MyString a("Hello");
    MyString b("World");
    b = a;                          // lvalue -> copy assignment
    std::cout << "b sau copy = " << b.c_str() << " (ky vong Hello)\n";
    std::cout << "a van con   = " << a.c_str() << " (ky vong Hello, khong bi anh huong)\n";

    MyString c("Temp");
    c = MyString("Moved");          // rvalue tam thoi -> move assignment
    std::cout << "c sau move  = " << c.c_str() << " (ky vong Moved)\n";

    MyString d("KeepMe");
    d = std::move(a);                // std::move ep a thanh rvalue -> move assignment
    std::cout << "d sau move tu a = " << d.c_str() << " (ky vong Hello)\n";
    std::cout << "a sau khi bi move (ky vong chuoi rong): \"" << a.c_str() << "\"\n";

    return 0;
}

/* ===== LỜI GIẢI =====

#include <cstring>
#include <iostream>

class MyString {
public:
    MyString(const char* text) {
        len_ = std::strlen(text) + 1;
        data_ = new char[len_];
        std::memcpy(data_, text, len_);
    }

    ~MyString() { delete[] data_; }

    MyString& operator=(const MyString& other) {
        if (this == &other) return *this;
        delete[] data_;
        len_ = other.len_;
        data_ = new char[len_];
        std::memcpy(data_, other.data_, len_);
        return *this;
    }

    MyString& operator=(MyString&& other) noexcept {
        if (this == &other) return *this;
        delete[] data_;
        data_ = other.data_;
        len_ = other.len_;
        other.data_ = nullptr;
        other.len_ = 0;
        return *this;
    }

    const char* c_str() const {
        return data_ ? data_ : "";
    }

private:
    char* data_ = nullptr;
    size_t len_ = 0;
};

int main() {
    std::cout << "[Bai 1] MyString - copy assignment vs move assignment\n";

    MyString a("Hello");
    MyString b("World");
    b = a;
    std::cout << "b sau copy = " << b.c_str() << "\n";
    std::cout << "a van con   = " << a.c_str() << "\n";

    MyString c("Temp");
    c = MyString("Moved");
    std::cout << "c sau move  = " << c.c_str() << "\n";

    MyString d("KeepMe");
    d = std::move(a);
    std::cout << "d sau move tu a = " << d.c_str() << "\n";
    std::cout << "a sau khi bi move: \"" << a.c_str() << "\"\n";

    return 0;
}

===================== */
