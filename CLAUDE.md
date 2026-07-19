# Quy tắc chung khi ghi note C++

## Bối cảnh
Tôi có nền tảng C vững (embedded MCU, bare-metal) và đang học Linux song song.
C++ với tôi là ngôn ngữ mới nhưng không phải khái niệm lập trình mới hoàn toàn —
điểm khó chủ yếu là các khái niệm không có trong C: OOP, template, RAII, quản lý
bộ nhớ tự động, thư viện chuẩn (STL).

## Cấu trúc mỗi thư mục bài học

```
NN-ten-bai-hoc/
├── sources/          # transcript video, note dẫn nguồn tài liệu gốc
├── questions.md      # câu hỏi đang thắc mắc, nguồn liên quan
├── notes.md          # nội dung đã chốt, note hoàn chỉnh
└── exercises/        # bài tập thực hành + lời giải, xem mục riêng bên dưới
```

- `sources/*.md`: nếu là bài viết/web, chỉ cần ghi URL (Claude Code tự fetch được).
  Nếu là video YouTube, dán transcript vào đây (Claude Code không xem được video).
- `questions.md`: nơi tôi ghi câu hỏi nháp ngay sau khi xem bài giảng, kèm link
  tới file trong `sources/` liên quan.
- `notes.md`: nơi Claude Code viết note hoàn chỉnh sau khi tôi hỏi xong.
- `exercises/`: chỉ tạo khi tôi yêu cầu làm bài tập cho thư mục đó.

## Định dạng nội dung trong notes.md

Mỗi khái niệm trình bày theo thứ tự sau (bỏ mục nào không có nội dung, đừng để trống hình thức):

1. **Vấn đề** — C++ giải quyết bài toán gì mà C thuần không làm được / làm được nhưng rất cồng kềnh, dễ lỗi?
2. **Đối chiếu với C** — trên C tôi phải làm tương đương thế nào (thường là tay làm bằng struct + function pointer, malloc/free thủ công...). Đây là cầu nối tư duy, gần như bài nào cũng nên có.
3. **Cơ chế C++** — cú pháp, quy tắc hoạt động, những gì trình biên dịch làm ngầm (constructor/destructor tự gọi, vtable, template instantiation...).
4. **Thực hành** — đoạn code C++ chạy được, so sánh side-by-side với bản C nếu hợp lý.
5. **Cạm bẫy / lưu ý** — chi phí ẩn (hidden cost) so với C, chỗ dễ dùng sai khi mang tư duy C sang thẳng C++, các UB đặc thù.
6. **Câu hỏi mở** — dạng checkbox `- [ ]` để theo dõi phần chưa thông.

## Bài tập thực hành (exercises/)

Chưa có nguồn bài tập ngoài (sách/web) — bài tập do Claude Code tự soạn dựa trên
nội dung đã chốt trong `notes.md` của (các) bài liên quan, để luôn bám sát đúng
kiến thức vừa học thay vì lan man ngoài phạm vi.

- **Vị trí**: `course/NN-ten-bai-hoc/exercises/`, mỗi bài tập một file
  `exNN_ten-chu-de.cpp` (số thứ tự không nhất thiết trùng số thứ tự file note,
  đặt theo tên chủ đề cho dễ tra).
- **Định dạng 1 file**: gộp đề và lời giải trong cùng file, không tách file
  riêng.
  - Phần đề: code khung với comment `// TODO: ...` tại chỗ cần tự viết. Một số
    TODO có thể khiến file **chưa compile được cho tới khi làm xong** — đây là
    chủ đích (ví dụ bài về `const` cố tình để lỗi compiler hiện ra), không phải
    lỗi cần sửa.
  - Cuối file: khối `/* ===== LỜI GIẢI ===== ... ===================== */`
    chứa lời giải đầy đủ, gọn lại phần TODO tương ứng. Muốn build lời giải:
    comment phần TODO đã viết, bỏ comment khối lời giải.
  - Trước khi đưa file cho tôi, Claude Code phải tự trích lời giải ra build thử
    (`g++ -std=c++17 -Wall -Wextra`) để đảm bảo chạy đúng, không chỉ đưa code
    chưa test.
- **Độ khó**: mỗi chủ đề 2-3 bài từ dễ đến khó, bám vào đúng ví dụ/khái niệm đã
  nêu trong `notes.md` (không bịa thêm khái niệm ngoài nguồn).
- **Bài tập kết hợp**: ngoài bài riêng lẻ theo từng khái niệm, có thể thêm vài
  bài kết hợp nhiều khái niệm trong cùng thư mục lớn — chỉ khi có tình huống
  ứng dụng thực tế tự nhiên gộp chúng lại (ví dụ: một class quản lý tài nguyên
  vừa cần Rule of Three vừa cần static counter vừa cần friend để in log), không
  gộp máy móc cho đủ số lượng khái niệm.
- **Output trong `main()`**: trước mỗi bài trong file, in một dòng tag
  `std::cout << "[Bai N] mo ta ngan\n";` để phân biệt output của bài nào khi
  chạy chung 1 file — không chỉ dựa vào comment `// --- Test bài N ---`
  (comment không hiện ra khi chạy). Áp dụng cho cả phần đề và phần lời giải.
- **Build/chạy**: dùng `.vscode/tasks.json` (Ctrl+Shift+B — build file `.cpp`
  đang mở) và `.vscode/launch.json` (F5 — build rồi debug bằng gdb) đã cấu hình
  sẵn ở gốc repo, dùng chung cho mọi bài tập, không cần CMake trừ khi bài tập
  sau này có nhiều file/phụ thuộc lẫn nhau.

## Nguyên tắc khi trả lời

1. Ưu tiên giải thích **tại sao** cần cơ chế này, thay vì chỉ nói cú pháp và cách dùng.
2. Luôn cố **bắc cầu từ C** — dùng cái tôi đã biết (con trỏ, struct, quản lý bộ nhớ thủ công) để giải thích cái mới.
3. Làm rõ **chi phí ẩn**: mỗi tính năng tiện lợi của C++ (RAII, exception, virtual, template) đều đánh đổi gì so với viết tay bằng C — về hiệu năng, kích thước binary, độ khó debug. Đây là mối quan tâm hợp lý với người có nền embedded.
4. Ưu tiên **ví dụ code chạy được** hơn mô tả trừu tượng.
5. Khi tôi hỏi trong `questions.md`, trả lời rồi tổng hợp phần đã chốt vào `notes.md` theo đúng định dạng ở trên.

## Khi tôi yêu cầu đọc nguồn

- File trong `sources/`:
  - Nếu là link bài viết/web → tự `WebFetch` URL đó rồi tóm tắt phần liên quan tới câu hỏi.
  - Nếu là transcript video (YouTube) → đọc nội dung đã dán trong file, không cố truy cập video.
- Bám vào nguồn tôi đã đưa; nếu cần kiến thức ngoài nguồn thì nói rõ "phần này ngoài nguồn".
- Trước khi viết note, đọc `questions.md` của bài đó **cùng với** các file `sources/` mà nó trỏ tới.
- Nếu nguồn mâu thuẫn hoặc thiếu, nêu ra ở mục "Câu hỏi mở" thay vì tự lấp đầy.

## Danh sách bài học (cập nhật index này khi thêm bài mới)

| STT | Bài học | Thư mục | Trạng thái |
|---|---|---|---|
| 00 | Template mẫu | `course/00-template/` | 📄 khung |

Khi thêm bài mới: copy `course/00-template/` thành `course/NN-ten-bai-hoc/`, thêm dòng vào bảng trên và cập nhật `index.md`.
