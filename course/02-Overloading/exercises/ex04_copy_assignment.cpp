// Bài tập: Copy Assignment Operator
// Dựa trên note: ../04_Copy Assignment Operator.md
//
// Nhắc lại: Copy Assignment (operator=) chỉ được gọi khi object ĐÍCH đã tồn
// tại (khác Copy Constructor — gọi khi tạo object mới). Phải: (1) kiểm tra
// self-assignment (this == &other), (2) giải phóng tài nguyên cũ trước khi
// cấp phát mới, (3) trả về ClassName& để hỗ trợ gán liên tiếp (a = b = c).

#include <cstring>
#include <iostream>

// ===== Bài 1 (dễ): IntBuffer — operator= deep copy =====
// TODO: operator= phải: kiểm tra self-assignment, giải phóng data_ cũ,
// cấp phát vùng nhớ mới đúng kích thước other, memcpy nội dung, trả về *this.
class IntBuffer {
public:
    IntBuffer(size_t n) : len_(n) {
        data_ = new int[len_];
        for (size_t i = 0; i < len_; ++i) data_[i] = 0;
    }

    ~IntBuffer() { delete[] data_; }

    IntBuffer(const IntBuffer& other) : len_(other.len_) {
        data_ = new int[len_];
        std::memcpy(data_, other.data_, len_ * sizeof(int));
    }

    // TODO: IntBuffer& operator=(const IntBuffer& other) { ... }
    IntBuffer& operator=(const IntBuffer& other) {
        if (this == &other) return *this; // self-assignment check
        delete[] data_; // free old memory
        len_ = other.len_; // copy length
        data_ = new int[len_]; // allocate new memory
        std::memcpy(data_, other.data_, len_ * sizeof(int)); // copy data
        return *this; // return *this for chaining
    }

    void set(size_t i, int v) { data_[i] = v; }
    int get(size_t i) const { return data_[i]; }
    size_t size() const { return len_; }

private:
    int* data_;
    size_t len_;
};

int main() {
    std::cout << "[Bai 1] IntBuffer operator= - deep copy + self-assignment\n";
    IntBuffer a(3);
    a.set(0, 1); a.set(1, 2); a.set(2, 3);

    IntBuffer b(5);
    b = a;                 // copy assignment (b da ton tai truoc do)
    b.set(0, 999);          // sua b khong duoc anh huong a

    std::cout << "a[0] = " << a.get(0) << " (ky vong 1)\n";
    std::cout << "b[0] = " << b.get(0) << " (ky vong 999)\n";
    std::cout << "b.size() = " << b.size() << " (ky vong 3, khong con la 5)\n";

    a = a;   // self-assignment, khong duoc crash / khong duoc lam mat du lieu
    std::cout << "a[0] sau a = a: " << a.get(0) << " (ky vong van la 1)\n";

    return 0;
}

/* ===== LỜI GIẢI =====

#include <cstring>
#include <iostream>

class IntBuffer {
public:
    IntBuffer(size_t n) : len_(n) {
        data_ = new int[len_];
        for (size_t i = 0; i < len_; ++i) data_[i] = 0;
    }

    ~IntBuffer() { delete[] data_; }

    IntBuffer(const IntBuffer& other) : len_(other.len_) {
        data_ = new int[len_];
        std::memcpy(data_, other.data_, len_ * sizeof(int));
    }

    IntBuffer& operator=(const IntBuffer& other) {
        if (this == &other) return *this;
        delete[] data_;
        len_ = other.len_;
        data_ = new int[len_];
        std::memcpy(data_, other.data_, len_ * sizeof(int));
        return *this;
    }

    void set(size_t i, int v) { data_[i] = v; }
    int get(size_t i) const { return data_[i]; }
    size_t size() const { return len_; }

private:
    int* data_;
    size_t len_;
};

int main() {
    std::cout << "[Bai 1] IntBuffer operator= - deep copy + self-assignment\n";
    IntBuffer a(3);
    a.set(0, 1); a.set(1, 2); a.set(2, 3);

    IntBuffer b(5);
    b = a;
    b.set(0, 999);

    std::cout << "a[0] = " << a.get(0) << "\n";
    std::cout << "b[0] = " << b.get(0) << "\n";
    std::cout << "b.size() = " << b.size() << "\n";

    a = a;
    std::cout << "a[0] sau a = a: " << a.get(0) << "\n";

    return 0;
}

===================== */
