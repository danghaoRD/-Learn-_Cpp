# Inheritance là gì?

## 1. Vấn đề
Trong C, khi có nhiều kiểu dữ liệu chia sẻ chung một tập thuộc tính/hành vi
nhưng cũng có phần riêng biệt, ta không có cách nào để "tái dùng" định nghĩa
struct/function cho phần chung — chỉ có thể:

- Copy-paste field giữa các struct (dễ lệch nhau khi sửa 1 chỗ quên chỗ khác).
- Hoặc nhúng struct chung làm field đầu tiên (kỹ thuật "poor man's inheritance"),
  rồi ép kiểu con trỏ qua lại bằng tay.

Cả hai cách đều không có sự ràng buộc nào từ compiler: không gì ngăn bạn quên
đồng bộ struct, hoặc ép kiểu sai chỗ. Inheritance trong C++ là cơ chế ngôn ngữ
hóa cái pattern "struct chung nhúng đầu" đó, để compiler tự lo phần bố trí bộ
nhớ và kiểm tra kiểu.

## 2. Đối chiếu với C
Pattern kinh điển trong C (ví dụ driver cho nhiều loại thiết bị dùng chung
"base"):

```c
// "base" struct — phần chung
typedef struct {
    uint32_t id;
    void (*init)(void *self);
} Device;

// "derived" struct — nhúng Device làm field đầu tiên
typedef struct {
    Device base;   // phải là field ĐẦU TIÊN để ép kiểu con trỏ hợp lệ
    uint32_t baud_rate;
} UartDevice;

void uart_init(void *self) {
    UartDevice *u = (UartDevice *)self; // tự ép kiểu, không ai kiểm tra giúp
    // ...
}
```

Ở đây bạn tự tay:
- Đảm bảo `base` nằm ở offset 0 để `(Device*)&uart_dev` và `&uart_dev` trỏ
  cùng địa chỉ (đây chính là thứ mà C++ sẽ làm ngầm và đảm bảo đúng, kể cả
  khi có nhiều base hoặc base không nằm ở offset 0).
- Tự quản lý con trỏ hàm (`init`) để giả lập "override" — đây là tiền thân
  thủ công của virtual function/vtable.
- Không có gì ngăn bạn gọi `uart_init()` trên một `Device*` không thực sự là
  `UartDevice*` — lỗi này compiler C không bắt được.

## 3. Cơ chế C++
Inheritance cho phép một class (derived) kế thừa member (data + function) từ
một class khác (base), đồng thời **tuyên bố rõ mối quan hệ "is-a"** để
compiler kiểm tra kiểu tại compile-time.

```cpp
class Device {
public:
    uint32_t id;
    void init() { /* dùng chung cho mọi Device */ }
};

class UartDevice : public Device {   // UartDevice "is-a" Device
public:
    uint32_t baudRate;
};
```

Những gì compiler làm ngầm:
- **Bố trí bộ nhớ**: object `UartDevice` chứa subobject `Device` ở đầu (thường
  vậy, trừ trường hợp có virtual base/nhiều base — sẽ nói ở bài multiple
  inheritance sau). Đây chính xác là pattern "nhúng struct đầu tiên" ở trên,
  nhưng compiler tự sắp xếp và đảm bảo đúng.
- **Con trỏ tự ép kiểu an toàn**: `UartDevice*` tự động chuyển thành `Device*`
  (upcasting) mà không cần cast tay, vì compiler biết chắc layout.
- **Kế thừa member**: `UartDevice` có sẵn `id` và `init()` mà không cần viết
  lại.
- **Access control**: từ khóa kế thừa (`public`/`protected`/`private`) quyết
  định member của base "lộ" ra ngoài `UartDevice` ở mức nào — khác với C nơi
  mọi field struct đều public như nhau.

Đây mới là **kế thừa tĩnh** (compile-time, không có `virtual`). Việc override
hành vi giống con trỏ hàm trong ví dụ C (runtime polymorphism) là chuyện của
`virtual function` — một cơ chế riêng, sẽ học ở bài sau.

## 4. Thực hành
```cpp
#include <iostream>

class Device {
public:
    uint32_t id;
    void init() { std::cout << "Device " << id << " init\n"; }
};

class UartDevice : public Device {
public:
    uint32_t baudRate;
};

int main() {
    UartDevice u;
    u.id = 1;
    u.baudRate = 115200;

    u.init();                 // gọi member kế thừa từ Device
    Device *d = &u;           // upcast tự động, không cần cast tay
    d->init();                // vẫn chạy Device::init (chưa có virtual)
}
```

So với bản C ở mục 2: không còn `(UartDevice*)self` thủ công, không còn lo
offset field — compiler đảm bảo `Device*` và `UartDevice*` trỏ đúng chỗ.

## 5. Cạm bẫy / lưu ý
- **Chi phí ẩn gần như bằng 0** với single inheritance không virtual: layout
  bộ nhớ và tốc độ truy cập member tương đương struct lồng nhau viết tay
  trong C — không có runtime overhead. Overhead chỉ xuất hiện khi có
  `virtual` (vtable pointer, indirect call) — nội dung của bài virtual
  function.
- **`public` vs `private` vs `protected` inheritance**: mặc định `class`
  kế thừa `private`, `struct` kế thừa `public`. Người quen C dễ quên khai
  báo `public` tường minh và ăn lỗi access không mong muốn.
- **"is-a" phải thật sự đúng về mặt logic**: dùng inheritance chỉ để tái sử
  dụng code (mà quan hệ không phải is-a) là anti-pattern — nên cân nhắc
  composition (member là object khác) giống hệt lựa chọn trong C giữa "nhúng
  struct" và "giữ con trỏ tới struct khác".
- **Slicing**: gán `UartDevice` vào biến kiểu `Device` (không phải con trỏ/
  reference) sẽ cắt mất phần `UartDevice`, chỉ còn lại phần `Device` — lỗi
  không tồn tại trong C vì C không có cơ chế "tự động cắt" struct.

## 6. Câu hỏi mở
- [ ] Multiple inheritance bố trí bộ nhớ thế nào khi có nhiều base?
- [ ] Virtual inheritance (diamond problem) giải quyết vấn đề gì?
- [ ] `protected` khác `private` thế nào trong ngữ cảnh kế thừa?
