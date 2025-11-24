#include "DanhSachPhuongTien.h"
#include <cstdlib>
#include <limits>
#include <conio.h> // <-- Cần cho _getch() trên Windows

// Định nghĩa lệnh xóa màn hình
#ifdef _WIN32
#define CLEAR_SCREEN system("cls")
#else
#define CLEAR_SCREEN system("clear") // Cho Linux/macOS (nếu conio.h không được hỗ trợ)
#endif

// Hàm chờ người dùng nhấn Enter
void PressEnterToContinue() {
    cout << "\nNhan ENTER de tiep tuc...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Hàm tiện ích: Đọc lựa chọn không cần Enter
int ReadChoice() {
    // Chỉ hoạt động trên Windows/VS
    char key = _getch();
    if (key >= '0' && key <= '9') {
        return key - '0'; // Chuyển ký tự số sang giá trị số
    }
    return -1; // Trả về -1 nếu không phải phím số hợp lệ
}


// === Lớp CQuanLyTienGiaoThong (Đã tối ưu UX) ===
class CQuanLyTienGiaoThong {
private:
    CDanhSachPhuongTien dsPhuongTien;
    const int PHAN_TU_MOI_TRANG = 5;

    // Tieu de chung cho các chức năng con
    void HienThiTieuDe(const string& chucNang) const {
        CLEAR_SCREEN;
        cout << "========================================================\n";
        cout << "|| QUAN LY PHUONG TIEN --> " << chucNang << "\n";
        cout << "========================================================\n";
    }

    // Menu con: Chọn loại phương tiện
    CPhuongTien* ChonLoaiPhuongTien() {
        int choice;
        while (true) {
            HienThiTieuDe("Chon Loai Phuong Tien");
            cout << "1. Oto\n";
            cout << "2. Moto\n";
            cout << "0. Quay lai Menu chinh\n";
            cout << "Lua chon: ";

            choice = ReadChoice(); // <-- Đọc phím ngay lập tức
            cout << choice << endl;

            if (choice == 1) return new COto();
            if (choice == 2) return new CMoto();
            if (choice == 0) return nullptr;

            cout << "\nLua chon khong hop le. Vui long chon lai.\n";
            PressEnterToContinue();
        }
    }

public:
    CQuanLyTienGiaoThong() { dsPhuongTien.DocFile(); }
    ~CQuanLyTienGiaoThong() { dsPhuongTien.LuuFile(); }

    // === 1. Thêm Phương Tiện ===
    void ThemPhuongTien() {
        CPhuongTien* pt = ChonLoaiPhuongTien();
        if (pt == nullptr) return;

        HienThiTieuDe("Them Phuong Tien Moi");
        cout << "(Bien so la thong tin duy nhat. Neu Bien so de trong se huy them.)\n";

        // Cần dùng cin.ignore/getline cho phần Nhap
        pt->Nhap();

        if (pt->getBienSo().empty()) {
            cout << "⚠️ Da huy them phuong tien.\n";
            delete pt;
        }
        else if (dsPhuongTien.TimKiem(pt->getBienSo()) != nullptr) {
            cout << "❌ Phuong tien voi bien so nay da ton tai. Them that bai.\n";
            delete pt;
        }
        else {
            dsPhuongTien.Them(pt);
        }
        PressEnterToContinue();
    }

    // === 2. Xóa Phương Tiện ===
    void XoaPhuongTien() {
        HienThiTieuDe("Xoa Phuong Tien");
        string bienSo;
        cout << "Nhap Bien so phuong tien muon XOA (Nhap 0 de thoat): ";
        cin.ignore(); getline(cin, bienSo); // Vẫn dùng getline vì biển số có thể có khoảng trắng

        if (bienSo == "0") return;

        if (dsPhuongTien.Xoa(bienSo)) {
            cout << "✅ Da xoa thanh cong phuong tien Bien so: " << bienSo << endl;
        }
        else {
            cout << "❌ Khong tim thay phuong tien co Bien so: " << bienSo << endl;
        }
        PressEnterToContinue();
    }

    // === 3. Sửa Thông Tin Phương Tiện ===
    void SuaPhuongTien() {
        HienThiTieuDe("Sua Thong Tin Phuong Tien");
        string bienSo;
        cout << "Nhap Bien so phuong tien muon SUA (Nhap 0 de thoat): ";
        cin.ignore(); getline(cin, bienSo);

        if (bienSo == "0") return;

        dsPhuongTien.Sua(bienSo); // Hàm Sua tự xử lý nhập liệu
        PressEnterToContinue();
    }

    // === 4. Tìm Kiếm Phương Tiện ===
    void TimPhuongTien() {
        HienThiTieuDe("Tim Kiem Phuong Tien");
        string bienSo;
        cout << "Nhap Bien so phuong tien muon TIM KIEM (Nhap 0 de thoat): ";
        cin.ignore(); getline(cin, bienSo);

        if (bienSo == "0") return;

        CPhuongTien* pt = dsPhuongTien.TimKiem(bienSo);
        if (pt != nullptr) {
            cout << "\n✅ Tim thay phuong tien:\n";
            // Xuất bảng cho 1 phần tử
            cout << "+------------------+----------------+------------+----------+----------+----------------+----------+\n";
            cout << "| Bien So          | Hang SX        | Nam SX     | Mau Sac  | So Cho   | Kieu DC        | Loai Xe  |\n";
            cout << "+------------------+----------------+------------+----------+----------+----------------+----------+\n";
            pt->Xuat();
            cout << "+------------------+----------------+------------+----------+----------+----------------+----------+\n";
        }
        else {
            cout << "❌ Khong tim thay phuong tien co Bien so: " << bienSo << endl;
        }
        PressEnterToContinue();
    }

    // === 5. Hiển Thị Danh Sách Phương Tiện (Phân trang) ===
    void HienThiPhuongTien() {
        int tongSoPT = dsPhuongTien.DemSoLuong();
        if (tongSoPT == 0) {
            HienThiTieuDe("Hien Thi Danh Sach");
            cout << "Danh sach phuong tien rong.\n";
            PressEnterToContinue();
            return;
        }

        int tongSoTrang = (tongSoPT + PHAN_TU_MOI_TRANG - 1) / PHAN_TU_MOI_TRANG;
        int trangHienTai = 1;
        int choice;

        do {
            HienThiTieuDe("Hien Thi Danh Sach");

            dsPhuongTien.XuatDanhSach(trangHienTai, PHAN_TU_MOI_TRANG);

            cout << "Trang " << trangHienTai << " / " << tongSoTrang << " | Tong so: " << tongSoPT << "\n";
            cout << "--------------------------------------------------------\n";
            cout << "1. Trang truoc | 2. Trang sau | 0. Quay lai Menu chinh\n";
            cout << "Lua chon: ";

            choice = ReadChoice(); // <-- Đọc phím ngay lập tức
            cout << choice << endl;

            if (choice == 1) { // Trang trước
                if (trangHienTai > 1) trangHienTai--;
                else { cout << "\n⚠️ Da o trang dau tien.\n"; PressEnterToContinue(); }
            }
            else if (choice == 2) { // Trang sau
                if (trangHienTai < tongSoTrang) trangHienTai++;
                else { cout << "\n⚠️ Da o trang cuoi cung.\n"; PressEnterToContinue(); }
            }
            else if (choice != 0) {
                cout << "\nLua chon khong hop le.\n";
                PressEnterToContinue();
            }

        } while (choice != 0);
    }

    // === 6. Lưu Dữ Liệu ===
    void LuuDanhSach() {
        HienThiTieuDe("Luu Du Lieu");
        dsPhuongTien.LuuFile();
        PressEnterToContinue();
    }

    // === 7. Đọc Dữ Liệu ===
    void DocDanhSach() {
        HienThiTieuDe("Doc Du Lieu");
        dsPhuongTien.DocFile();
        PressEnterToContinue();
    }

    // === Menu Chính ===
    void MenuChinh() {
        int choice;
        do {
            CLEAR_SCREEN;
            cout << "========================================================\n";
            cout << "|| MENU CHINH: QUAN LY PHUONG TIEN GIAO THONG\n";
            cout << "========================================================\n";
            cout << "1. Them phuong tien\n";
            cout << "2. Xoa phuong tien\n";
            cout << "3. Sua thong tin phuong tien\n";
            cout << "4. Tim kiem phuong tien\n";
            cout << "5. Hien thi danh sach phuong tien\n";
            cout << "6. Luu du lieu\n";
            cout << "7. Doc du lieu\n";
            cout << "0. Thoat chuong trinh\n";
            cout << "Lua chon cua ban: ";

            choice = ReadChoice(); // <-- Đọc phím ngay lập tức
            cout << choice << endl;

            switch (choice) {
            case 1: ThemPhuongTien(); break;
            case 2: XoaPhuongTien(); break;
            case 3: SuaPhuongTien(); break;
            case 4: TimPhuongTien(); break;
            case 5: HienThiPhuongTien(); break;
            case 6: LuuDanhSach(); break;
            case 7: DocDanhSach(); break;
            case 0: cout << "✅ Dang thoat chuong trinh... Tu dong luu du lieu.\n"; break;
            default:
                cout << "❌ Lua chon khong hop le. Vui long chon lai.\n";
                PressEnterToContinue();
            }
        } while (choice != 0);
    }
};

int main() {
    // Tắt đồng bộ I/O
    ios_base::sync_with_stdio(false);

    CQuanLyTienGiaoThong app;
    app.MenuChinh();

    return 0;
}