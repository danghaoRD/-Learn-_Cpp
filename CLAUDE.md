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
└── notes.md          # nội dung đã chốt, note hoàn chỉnh
```

- `sources/*.md`: nếu là bài viết/web, chỉ cần ghi URL (Claude Code tự fetch được).
  Nếu là video YouTube, dán transcript vào đây (Claude Code không xem được video).
- `questions.md`: nơi tôi ghi câu hỏi nháp ngay sau khi xem bài giảng, kèm link
  tới file trong `sources/` liên quan.
- `notes.md`: nơi Claude Code viết note hoàn chỉnh sau khi tôi hỏi xong.

## Định dạng nội dung trong notes.md

Mỗi khái niệm trình bày theo thứ tự sau (bỏ mục nào không có nội dung, đừng để trống hình thức):

1. **Vấn đề** — C++ giải quyết bài toán gì mà C thuần không làm được / làm được nhưng rất cồng kềnh, dễ lỗi?
2. **Đối chiếu với C** — trên C tôi phải làm tương đương thế nào (thường là tay làm bằng struct + function pointer, malloc/free thủ công...). Đây là cầu nối tư duy, gần như bài nào cũng nên có.
3. **Cơ chế C++** — cú pháp, quy tắc hoạt động, những gì trình biên dịch làm ngầm (constructor/destructor tự gọi, vtable, template instantiation...).
4. **Thực hành** — đoạn code C++ chạy được, so sánh side-by-side với bản C nếu hợp lý.
5. **Cạm bẫy / lưu ý** — chi phí ẩn (hidden cost) so với C, chỗ dễ dùng sai khi mang tư duy C sang thẳng C++, các UB đặc thù.
6. **Câu hỏi mở** — dạng checkbox `- [ ]` để theo dõi phần chưa thông.

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
