// Bài tập kết hợp — dùng nhiều khái niệm cùng lúc trong 1 class, giống tình
// huống thực tế (không phải bài học nào cũng đứng riêng một mình).
// Tham khảo lại: ../01_Copy_Constructor.md, ../03_Const_with_Class.md,
//                ../04_Static_Class_members.md, ../05_Struct_and_Class.md,
//                ../06_Friend_of_Class.md

#include <cstring>
#include <iostream>

// ===================================================================
// Bài A: SensorBuffer — RAII buffer chứa các giá trị đọc từ cảm biến
// Kết hợp: Rule of Three (deep copy) + const getter + static counter +
//          friend operator<<
// ===================================================================
//
// Bối cảnh: một thiết bị có thể có nhiều SensorBuffer sống cùng lúc (mỗi
// cái ghi log riêng), cần biết tổng số buffer đang tồn tại (static), copy
// một buffer phải deep-copy (không dùng chung vùng nhớ), và cần in nội
// dung ra debug console qua operator<<.
class SensorBuffer {
public:
    SensorBuffer(size_t capacity) : cap_(capacity), len_(0) {
        data_ = new float[cap_];
        // TODO: tăng biến static đếm số SensorBuffer đang sống lên 1
    }

    // TODO: destructor — giải phóng data_, giảm biến static đếm xuống 1

    // TODO: copy constructor — deep copy data_ (KHÔNG dùng chung con trỏ
    // với bản gốc), tăng biến static đếm lên 1 (vì đây cũng là 1 buffer
    // mới đang sống)

    // TODO: copy assignment operator — nhớ tự-gán (self-assignment) và
    // giải phóng vùng nhớ cũ trước khi cấp phát mới. KHÔNG tăng biến đếm
    // (số buffer sống không đổi khi gán, chỉ đổi khi tạo/hủy)

    void push(float v) {
        if (len_ < cap_) data_[len_++] = v;
    }

    // TODO: getAt(size_t i) — trả về data_[i], phải là hàm const (chỉ đọc)
    float getAt(size_t /*i*/) const { return 0; } // TODO: sửa lại cho đúng

    size_t size() const { return len_; }

    // TODO: static method aliveCount() trả về số SensorBuffer đang sống

    // TODO: khai báo friend operator<< để in toàn bộ giá trị trong buffer

private:
    float* data_;
    size_t cap_;
    size_t len_;
    // TODO: static size_t aliveCount_;
};

// TODO: định nghĩa static member ngoài class
// size_t SensorBuffer::aliveCount_ = 0;

// TODO: định nghĩa operator<<, dạng "SensorBuffer[v0, v1, v2]"
// std::ostream& operator<<(std::ostream& os, const SensorBuffer& b) { ... }

// ===================================================================
// Bài B: Device / DeviceFactory — struct config thuần + class có friend
// Kết hợp: struct POD (default public) + friend class + static counter +
//          const getter
// ===================================================================
//
// Bối cảnh: DeviceConfig chỉ là dữ liệu cấu hình thuần (giống C struct),
// không cần đóng gói. Device thì cần đóng gói chặt: id chỉ được gán bởi
// DeviceFactory (không có setter công khai), và cần đếm tổng số Device đã
// tạo qua factory.

// TODO: struct DeviceConfig với 2 field: int baudRate, bool useDma.
// Không cần constructor, không cần access specifier (mặc định public là
// đủ vì đây chỉ là data thuần túy để truyền cấu hình).
struct DeviceConfig {
    // TODO
};

class Device {
public:
    Device(DeviceConfig cfg) : cfg_(cfg), id_(-1) {}

    int getId() const { return id_; }
    int getBaudRate() const { return cfg_.baudRate; }

    // TODO: friend class DeviceFactory;

private:
    DeviceConfig cfg_;
    int id_; // chỉ DeviceFactory được phép gán giá trị thật cho id_
    // TODO: static int nextId_;
};

// TODO: định nghĩa static member ngoài class
// int Device::nextId_ = 0;

class DeviceFactory {
public:
    Device create(DeviceConfig cfg) {
        Device d(cfg);
        // TODO: gán d.id_ = Device::nextId_++; (truy cập trực tiếp private
        // id_ của Device — chỉ hợp lệ sau khi Device khai báo friend class
        // DeviceFactory)
        return d;
    }
};

int main() {
    // --- Test bài A ---
    {
        SensorBuffer a(4);
        a.push(1.0f); a.push(2.0f); a.push(3.0f);

        std::cout << "Alive (ky vong 1): " << SensorBuffer::aliveCount() << "\n";

        SensorBuffer b = a;      // copy constructor -> deep copy
        std::cout << "Alive sau khi copy (ky vong 2): "
                  << SensorBuffer::aliveCount() << "\n";

        std::cout << "a = " << a << "\n";
        std::cout << "b = " << b << "\n";
    } // a, b ra khoi scope -> destructor giam aliveCount ve 0

    std::cout << "Alive cuoi cung (ky vong 0): "
              << SensorBuffer::aliveCount() << "\n";

    // --- Test bài B ---
    DeviceFactory factory;
    Device d1 = factory.create(DeviceConfig{115200, true});
    Device d2 = factory.create(DeviceConfig{9600, false});

    std::cout << "d1.id = " << d1.getId() << " (ky vong 0)\n";
    std::cout << "d2.id = " << d2.getId() << " (ky vong 1)\n";
    std::cout << "d2.baudRate = " << d2.getBaudRate() << " (ky vong 9600)\n";

    return 0;
}

/* ===== LỜI GIẢI =====

#include <cstring>
#include <iostream>

class SensorBuffer {
public:
    SensorBuffer(size_t capacity) : cap_(capacity), len_(0) {
        data_ = new float[cap_];
        aliveCount_++;
    }

    ~SensorBuffer() {
        delete[] data_;
        aliveCount_--;
    }

    SensorBuffer(const SensorBuffer& other)
        : cap_(other.cap_), len_(other.len_) {
        data_ = new float[cap_];
        std::memcpy(data_, other.data_, len_ * sizeof(float));
        aliveCount_++;
    }

    SensorBuffer& operator=(const SensorBuffer& other) {
        if (this == &other) return *this;
        delete[] data_;
        cap_ = other.cap_;
        len_ = other.len_;
        data_ = new float[cap_];
        std::memcpy(data_, other.data_, len_ * sizeof(float));
        return *this;
    }

    void push(float v) {
        if (len_ < cap_) data_[len_++] = v;
    }

    float getAt(size_t i) const { return data_[i]; }

    size_t size() const { return len_; }

    static size_t aliveCount() { return aliveCount_; }

    friend std::ostream& operator<<(std::ostream& os, const SensorBuffer& b);

private:
    float* data_;
    size_t cap_;
    size_t len_;
    static size_t aliveCount_;
};

size_t SensorBuffer::aliveCount_ = 0;

std::ostream& operator<<(std::ostream& os, const SensorBuffer& b) {
    os << "SensorBuffer[";
    for (size_t i = 0; i < b.len_; ++i) {
        os << b.data_[i];
        if (i + 1 < b.len_) os << ", ";
    }
    os << "]";
    return os;
}

struct DeviceConfig {
    int baudRate;
    bool useDma;
};

class Device {
public:
    Device(DeviceConfig cfg) : cfg_(cfg), id_(-1) {}

    int getId() const { return id_; }
    int getBaudRate() const { return cfg_.baudRate; }

    friend class DeviceFactory;

private:
    DeviceConfig cfg_;
    int id_;
    static int nextId_;
};

int Device::nextId_ = 0;

class DeviceFactory {
public:
    Device create(DeviceConfig cfg) {
        Device d(cfg);
        d.id_ = Device::nextId_++;
        return d;
    }
};

int main() {
    {
        SensorBuffer a(4);
        a.push(1.0f); a.push(2.0f); a.push(3.0f);

        std::cout << "Alive: " << SensorBuffer::aliveCount() << "\n";

        SensorBuffer b = a;
        std::cout << "Alive sau khi copy: " << SensorBuffer::aliveCount() << "\n";

        std::cout << "a = " << a << "\n";
        std::cout << "b = " << b << "\n";
    }

    std::cout << "Alive cuoi cung: " << SensorBuffer::aliveCount() << "\n";

    DeviceFactory factory;
    Device d1 = factory.create(DeviceConfig{115200, true});
    Device d2 = factory.create(DeviceConfig{9600, false});

    std::cout << "d1.id = " << d1.getId() << "\n";
    std::cout << "d2.id = " << d2.getId() << "\n";
    std::cout << "d2.baudRate = " << d2.getBaudRate() << "\n";

    return 0;
}

===================== */
