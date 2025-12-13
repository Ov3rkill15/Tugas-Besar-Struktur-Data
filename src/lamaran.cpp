#include <windows.h> // For CopyFile
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

// Helper to get file extension
string getExtension(string path) {
    size_t dot = path.find_last_of(".");
    if (dot != string::npos) return path.substr(dot);
    return "";
}

// Helper function to "Upload" file (Copy to uploads folder)
string uploadFile(string sourcePath, string nim, int id_lowongan) {
    string ext = getExtension(sourcePath);
    if (ext.empty()) ext = ".pdf"; // Default to pdf if unknown

    // Create unique filename: uploads/CV_NIM_ID.pdf
    string destPath = "uploads\\CV_" + nim + "_" + to_string(id_lowongan) + ext;
    
    // Copy file
    if (CopyFile(sourcePath.c_str(), destPath.c_str(), FALSE)) {
        return destPath;
    } else {
        return ""; // Failed
    }
}

// 1. Insert element relation (Mahasiswa Ajukan Lamaran)
void insertRelasi(ListParent &L_Parent, ListChild &L_Child, int ID_Lowongan, string NIM_Mhs, string Nama_Mhs, int ID_Lamaran_Baru, string CV_Path) {
    // 1. Pastikan data Mahasiswa ada/dibuat di List Child
    handleInputMahasiswa(L_Child, NIM_Mhs, Nama_Mhs);

    // 2. Cari Node Parent dan Child
    address_parent P_Parent = findParent(L_Parent, ID_Lowongan);
    address_child P_Child = findChildByNIM(L_Child, NIM_Mhs);

    if (P_Parent != nullptr && P_Child != nullptr) {
        // --- UPLOAD PROCESS ---
        string uploadedPath = uploadFile(CV_Path, NIM_Mhs, ID_Lowongan);
        if (uploadedPath.empty()) {
            cout << " [ERROR] Gagal mengupload file CV. Pastikan file ada." << endl;
            return;
        }
        // ----------------------

        address_relasi R_Baru = new NodeRelasi;
        R_Baru->info.id_lamaran = ID_Lamaran_Baru;
        R_Baru->info.status_dosen = 0; // Menunggu
        R_Baru->info.status_perusahaan = 0; // Menunggu
        R_Baru->info.cv_path = uploadedPath; // Store the SERVER path
        R_Baru->info.cv_score = hitungSkorATS(uploadedPath); // Score the UPLOADED file

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
        cout << " [UPLOAD] File berhasil disimpan ke: " << uploadedPath << endl;
        cout << " [ATS] Skor awal: " << R_Baru->info.cv_score << endl;
    } else {
        cout << " Error: Lowongan ID " << ID_Lowongan << " tidak ditemukan. Lamaran dibatalkan." << endl;
    }
}
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
                cout << "   - [ID LAMARAN: " << R->info.id_lamaran << "] " << endl;
                cout << "     NIM: " << C->info.nim << " | Nama: " << C->info.nama << endl;
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
void editStatusPerusahaan(ListParent &L_Parent, int ID_Lamaran_Target) {
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

        // --- INTEGRASI REVIEW CV ---
        cout << "\n--- DETAIL PELAMAR ---" << endl;
        cout << "Nama: " << R_Target->ptr_child->info.nama << endl;
        cout << "NIM : " << R_Target->ptr_child->info.nim << endl;
        cout << "File CV : " << R_Target->info.cv_path << endl;
        cout << "Skor ATS: " << R_Target->info.cv_score << endl;
        
        char openCv;
        cout << "\nApakah Anda ingin membuka file CV? (y/n): ";
        cin >> openCv;
        
        if (openCv == 'y' || openCv == 'Y') {
            string command = "start \"\" \"" + R_Target->info.cv_path + "\"";
            cout << "Membuka file..." << endl;
            system(command.c_str());
        }
        // ---------------------------

        int Status_Baru;
        cout << "\nStatus Keputusan (1=DITERIMA, 2=DITOLAK): ";
        cin >> Status_Baru;

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

void showRekapLamaranDosen(ListParent L_Parent) {
    address_parent P = L_Parent.first;

    cout << "\n========================================================" << endl;
    cout << "REKAP LAMARAN MAHASISWA (DOSEN VIEW)" << endl;
    cout << "========================================================" << endl;

    if (P == nullptr) {
        cout << "Belum ada lowongan/lamaran." << endl;
        return;
    }

    bool adaLamaran = false;
    while (P != nullptr) {
        address_relasi R = P->first_relasi;
        while (R != nullptr) {
            adaLamaran = true;
            address_child C = R->ptr_child;
            
            cout << "[ID LAMARAN: " << R->info.id_lamaran << "] " << endl;
            cout << "   Mahasiswa : " << C->info.nama << " (" << C->info.nim << ")" << endl;
            cout << "   Perusahaan: " << P->info.nama_perusahaan << " - " << P->info.posisi << endl;
            cout << "   Status Dosen     : " << (R->info.status_dosen == 1 ? "DISETUJUI" : (R->info.status_dosen == 2 ? "DITOLAK" : "Menunggu")) << endl;
            cout << "   Status Perusahaan: " << (R->info.status_perusahaan == 1 ? "DITERIMA" : (R->info.status_perusahaan == 2 ? "DITOLAK" : "Menunggu")) << endl;
            cout << "--------------------------------------------------------" << endl;

            R = R->next;
        }
        P = P->next;
    }

    if (!adaLamaran) {
        cout << "Tidak ada data lamaran mahasiswa." << endl;
    }
}

int countNotifikasi(ListParent L_Parent, string NIM) {
    int count = 0;
    address_parent P = L_Parent.first;
    while (P != nullptr) {
        address_relasi R = P->first_relasi;
        while (R != nullptr) {
            if (R->ptr_child->info.nim == NIM && R->info.status_perusahaan == 1) {
                count++;
            }
            R = R->next;
        }
        P = P->next;
    }
    return count;
}

void showNotifikasi(ListParent L_Parent, string NIM) {
    cout << "\n========================================================" << endl;
    cout << "KOTAK MASUK (PESAN DITERIMA KERJA)" << endl;
    cout << "========================================================" << endl;

    int count = 0;
    address_parent P = L_Parent.first;
    while (P != nullptr) {
        address_relasi R = P->first_relasi;
        while (R != nullptr) {
            if (R->ptr_child->info.nim == NIM && R->info.status_perusahaan == 1) {
                count++;
                cout << count << ". SELAMAT! Lamaran Anda diterima." << endl;
                cout << "   Perusahaan: " << P->info.nama_perusahaan << endl;
                cout << "   Posisi    : " << P->info.posisi << endl;
                cout << "   Segera hubungi perusahaan untuk proses onboarding." << endl;
                cout << "--------------------------------------------------------" << endl;
            }
            R = R->next;
        }
        P = P->next;
    }

    if (count == 0) {
        cout << "Belum ada pesan baru (Belum ada lamaran yang diterima)." << endl;
    }
}

// ==================================================================================
// FUNGSI BARU MLL TIPE B (Requirement f, i, o, p, q, r, s, t)
// ==================================================================================

// f. Delete element relation - Hapus lamaran berdasarkan ID
void deleteRelasi(ListParent &L_Parent, int ID_Lamaran_Target) {
    address_parent P = L_Parent.first;
    
    while (P != nullptr) {
        address_relasi R_Before = nullptr;
        address_relasi R_Current = P->first_relasi;
        
        while (R_Current != nullptr) {
            if (R_Current->info.id_lamaran == ID_Lamaran_Target) {
                // Found the relation to delete
                if (R_Before == nullptr) {
                    // Hapus node pertama
                    P->first_relasi = R_Current->next;
                } else {
                    // Hapus node tengah/akhir
                    R_Before->next = R_Current->next;
                }
                cout << " Lamaran ID " << ID_Lamaran_Target << " berhasil dihapus/dibatalkan." << endl;
                delete R_Current;
                return;
            }
            R_Before = R_Current;
            R_Current = R_Current->next;
        }
        P = P->next;
    }
    cout << " Lamaran ID " << ID_Lamaran_Target << " tidak ditemukan." << endl;
}

// i. Find apakah parent dan child tertentu memiliki relasi
bool findRelasi(ListParent L_Parent, int ID_Lowongan, string NIM_Mhs) {
    address_parent P = findParent(L_Parent, ID_Lowongan);
    
    if (P == nullptr) {
        cout << " Lowongan ID " << ID_Lowongan << " tidak ditemukan." << endl;
        return false;
    }
    
    address_relasi R = P->first_relasi;
    while (R != nullptr) {
        if (R->ptr_child->info.nim == NIM_Mhs) {
            cout << " DITEMUKAN! Mahasiswa " << NIM_Mhs << " sudah melamar ke Lowongan ID " << ID_Lowongan << "." << endl;
            cout << "   ID Lamaran: " << R->info.id_lamaran << endl;
            cout << "   Status Dosen: " << (R->info.status_dosen == 1 ? "Disetujui" : (R->info.status_dosen == 2 ? "Ditolak" : "Menunggu")) << endl;
            cout << "   Status Perusahaan: " << (R->info.status_perusahaan == 1 ? "Diterima" : (R->info.status_perusahaan == 2 ? "Ditolak" : "Menunggu")) << endl;
            return true;
        }
        R = R->next;
    }
    cout << " Mahasiswa " << NIM_Mhs << " BELUM melamar ke Lowongan ID " << ID_Lowongan << "." << endl;
    return false;
}

// o. Show setiap data child beserta data parent yang berelasi dengannya
void showAllChildWithParent(ListParent L_Parent, ListChild L_Child) {
    cout << "\n========================================================" << endl;
    cout << "DAFTAR SEMUA MAHASISWA DAN LOWONGAN YANG DILAMAR" << endl;
    cout << "========================================================" << endl;
    
    address_child C = L_Child.first;
    if (C == nullptr) {
        cout << "List Mahasiswa kosong." << endl;
        return;
    }
    
    while (C != nullptr) {
        cout << "\n[MAHASISWA] NIM: " << C->info.nim << " | Nama: " << C->info.nama << endl;
        
        int count = 0;
        address_parent P = L_Parent.first;
        while (P != nullptr) {
            address_relasi R = P->first_relasi;
            while (R != nullptr) {
                if (R->ptr_child->info.nim == C->info.nim) {
                    count++;
                    cout << "   -> [Lamaran ID " << R->info.id_lamaran << "] " 
                         << P->info.posisi << " (" << P->info.nama_perusahaan << ")" << endl;
                    cout << "      Status: Dosen=" << (R->info.status_dosen == 1 ? "OK" : (R->info.status_dosen == 2 ? "X" : "-"))
                         << ", Perusahaan=" << (R->info.status_perusahaan == 1 ? "DITERIMA" : (R->info.status_perusahaan == 2 ? "DITOLAK" : "-")) << endl;
                }
                R = R->next;
            }
            P = P->next;
        }
        
        if (count == 0) {
            cout << "   -> Belum melamar ke lowongan manapun." << endl;
        }
        C = C->next;
    }
}

// p. Count jumlah child element parent tertentu
int countChildPerParent(ListParent L_Parent, int ID_Lowongan) {
    address_parent P = findParent(L_Parent, ID_Lowongan);
    if (P == nullptr) {
        cout << " Lowongan ID " << ID_Lowongan << " tidak ditemukan." << endl;
        return -1;
    }
    
    int count = 0;
    address_relasi R = P->first_relasi;
    while (R != nullptr) {
        count++;
        R = R->next;
    }
    
    cout << " Lowongan ID " << ID_Lowongan << " (" << P->info.posisi << ") memiliki " << count << " pelamar." << endl;
    return count;
}

// q. Count jumlah parent yang dimiliki oleh child tertentu
int countParentPerChild(ListParent L_Parent, string NIM_Target) {
    int count = 0;
    
    address_parent P = L_Parent.first;
    while (P != nullptr) {
        address_relasi R = P->first_relasi;
        while (R != nullptr) {
            if (R->ptr_child->info.nim == NIM_Target) {
                count++;
            }
            R = R->next;
        }
        P = P->next;
    }
    
    cout << " Mahasiswa NIM " << NIM_Target << " telah melamar ke " << count << " lowongan." << endl;
    return count;
}

// r. Count element child yang tidak memiliki parent (belum melamar)
int countChildWithoutRelasi(ListParent L_Parent, ListChild L_Child) {
    int count = 0;
    
    address_child C = L_Child.first;
    while (C != nullptr) {
        bool hasRelasi = false;
        
        address_parent P = L_Parent.first;
        while (P != nullptr && !hasRelasi) {
            address_relasi R = P->first_relasi;
            while (R != nullptr) {
                if (R->ptr_child->info.nim == C->info.nim) {
                    hasRelasi = true;
                    break;
                }
                R = R->next;
            }
            P = P->next;
        }
        
        if (!hasRelasi) {
            count++;
            cout << " - " << C->info.nim << " (" << C->info.nama << ") belum melamar." << endl;
        }
        C = C->next;
    }
    
    cout << "\n Total mahasiswa belum melamar: " << count << endl;
    return count;
}

// s. Count element parent yang tidak memiliki child (lowongan tanpa pelamar)
int countParentWithoutRelasi(ListParent L_Parent) {
    int count = 0;
    
    address_parent P = L_Parent.first;
    while (P != nullptr) {
        if (P->first_relasi == nullptr) {
            count++;
            cout << " - Lowongan ID " << P->info.id_lowongan << " (" << P->info.posisi << ") belum ada pelamar." << endl;
        }
        P = P->next;
    }
    
    cout << "\n Total lowongan tanpa pelamar: " << count << endl;
    return count;
}

// t. Edit relasi - Ganti child dari parent tertentu
void editRelasi(ListParent &L_Parent, ListChild L_Child, int ID_Lamaran_Target) {
    address_parent P = L_Parent.first;
    address_relasi R_Target = nullptr;
    
    // Cari relasi yang akan diedit
    while (P != nullptr && R_Target == nullptr) {
        address_relasi R = P->first_relasi;
        while (R != nullptr) {
            if (R->info.id_lamaran == ID_Lamaran_Target) {
                R_Target = R;
                break;
            }
            R = R->next;
        }
        P = P->next;
    }
    
    if (R_Target == nullptr) {
        cout << " Lamaran ID " << ID_Lamaran_Target << " tidak ditemukan." << endl;
        return;
    }
    
    cout << "\n--- EDIT RELASI (Lamaran ID: " << ID_Lamaran_Target << ") ---" << endl;
    cout << "Data saat ini:" << endl;
    cout << "  Mahasiswa: " << R_Target->ptr_child->info.nim << " (" << R_Target->ptr_child->info.nama << ")" << endl;
    cout << "  Lowongan : " << R_Target->ptr_parent->info.posisi << " (" << R_Target->ptr_parent->info.nama_perusahaan << ")" << endl;
    
    cout << "\nPilih yang ingin diubah:" << endl;
    cout << "1. Ganti Mahasiswa (Child)" << endl;
    cout << "2. Batal" << endl;
    cout << "Pilihan: ";
    
    int pilihan;
    cin >> pilihan;
    
    if (pilihan == 1) {
        string nim_baru;
        cout << "Masukkan NIM Mahasiswa baru: ";
        cin.ignore();
        getline(cin, nim_baru);
        
        address_child C_Baru = findChildByNIM(L_Child, nim_baru);
        if (C_Baru == nullptr) {
            cout << " Mahasiswa dengan NIM " << nim_baru << " tidak ditemukan di sistem." << endl;
            return;
        }
        
        R_Target->ptr_child = C_Baru;
        cout << " Berhasil! Lamaran ID " << ID_Lamaran_Target << " sekarang milik " << C_Baru->info.nama << " (" << nim_baru << ")." << endl;
    } else {
        cout << " Edit dibatalkan." << endl;
    }
}
