#ifndef DANHSACHPHUONGTIEN_H
#define DANHSACHPHUONGTIEN_H

#include "PhuongTien.h"

// === Cấu Trúc NodePhuongTien ===
struct NodePhuongTien {
    CPhuongTien* data; // Con trỏ tới đối tượng CPhuongTien* (Đa hình)
    NodePhuongTien* next;

    NodePhuongTien(CPhuongTien* p) : data(p), next(nullptr) {}
};

// === Lớp CDanhSachPhuongTien ===
class CDanhSachPhuongTien {
private:
    NodePhuongTien* head;
    const string FILENAME = "data_phuongtien.txt";

    NodePhuongTien* TimNodeTruoc(string BienSo) {
        NodePhuongTien* current = head;
        NodePhuongTien* prev = nullptr;
        while (current != nullptr && current->data->getBienSo() != BienSo) {
            prev = current;
            current = current->next;
        }
        return prev;
    }

public:
    CDanhSachPhuongTien() : head(nullptr) {}

    // Destructor: Giải phóng bộ nhớ
    ~CDanhSachPhuongTien() {
        NodePhuongTien* current = head;
        while (current != nullptr) {
            NodePhuongTien* nextNode = current->next;
            delete current->data;
            delete current;
            current = nextNode;
        }
        head = nullptr;
    }

    // Hàm tiện ích: Đếm số lượng phương tiện
    int DemSoLuong() const {
        int count = 0;
        NodePhuongTien* current = head;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        return count;
    }

    // === 1. Thêm phương tiện ===
    void Them(CPhuongTien* pt) {
        NodePhuongTien* newNode = new NodePhuongTien(pt);
        if (head == nullptr) {
            head = newNode;
        }
        else {
            NodePhuongTien* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        cout << "✅ Da them phuong tien Bien so: " << pt->getBienSo() << endl;
    }

    // === Tìm kiếm phương tiện ===
    CPhuongTien* TimKiem(string BienSo) {
        NodePhuongTien* current = head;
        while (current != nullptr) {
            if (current->data->getBienSo() == BienSo) {
                return current->data;
            }
            current = current->next;
        }
        return nullptr;
    }

    // === 2. Xóa phương tiện ===
    bool Xoa(string BienSo) {
        if (head == nullptr) return false;

        if (head->data->getBienSo() == BienSo) {
            NodePhuongTien* temp = head;
            head = head->next;
            delete temp->data;
            delete temp;
            return true;
        }

        NodePhuongTien* prev = TimNodeTruoc(BienSo);
        if (prev != nullptr && prev->next != nullptr) {
            NodePhuongTien* temp = prev->next;
            prev->next = temp->next;
            delete temp->data;
            delete temp;
            return true;
        }
        return false;
    }

    // === 3. Sửa thông tin phương tiện ===
    bool Sua(string BienSo) {
        CPhuongTien* pt = TimKiem(BienSo);
        if (pt == nullptr) {
            cout << "❌ Khong tim thay phuong tien co Bien so: " << BienSo << endl;
            return false;
        }

        cout << "\n**Nhap THONG TIN MỚI cho phuong tien Bien so: " << BienSo << endl;
        int choice;

        cout << "Chon thong tin muon sua:\n";
        cout << "1. Hang SX\n2. Nam SX\n3. Mau Sac\n4. Thong tin rieng\n0. Huy bo\n";
        cout << "Lua chon: ";

        // Cần dùng cin >> choice vì không thể dùng _getch trong hàm này
        // (Do _getch sẽ làm mất tính đồng bộ với các lệnh cin.ignore() tiếp theo)
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Nhap khong hop le.\n"; return false;
        }

        if (choice == 1) {
            string newHangSX;
            cout << "   Hang san xuat moi: "; cin.ignore(); getline(cin, newHangSX); pt->setHangSX(newHangSX);
        }
        else if (choice == 2) {
            int newNamSX;
            cout << "   Nam san xuat moi: "; cin >> newNamSX; pt->setNamSX(newNamSX);
        }
        else if (choice == 3) {
            string newMauSac;
            cout << "   Mau sac moi: "; cin.ignore(); getline(cin, newMauSac); pt->setMauSac(newMauSac);
        }
        else if (choice == 4) {
            if (pt->getLoaiPhuongTien() == "Oto") {
                COto* oto = dynamic_cast<COto*>(pt);
                int newSoCho;
                string newKieuDC;
                cout << "   So cho ngoi moi: "; cin >> newSoCho; oto->setSoChoNgoi(newSoCho);
                cout << "   Kieu dong co moi: "; cin.ignore(); getline(cin, newKieuDC); oto->setKieuDongCo(newKieuDC);
            }
            else if (pt->getLoaiPhuongTien() == "Moto") {
                CMoto* moto = dynamic_cast<CMoto*>(pt);
                string newLoaiXe;
                cout << "   Loai xe moi: "; cin.ignore(); getline(cin, newLoaiXe); moto->setLoaiXe(newLoaiXe);
            }
        }
        else if (choice == 0) {
            cout << "⚠️ Da huy sua thong tin.\n";
            return false;
        }
        else {
            cout << "Khong hop le!\n"; return false;
        }

        cout << "✅ Cap nhat thong tin thanh cong!\n";
        return true;
    }

    // === 5. Hiển thị danh sách phương tiện (Có phân trang) ===
    void XuatDanhSach(int trangHienTai, int soPhanTuMoiTrang) const {
        if (head == nullptr) return;

        int start = (trangHienTai - 1) * soPhanTuMoiTrang;
        int count = 0;
        int displayedCount = 0;
        NodePhuongTien* current = head;

        while (current != nullptr && count < start) {
            current = current->next;
            count++;
        }

        // Xuất bảng
        cout << "+------------------+----------------+------------+----------+----------+----------------+----------+\n";
        cout << "| Bien So          | Hang SX        | Nam SX     | Mau Sac  | So Cho   | Kieu DC        | Loai Xe  |\n";
        cout << "+------------------+----------------+------------+----------+----------+----------------+----------+\n";

        // Xuất dữ liệu của trang hiện tại
        while (current != nullptr && displayedCount < soPhanTuMoiTrang) {
            current->data->Xuat();
            current = current->next;
            displayedCount++;
        }

        cout << "+------------------+----------------+------------+----------+----------+----------------+----------+\n";
    }

    // === 6. Lưu dữ liệu ===
    bool LuuFile() const {
        // ... (Logic lưu file giữ nguyên)
        ofstream ofs(FILENAME);
        if (!ofs.is_open()) {
            cout << "❌ Khong mo duoc file de luu: " << FILENAME << endl;
            return false;
        }

        NodePhuongTien* current = head;
        int count = 0;
        while (current != nullptr) {
            current->data->GhiFile(ofs);
            count++;
            current = current->next;
        }
        ofs.close();
        cout << "✅ Luu thanh cong " << count << " phuong tien vao file: " << FILENAME << endl;
        return true;
    }

    // === 7. Đọc dữ liệu ===
    bool DocFile() {
        // ... (Logic đọc file giữ nguyên)
        ifstream ifs(FILENAME);
        if (!ifs.is_open()) {
            // Đã đổi sang cảnh báo thay vì lỗi nghiêm trọng
            cout << "⚠️ File du lieu chua ton tai hoac khong mo duoc. (Se tao moi khi luu lan dau)\n";
            return false;
        }

        // Dọn dẹp dữ liệu cũ
        NodePhuongTien* current = head;
        while (current != nullptr) {
            NodePhuongTien* nextNode = current->next;
            delete current->data;
            delete current;
            current = nextNode;
        }
        head = nullptr;

        string loaiPT;
        int count = 0;
        while (getline(ifs, loaiPT)) {
            CPhuongTien* pt = nullptr;
            if (loaiPT == "Oto") {
                pt = new COto();
            }
            else if (loaiPT == "Moto") {
                pt = new CMoto();
            }

            if (pt != nullptr) {
                pt->DocFile(ifs);
                Them(pt); // Tái sử dụng hàm Them
                count++;
            }
        }

        ifs.close();
        if (count > 0) {
            cout << "✅ Doc thanh cong " << count << " phuong tien tu file: " << FILENAME << endl;
            return true;
        }
        return false;
    }
};

#endif // DANHSACHPHUONGTIEN_H