// Bài tập: Copy Constructor
// Dựa trên note: ../01_Copy_Constructor.md
//
// Nhắc lại: copy constructor mặc định (compiler-generated) là SHALLOW COPY —
// chỉ copy con trỏ, không copy vùng nhớ được trỏ tới. Class nào sở hữu tài
// nguyên động (new[]/malloc) mà không tự viết copy constructor sẽ dính
// double-free khi 2 object cùng trỏ 1 vùng nhớ và destructor chạy 2 lần.

#include <cstring>
#include <iostream>

// ===== Bài 1 (dễ): Deep copy cho IntArray =====
// IntArray sở hữu một mảng int cấp phát động. Constructor thường và
// destructor đã viết sẵn. TODO: viết copy constructor deep-copy (cấp phát
// vùng nhớ MỚI cho bản copy, không dùng chung con trỏ với bản gốc).
class IntArray {
public:
    IntArray(size_t n) : len(n) {
        data = new int[len];
        for (size_t i = 0; i < len; ++i) data[i] = 0;
    }

    // TODO: copy constructor — deep copy
    // IntArray(const IntArray& other) { ... }

    ~IntArray() { delete[] data; }

    void set(size_t i, int v) { data[i] = v; }
    int get(size_t i) const { return data[i]; }
    size_t size() const { return len; }

private:
    int* data;
    size_t len;
};

// ===== Bài 2 (trung bình): Rule of Three cho MyString =====
// MyString sở hữu char* data. Đã có constructor từ const char*.
// TODO: viết đủ 3 hàm theo Rule of Three:
//   1. Destructor
//   2. Copy constructor (deep copy)
//   3. Copy assignment operator (operator=) — nhớ xử lý self-assignment
//      (a = a;) và giải phóng vùng nhớ cũ trước khi cấp phát vùng nhớ mới.
class MyString {
public:
    MyString(const char* text) {
        len = std::strlen(text) + 1;
        data = new char[len];
        std::memcpy(data, text, len);
    }

    // TODO: ~MyString() { ... }
    // TODO: MyString(const MyString& other) { ... }
    // TODO: MyString& operator=(const MyString& other) { ... }

    const char* c_str() const { return data; }

private:
    char* data;
    size_t len;
};

int main() {
    // --- Test bài 1 ---
    IntArray a(3);
    a.set(0, 10); a.set(1, 20); a.set(2, 30);
    IntArray b = a;      // phải gọi copy constructor
    b.set(0, 999);        // sửa b không được ảnh hưởng a

    std::cout << "a[0] = " << a.get(0) << " (ky vong 10)\n";
    std::cout << "b[0] = " << b.get(0) << " (ky vong 999)\n";

    // --- Test bài 2 ---
    MyString s1("hello");
    MyString s2 = s1;     // copy constructor
    MyString s3("temp");
    s3 = s1;              // copy assignment operator
    s3 = s3;              // self-assignment, không được crash

    std::cout << "s1 = " << s1.c_str() << "\n";
    std::cout << "s2 = " << s2.c_str() << "\n";
    std::cout << "s3 = " << s3.c_str() << "\n";

    return 0;
    // Khi a,b,s1,s2,s3 ra khoi scope: neu copy constructor/operator= sai
    // (shallow copy), chuong trinh se crash (double free) hoac heap corrupt
    // o day.
}

/* ===== LỜI GIẢI =====
   Bỏ comment khối này (và comment lại phần TODO tương ứng ở trên) để đối chiếu.

#include <cstring>
#include <iostream>

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
        if (this == &other) return *this;   // self-assignment guard
        delete[] data;                       // giai phong vung nho cu
        len = other.len;
        data = new char[len];
        std::memcpy(data, other.data, len);
        return *this;
    }

    const char* c_str() const { return data; }

private:
    char* data;
    size_t len;
};

int main() {
    IntArray a(3);
    a.set(0, 10); a.set(1, 20); a.set(2, 30);
    IntArray b = a;
    b.set(0, 999);

    std::cout << "a[0] = " << a.get(0) << "\n";
    std::cout << "b[0] = " << b.get(0) << "\n";

    MyString s1("hello");
    MyString s2 = s1;
    MyString s3("temp");
    s3 = s1;
    s3 = s3;

    std::cout << "s1 = " << s1.c_str() << "\n";
    std::cout << "s2 = " << s2.c_str() << "\n";
    std::cout << "s3 = " << s3.c_str() << "\n";

    return 0;
}

===================== */
