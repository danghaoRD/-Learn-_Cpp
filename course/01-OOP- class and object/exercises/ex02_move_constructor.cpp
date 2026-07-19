// Bài tập: Move Constructor
// Dựa trên note: ../02_Move_constructor.md
//
// Nhắc lại: copy constructor deep-copy (bài 01) an toàn nhưng tốn chi phí —
// cấp phát vùng nhớ MỚI và sao chép từng byte, dù object nguồn sắp bị hủy
// ngay sau đó (ví dụ: return theo giá trị, đẩy vào vector khi resize...).
// Move constructor giải quyết vấn đề này bằng cách "cướp" con trỏ của object
// nguồn (rvalue) thay vì sao chép, rồi đặt con trỏ nguồn về nullptr để
// destructor của nó không free vùng nhớ đã bị cướp -> tránh double-free.
//
// Bên C, điều tương đương là tự tay "chuyển quyền sở hữu" con trỏ:
//   int* tmp = src->data; src->data = NULL; dst->data = tmp;
// C++ chỉ hóa cú pháp này bằng MyString(MyString&& other) và std::move().

#include <cstring>
#include <iostream>
#include <utility>  // std::move

// ===== Bài 1 (dễ): Move constructor cho IntArray =====
// IntArray sở hữu một mảng int cấp phát động. Đã có constructor thường,
// destructor, và copy constructor (deep copy). TODO: viết thêm move
// constructor — KHÔNG cấp phát vùng nhớ mới, chỉ "cướp" con trỏ của other
// rồi đặt other.data = nullptr, other.len = 0.
class IntArray {
public:
    IntArray(size_t n) : len(n) {
        data = new int[len];
        for (size_t i = 0; i < len; ++i) data[i] = 0;
    }

    IntArray(const IntArray& other) : len(other.len) {
        data = new int[len];
        std::memcpy(data, other.data, len * sizeof(int));
        std::cout << "  [copy ctor IntArray]\n";
    }

    // TODO: move constructor
    // IntArray(IntArray&& other) noexcept { ... }
    IntArray(IntArray&& other) noexcept
        :   data(other.data),  
            len(other.len)
    {
        other.data = nullptr;
        other.len = 0;
    }
    ~IntArray() { delete[] data; }

    void set(size_t i, int v) { data[i] = v; }
    int get(size_t i) const { return data[i]; }
    size_t size() const { return len; }

private:
    int* data;
    size_t len;
};

// ===== Bài 2 (trung bình): Rule of Five cho MyString =====
// MyString sở hữu char* data. Đã có constructor từ const char*, destructor,
// copy constructor, copy assignment (Rule of Three từ bài 01).
// TODO: bổ sung 2 hàm còn lại của Rule of Five:
//   4. Move constructor (MyString&& other) noexcept
//   5. Move assignment operator (operator=(MyString&& other))
// Lưu ý move assignment cũng phải xử lý self-assignment (s = std::move(s);)
// và giải phóng vùng nhớ cũ của *this trước khi cướp vùng nhớ của other.
class MyString {
public:
    MyString(const char* text) {
        len = std::strlen(text) + 1;
        data = new char[len];
        std::memcpy(data, text, len);
    }

    ~MyString() { delete[] data; }

    MyString(const MyString& other) : len(other.len) {
        data = new char[len];
        std::memcpy(data, other.data, len);
        std::cout << "  [copy ctor MyString]\n";
    }

    MyString& operator=(const MyString& other) {
        if (this == &other) return *this;
        delete[] data;
        len = other.len;
        data = new char[len];
        std::memcpy(data, other.data, len);
        std::cout << "  [copy assign MyString]\n";
        return *this;
    }

    // TODO: MyString(MyString&& other) noexcept { ... }
    // TODO: MyString& operator=(MyString&& other) noexcept { ... }

    MyString(MyString&& other) noexcept
        : data(other.data), len(other.len)
    {
        other.data = nullptr;
        other.len = 0;
    }

    MyString& operator=(MyString&& other) noexcept
    {
        if (this == &other) return *this;   // self-move guard
        delete[] data;                     // giải phóng vùng nhớ cũ của *this
        data = other.data;                 // cướp con trỏ  
        len = other.len;
        other.data = nullptr;              // nguồn không được free nữa
        other.len = 0;
        return *this;
    }
    const char* c_str() const { return data ? data : "(moved-from)"; }

private:
    char* data = nullptr;
    size_t len = 0;
};

int main() {
    // --- Test bài 1 ---
    IntArray a(3);
    a.set(0, 10); a.set(1, 20); a.set(2, 30);
    std::cout << "Bai 1: \n";
    std::cout << "b = std::move(a):\n";
    IntArray b = std::move(a);   // phải gọi move constructor, KHÔNG copy
    b.set(0, 999);
    std::cout << "b[0] = " << b.get(0) << " (kỳ vọng 999)\n";
    // a.data giờ phải là nullptr -> a.size() vẫn là 3 nhưng gọi a.get(0)
    // sẽ đọc null pointer, nên không test a ở đây nữa (a coi như "rỗng").

    // --- Test bài 2 ---
    std::cout << "Bai 2 \n";
    MyString s1("hello");
    std::cout << "s2 = std::move(s1):\n";
    MyString s2 = std::move(s1);         // move constructor
    std::cout << "s1 (moved-from) = " << s1.c_str() << "\n";
    std::cout << "s2 = " << s2.c_str() << "\n";

    MyString s3("temp");
    std::cout << "s3 = std::move(s2):\n";
    s3 = std::move(s2);                  // move assignment
    std::cout << "s3 = " << s3.c_str() << "\n";

    std::cout << "self-move s3 = std::move(s3):\n";
    s3 = std::move(s3);   // self-move, không được crash
    std::cout << "s3 sau self-move = " << s3.c_str() << "\n";

    return 0;
    // Khi a,b,s1,s2,s3 ra khỏi scope: nếu move constructor/operator= không
    // đặt other.data = nullptr, hai object sẽ cùng trỏ 1 vùng nhớ ->
    // double-free / heap corruption ở đây.
}

/* ===== LỜI GIẢI =====
   Bỏ comment khối này (và comment lại phần TODO tương ứng ở trên) để đối chiếu.

#include <cstring>
#include <iostream>
#include <utility>

class IntArray {
public:
    IntArray(size_t n) : len(n) {
        data = new int[len];
        for (size_t i = 0; i < len; ++i) data[i] = 0;
    }

    IntArray(const IntArray& other) : len(other.len) {
        data = new int[len];
        std::memcpy(data, other.data, len * sizeof(int));
    }

    IntArray(IntArray&& other) noexcept : data(other.data), len(other.len) {
        other.data = nullptr;
        other.len = 0;
    }

    ~IntArray() { delete[] data; }

    void set(size_t i, int v) { data[i] = v; }
    int get(size_t i) const { return data[i]; }
    size_t size() const { return len; }

private:
    int* data;
    size_t len;
};

class MyString {
public:
    MyString(const char* text) {
        len = std::strlen(text) + 1;
        data = new char[len];
        std::memcpy(data, text, len);
    }

    ~MyString() { delete[] data; }

    MyString(const MyString& other) : len(other.len) {
        data = new char[len];
        std::memcpy(data, other.data, len);
    }

    MyString& operator=(const MyString& other) {
        if (this == &other) return *this;
        delete[] data;
        len = other.len;
        data = new char[len];
        std::memcpy(data, other.data, len);
        return *this;
    }

    MyString(MyString&& other) noexcept : data(other.data), len(other.len) {
        other.data = nullptr;
        other.len = 0;
    }

    MyString& operator=(MyString&& other) noexcept {
        if (this == &other) return *this;   // self-move guard
        delete[] data;                       // giải phóng vùng nhớ cũ của *this
        data = other.data;                   // cướp con trỏ
        len = other.len;
        other.data = nullptr;                // nguồn không được free nữa
        other.len = 0;
        return *this;
    }

    const char* c_str() const { return data ? data : "(moved-from)"; }

private:
    char* data = nullptr;
    size_t len = 0;
};

int main() {
    IntArray a(3);
    a.set(0, 10); a.set(1, 20); a.set(2, 30);
    IntArray b = std::move(a);
    b.set(0, 999);
    std::cout << "b[0] = " << b.get(0) << "\n";

    MyString s1("hello");
    MyString s2 = std::move(s1);
    std::cout << "s1 = " << s1.c_str() << "\n";
    std::cout << "s2 = " << s2.c_str() << "\n";

    MyString s3("temp");
    s3 = std::move(s2);
    std::cout << "s3 = " << s3.c_str() << "\n";

    s3 = std::move(s3);
    std::cout << "s3 sau self-move = " << s3.c_str() << "\n";

    return 0;
}

===================== */
