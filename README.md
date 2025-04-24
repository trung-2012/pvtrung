# gameeee
 Họ và tên: Phạm Văn Trung
MSSV: 24021649
Chủ đề game: Máy Bay Chiến Tranh
Ngưỡng điểm mong muốn: 9 điểm
Các phần đã tự làm(có tham khảo AI) :
- Logic chính của game:
  + Điều khiển máy bay bằng bàn phím (di chuyển theo hướng).
  + Bắn đạn tự động theo thời gian, có buff bắn nhanh tạm thời.
  + Địch sinh ra theo thời gian, tự bắn đạn theo nhiều hướng.
  + Xử lý va chạm giữa đạn người chơi và địch, đạn địch và người chơi.
- Quản lý vòng lặp game:
  + Hiển thị nền trôi liên tục tạo cảm giác chuyển động.
  + Tạm dừng và tiếp tục game bằng nút pause/resume.
  + Thoát về menu bằng nút tương ứng.
- Hệ thống điểm:
  + Cộng điểm khi tiêu diệt kẻ địch.
  + Hiển thị điểm hiện tại liên tục khi chơi.
  + Lưu lại điểm cao nhất khi kết thúc game.
- Giao diện người dùng:
  + Menu game với các nút điều khiển (Play, Restart, Menu...).
  + Tạm dừng game có giao diện mờ kèm tiêu đề 'PAUSED'.
- Âm thanh:
  + Phát nhạc nền và hiệu ứng (bắn, nổ) bằng SDL_mixer.
  + Có chế độ bật/tắt âm thanh (isMuted).
- Tài nguyên (Assets):
  + Load hình ảnh máy bay, đạn, nền, nút bấm.
  + Sử dụng font để hiển thị điểm và trạng thái.
Các phần có tham khảo:
- Xử lý đồ họa: Dùng SDL để render hình ảnh, background, animation cơ bản.
- Quản lý tài nguyên: Load ảnh và âm thanh qua SDLUtils.
- Hiển thị văn bản: TTF_RenderText_Solid để vẽ điểm số và chữ 'PAUSED'.
Nguồn tài liệu tham khảo:
- Code mẫu và tutorial: Từ tài liệu bổ sung của môn Lập Trình Nâng Cao (LTNC).
- hình ảnh và âm thanh trên mạng
Hướng phát triển tương lai:
- Thêm các màn chơi với độ khó tăng dần.
- Thêm hiệu ứng nổ, animation nâng cao hơn.
- Cải tiến hệ thống điểm và thêm bảng xếp hạng online.
- Tích hợp hệ thống lưu và tiếp tục game.

