#ifndef PHUONGTIEN_H
#define PHUONGTIEN_H

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

// === Lớp Cơ Sở: CPhuongTien ===
class CPhuongTien {
protected:
    string BienSo;
    string HangSX;
    int NamSX;
    string MauSac;

public:
    CPhuongTien() : NamSX(0) {}
    CPhuongTien(string bs, string hsx, int nsx, string ms)
        : BienSo(bs), HangSX(hsx), NamSX(nsx), MauSac(ms) {
    }
    virtual ~CPhuongTien() {} // Destructor ảo

    // Phương thức Nhập/Xuất (virtual)
    virtual void Nhap() {
        cout << "   Bien so: ";                  cin.ignore(); getline(cin, BienSo);
        cout << "   Hang san xuat: ";           getline(cin, HangSX);
        cout << "   Nam san xuat: ";            cin >> NamSX;
        cout << "   Mau sac: ";                 cin.ignore(); getline(cin, MauSac);
    }

    // Xuất bảng: In 4 cột chung và KHÔNG xuống dòng
    virtual void Xuat() const {
        // Độ rộng đã căn chỉnh để khớp với tiêu đề trong DanhSachPhuongTien.h
        cout << "|" << left << setw(16) << BienSo
            << "|" << setw(15) << HangSX
            << "|" << setw(10) << NamSX
            << "|" << setw(9) << MauSac;
    }

    // Ghi file
    virtual void GhiFile(ofstream& ofs) const {
        ofs << BienSo << "\n"
            << HangSX << "\n"
            << NamSX << "\n"
            << MauSac << "\n";
    }

    // Đọc file
    virtual void DocFile(ifstream& ifs) {
        getline(ifs, BienSo);
        getline(ifs, HangSX);
        //ifs >> NamSX;
        ifs.ignore();
        getline(ifs, MauSac);
    }

    // Getters
    string getBienSo() const { return BienSo; }
    void setHangSX(string hsx) { HangSX = hsx; }
    void setNamSX(int nsx) { NamSX = nsx; }
    void setMauSac(string ms) { MauSac = ms; }

    // Phương thức hỗ trợ tạo đối tượng đúng loại khi đọc file (Pure virtual)
    virtual string getLoaiPhuongTien() const = 0;
};

// === Lớp Kế Thừa: COto ===
class COto : public CPhuongTien {
private:
    int SoChoNgoi;
    string KieuDongCo;

public:
    COto() : SoChoNgoi(0) {}
    ~COto() override {}

    void Nhap() override {
        cout << "**Nhap thong tin OTO**\n";
        CPhuongTien::Nhap();
        cout << "   So cho ngoi: ";             cin >> SoChoNgoi;
        cout << "   Kieu dong co (Xang/Dau/Dien): "; cin.ignore(); getline(cin, KieuDongCo);
    }

    // Xuất bảng: In 3 cột riêng và XUỐNG DÒNG
    void Xuat() const override {
        CPhuongTien::Xuat();
        cout << "|" << setw(9) << SoChoNgoi
            << "|" << setw(14) << KieuDongCo
            << "|" << setw(8) << "N/A"
            << "|" << endl; // <-- XUỐNG DÒNG
    }

    void GhiFile(ofstream& ofs) const override {
        ofs << getLoaiPhuongTien() << "\n";
        CPhuongTien::GhiFile(ofs);
        ofs << SoChoNgoi << "\n"
            << KieuDongCo << "\n";
    }

    void DocFile(ifstream& ifs) override {
        CPhuongTien::DocFile(ifs);
        ifs >> SoChoNgoi;
        ifs.ignore();
        getline(ifs, KieuDongCo);
    }

    string getLoaiPhuongTien() const override { return "Oto"; }

    // Getters/Setters riêng của Oto
    int getSoChoNgoi() const { return SoChoNgoi; }
    void setSoChoNgoi(int scn) { SoChoNgoi = scn; }
    void setKieuDongCo(string kdc) { KieuDongCo = kdc; }
};

// === Lớp Kế Thừa: CMoto ===
class CMoto : public CPhuongTien {
private:
    string LoaiXe;

public:
    CMoto() {}
    ~CMoto() override {}

    void Nhap() override {
        cout << "**Nhap thong tin MOTO**\n";
        CPhuongTien::Nhap();
        cout << "   Loai xe (Tay ga/Xe so/Xe con): "; cin.ignore(); getline(cin, LoaiXe);
    }

    // Xuất bảng: In 3 cột riêng và XUỐNG DÒNG
    void Xuat() const override {
        CPhuongTien::Xuat();
        cout << "|" << setw(9) << "N/A"
            << "|" << setw(14) << "N/A"
            << "|" << setw(8) << LoaiXe
            << "|" << endl; // <-- XUỐNG DÒNG
    }

    void GhiFile(ofstream& ofs) const override {
        ofs << getLoaiPhuongTien() << "\n";
        CPhuongTien::GhiFile(ofs);
        ofs << LoaiXe << "\n";
    }

    void DocFile(ifstream& ifs) override {
        CPhuongTien::DocFile(ifs);
        getline(ifs, LoaiXe);
    }

    string getLoaiPhuongTien() const override { return "Moto"; }

    // Getters/Setters riêng của Moto
    void setLoaiXe(string lx) { LoaiXe = lx; }
    string getLoaiXe() const { return LoaiXe; }
};

#endif // PHUONGTIEN_H