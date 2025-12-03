#include "lamaran.h"
#include "lowongan.h"   // Diperlukan untuk mengakses fungsi findParent
#include "mahasiswa.h"  // Diperlukan untuk mengakses fungsi handleInputMahasiswa dan findChildByNIM
#include <iomanip>
#include <cstring>

using namespace std;

// Catatan: Fungsi yang dipanggil di sini harus sudah diimplementasikan
// di file lowongan.cpp dan mahasiswa.cpp masing-masing.

#include <fstream> // Include for file reading

// Helper function to calculate ATS Score from File
int hitungSkorATS(string filePath) {
    int score = 0;
    string keywords[] = {"C++", "Python", "Java", "SQL", "Teamwork", "Leadership", "Communication", "Problem Solving", "Analisis", "Desain"};
    
    ifstream file(filePath, ios::binary); // Open in binary mode to handle PDF/any file safely
    if (!file.is_open()) {
        return 0; // File not found or cannot open
    }

    // Read entire file into a string buffer
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    for (const string& keyword : keywords) {
        // Simple search in the raw binary/text content
        if (content.find(keyword) != string::npos) {
            score += 10;
        }
    }
    return score;
}

// 1. Insert element relation (Mahasiswa Ajukan Lamaran)
void insertRelasi(ListParent &L_Parent, ListChild &L_Child, int ID_Lowongan, string NIM_Mhs, string Nama_Mhs, int ID_Lamaran_Baru, string CV_Path) {
    // 1. Pastikan data Mahasiswa ada/dibuat di List Child
    handleInputMahasiswa(L_Child, NIM_Mhs, Nama_Mhs);

    // 2. Cari Node Parent dan Child
    address_parent P_Parent = findParent(L_Parent, ID_Lowongan);
    address_child P_Child = findChildByNIM(L_Child, NIM_Mhs);

    if (P_Parent != nullptr && P_Child != nullptr) {
        address_relasi R_Baru = new NodeRelasi;
        R_Baru->info.id_lamaran = ID_Lamaran_Baru;
        R_Baru->info.status_dosen = 0; // Menunggu
        R_Baru->info.status_perusahaan = 0; // Menunggu
        R_Baru->info.cv_path = CV_Path;
        R_Baru->info.cv_score = hitungSkorATS(CV_Path);

        // 3. Hubungkan Pointer Kunci MLL
        R_Baru->ptr_parent = P_Parent;
        R_Baru->ptr_child = P_Child;
        R_Baru->next = nullptr;

        // 4. Insert Last Relasi ke Sub-List Lowongan
        if (P_Parent->first_relasi == nullptr) {
            P_Parent->first_relasi = R_Baru;
        } else {
            address_relasi R_Last = P_Parent->first_relasi;
            while (R_Last->next != nullptr) R_Last = R_Last->next;
            R_Last->next = R_Baru;
        }
        cout << " [BERHASIL] Lamaran ID: " << ID_Lamaran_Baru << " (Simpan ID ini untuk verifikasi!) diajukan ke Lowongan ID: " << ID_Lowongan << "." << endl;
        cout << " [ATS] File CV diproses: " << CV_Path << endl;
        cout << " [ATS] Skor awal: " << R_Baru->info.cv_score << endl;
    } else {
        cout << " Error: Lowongan ID " << ID_Lowongan << " tidak ditemukan. Lamaran dibatalkan." << endl;
    }
}

// Tambahkan di lamaran.cpp

// Catatan: Asumsikan Anda dapat mengakses ListChild L_Child jika diperlukan
// Namun, karena Node Relasi (R) sudah menyimpan ptr_child, kita bisa traversing Parent saja.

void showStatusLamaranMahasiswa(ListParent L_Parent, string NIM_Target) {
    address_parent P = L_Parent.first;
    bool found_lamaran = false;

    cout << "\n========================================================" << endl;
    cout << "STATUS LAMARAN ANDA (NIM: " << NIM_Target << ")" << endl;
    cout << "========================================================" << endl;

    while (P != nullptr) {
        address_relasi R = P->first_relasi;
        while (R != nullptr) {
            // Cek apakah NIM pada ptr_child Node Relasi cocok dengan NIM_Target
            if (R->ptr_child->info.nim == NIM_Target) {
                found_lamaran = true;

                cout << "  [Lowongan ID " << P->info.id_lowongan << "]: " << P->info.posisi
                     << " (" << P->info.nama_perusahaan << ")" << endl;
                cout << "    Status Dosen: " << (R->info.status_dosen == 1 ? "Disetujui" : (R->info.status_dosen == 2 ? "Ditolak" : "Menunggu")) << endl;
                cout << "    Status Perusahaan: " << (R->info.status_perusahaan == 1 ? "**DITERIMA**" : (R->info.status_perusahaan == 2 ? "DITOLAK" : "Menunggu")) << endl;
                cout << "    Skor ATS: " << R->info.cv_score << endl;
                cout << "---------------------------------------" << endl;
            }
            R = R->next;
        }
        P = P->next;
    }

    if (!found_lamaran) {
        cout << "Tidak ada lamaran ditemukan untuk NIM " << NIM_Target << "." << endl;
    }
}
// Tambahkan atau pastikan implementasi ini ada di lamaran.cpp

void showRekapLamaranPerusahaan(ListParent L_Parent) {
    address_parent P = L_Parent.first;

    cout << "\n========================================================" << endl;
    cout << "REKAP DETAIL LAMARAN (Untuk Admin/Perusahaan)" << endl;
    cout << "========================================================" << endl;

    while (P != nullptr) {
        int count_pelamar = 0;
        int count_diterima = 0;

        cout << "\n[LOWONGAN ID " << P->info.id_lowongan << "]: " << P->info.posisi
             << " (Perusahaan: " << P->info.nama_perusahaan << ")" << endl;

        address_relasi R = P->first_relasi;

        if (R == nullptr) {
            cout << "   > Total Pelamar: 0" << endl;
        } else {
            cout << "   DETAIL PELAMAR:" << endl;
            while (R != nullptr) {
                count_pelamar++;
                if (R->info.status_perusahaan == 1) {
                    count_diterima++;
                }

                address_child C = R->ptr_child;
                cout << "   - NIM: " << C->info.nim << " | Nama: " << C->info.nama << endl;
                cout << "     File CV: " << R->info.cv_path << endl;
                cout << "     Skor ATS: " << R->info.cv_score << endl;
                cout << "     Status Akhir: " << (R->info.status_perusahaan == 1 ? "DITERIMA" : "DITOLAK/MENUNGGU") << endl;

                R = R->next;
            }
            cout << "   ---------------------------------------" << endl;
            cout << "   > Total Pelamar: " << count_pelamar << endl;
            cout << "   > Total Diterima: " << count_diterima << endl;
        }
        P = P->next;
    }
}

// Catatan: Pastikan juga Anda menghapus atau mengganti semua stubs (placeholder)
// yang tersisa di lamaran.cpp dengan implementasi aslinya.
// Tambahkan atau pastikan fungsi-fungsi ini sudah ada di lamaran.cpp

// Implementasi fungsi Edit Dosen (untuk verifikasi)
void editStatusDosen(ListParent &L_Parent, int ID_Lamaran_Target, int Status_Baru) {
    address_parent P_Parent = L_Parent.first;
    address_relasi R_Target = nullptr;

    // Cari Node Relasi (Lamaran) di seluruh Lowongan
    while (P_Parent != nullptr && R_Target == nullptr) {
        address_relasi R = P_Parent->first_relasi;
        while (R != nullptr) {
            if (R->info.id_lamaran == ID_Lamaran_Target) {
                R_Target = R;
                break;
            }
            R = R->next;
        }
        if (R_Target != nullptr) break;
        P_Parent = P_Parent->next;
    }

    if (R_Target != nullptr) {
        if (Status_Baru == 1 || Status_Baru == 2) {
            R_Target->info.status_dosen = Status_Baru;
            cout << "Status Verifikasi Dosen untuk Lamaran ID " << ID_Lamaran_Target << " diperbarui menjadi: **" << (Status_Baru == 1 ? "DISETUJUI" : "DITOLAK") << "**" << endl;
        } else {
            cout << "Input status verifikasi tidak valid (1=Setuju, 2=Tolak)." << endl;
        }
    } else {
        cout << "Lamaran dengan ID " << ID_Lamaran_Target << " tidak ditemukan." << endl;
    }
}

// Implementasi fungsi Edit Perusahaan (untuk keputusan akhir)
void editStatusPerusahaan(ListParent &L_Parent, int ID_Lamaran_Target, int Status_Baru) {
    address_parent P_Parent = L_Parent.first;
    address_relasi R_Target = nullptr;

    // Cari Node Relasi (Lamaran) di seluruh Lowongan
    while (P_Parent != nullptr && R_Target == nullptr) {
        address_relasi R = P_Parent->first_relasi;
        while (R != nullptr) {
            if (R->info.id_lamaran == ID_Lamaran_Target) {
                R_Target = R;
                break;
            }
            R = R->next;
        }
        if (R_Target != nullptr) break;
        P_Parent = P_Parent->next;
    }

    if (R_Target != nullptr) {
        // Logika Penting: Perusahaan hanya memproses jika Dosen sudah menyetujui (Status 1)
        if (R_Target->info.status_dosen != 1) {
            cout << "GAGAL. Lamaran ID " << ID_Lamaran_Target << " belum diverifikasi/ditolak Dosen (Status harus DISETUJUI)." << endl;
            return;
        }

        if (Status_Baru == 1 || Status_Baru == 2) {
            R_Target->info.status_perusahaan = Status_Baru;
            cout << "Keputusan Perusahaan untuk Lamaran ID " << ID_Lamaran_Target << " diperbarui menjadi: **" << (Status_Baru == 1 ? "DITERIMA" : "DITOLAK") << "**" << endl;
        } else {
            cout << "Status input tidak valid." << endl;
        }
    } else {
        cout << "Lamaran dengan ID " << ID_Lamaran_Target << " tidak ditemukan." << endl;
    }
}

// Catatan: Anda juga harus memastikan fungsi showLowonganDanPelamar sudah diimplementasikan untuk Show M:N.
void showLowonganDanPelamar(ListParent L_Parent) {
    address_parent P = L_Parent.first;

    cout << "\n========================================================" << endl;
    cout << "LAPORAN M:N LENGKAP: LOWONGAN DAN SEMUA PELAMAR" << endl;
    cout << "========================================================" << endl;

    if (P == nullptr) {
        cout << "List Lowongan (Parent) kosong. Tidak ada data untuk ditampilkan." << endl;
        return;
    }

    while (P != nullptr) {
        cout << "\n[LOWONGAN ID " << P->info.id_lowongan << "]: " << P->info.posisi
                  << " (Perusahaan: " << P->info.nama_perusahaan << ")" << endl;

        address_relasi R = P->first_relasi;

        if (R == nullptr) {
            cout << "   -> Status: Belum ada pelamar." << endl;
        } else {
            cout << "   -> DETAIL PELAMAR (RELASI):" << endl;
            cout << "   -----------------------------------------------------" << endl;
            while (R != nullptr) {
                address_child C = R->ptr_child;

                cout << "   [Lamaran ID: " << R->info.id_lamaran << "] "
                          << "NIM: " << C->info.nim << " | Nama: " << C->info.nama << endl;
                cout << "      Status Dosen: " << (R->info.status_dosen == 1 ? "Disetujui" : (R->info.status_dosen == 2 ? "Ditolak" : "Menunggu")) << endl;
                cout << "      Status Perusahaan: " << (R->info.status_perusahaan == 1 ? "**DITERIMA**" : (R->info.status_perusahaan == 2 ? "DITOLAK" : "Menunggu")) << endl;

                R = R->next;
            }
        }
        P = P->next;
    }
}
// Stubs (placeholder) untuk fungsi edit dan show lainnya

