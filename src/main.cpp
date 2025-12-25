#include <windows.h>
#include <commdlg.h>
#include "login.h"
#include "lowongan.h"
#include "mahasiswa.h"
#include "lamaran.h"
#include <limits>
#include <iomanip>
#include <cstdio>

using namespace std;

// Enum untuk Role-based Switch Case
enum Role {
    ROLE_MAHASISWA,
    ROLE_DOSEN,
    ROLE_PERUSAHAAN,
    ROLE_ADMIN,
    ROLE_UNKNOWN
};

Role getRoleEnum(string roleStr) {
    if (roleStr == "mahasiswa") return ROLE_MAHASISWA;
    if (roleStr == "dosen") return ROLE_DOSEN;
    if (roleStr == "perusahaan") return ROLE_PERUSAHAAN;
    if (roleStr == "admin") return ROLE_ADMIN;
    return ROLE_UNKNOWN;
}

// Helper function untuk open File Dialog
string OpenFileDialog() {
    OPENFILENAME ofn;
    char szFile[260];
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "PDF Files\0*.pdf\0Text Files\0*.txt\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        return string(ofn.lpstrFile);
    }
    return "";
}

void Cleanup() {
    cout << "Cleaning up temporary files..." << endl;
    
    // 1. Delete search_results.txt
    if (remove("search_results.txt") != 0) {
        // perror("Error deleting search_results.txt"); 
    }

    // 2. Delete all files in uploads directory
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("uploads\\*", &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            const string file_name = findFileData.cFileName;
            const string full_file_name = "uploads\\" + file_name;
            
            if (file_name != "." && file_name != "..") {
                if (DeleteFile(full_file_name.c_str())) {
                    // cout << "Deleted: " << full_file_name << endl;
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
}

int main() {
    ListParent L_Parent;
    ListChild L_Child;
    createListParent(L_Parent);
    createListChild(L_Child);

    // --- MOCK DATA UNTUK TESTING ---
    // Data Lowongan (3 lowongan) - format: ID, Posisi, Perusahaan, IPK Minimum, Kuota
    insertParent(L_Parent, alokasiParent(101, "Data Scientist", "TechCorp", 3.25, 5));
    insertParent(L_Parent, alokasiParent(102, "Mobile Developer", "GameDev Studio", 3.00, 3));
    insertParent(L_Parent, alokasiParent(103, "Backend Engineer", "ServerX", 2.75, 4));
    
    // Data Mahasiswa - dari data kelas (login.cpp)
    insertChild(L_Child, alokasiChild("103032400104", "Muhamad Alwan Suryadi", 2024));
    insertChild(L_Child, alokasiChild("103032400035", "Fathurrahman Alfarizi", 2024));
    insertChild(L_Child, alokasiChild("103032400010", "Azriel Raihan Eldova Hartoto", 2024));
    insertChild(L_Child, alokasiChild("103032400001", "Nathasya Yuan Maharani", 2024));
    insertChild(L_Child, alokasiChild("103032400006", "Theodore Elvis Estrada", 2024));
    insertChild(L_Child, alokasiChild("103032400009", "Dyah Kusuma Wardani", 2024));
    insertChild(L_Child, alokasiChild("103032400022", "Muhammad Ilham Alifianda", 2024));
    insertChild(L_Child, alokasiChild("103032400026", "Alya Aziza Puteri", 2024));
    insertChild(L_Child, alokasiChild("103032400029", "Ahmad Abdan Syakuro", 2024));
    insertChild(L_Child, alokasiChild("103032400040", "Nuswantoro Setyo Mukti", 2024));
    insertChild(L_Child, alokasiChild("103032400041", "Anggit Cahyati Hidayat", 2024));
    insertChild(L_Child, alokasiChild("103032400048", "Wibnu Hijrah Franstio", 2024));
    insertChild(L_Child, alokasiChild("103032400050", "Meysha Primiandita", 2024));
    insertChild(L_Child, alokasiChild("103032400056", "Muhammad Fiqri Habibi", 2024));
    insertChild(L_Child, alokasiChild("103032400060", "Fitri Cahyani", 2024));
    insertChild(L_Child, alokasiChild("103032400062", "Triansyah Daniswara Ibrahim", 2024));
    insertChild(L_Child, alokasiChild("103032400068", "Rakha Abdillah Alkautsar", 2024));
    insertChild(L_Child, alokasiChild("103032400073", "Avicenna Naufal Lathif", 2024));
    insertChild(L_Child, alokasiChild("103032400078", "Nayla Assyifa", 2024));
    insertChild(L_Child, alokasiChild("103032400084", "William Peter Vanxnajoan", 2024));
    insertChild(L_Child, alokasiChild("103032400087", "Rayvan Alifarlo Mahesworo", 2024));
    insertChild(L_Child, alokasiChild("103032400088", "Zaidan Salam Rojab", 2024));
    insertChild(L_Child, alokasiChild("103032400093", "Audrey Frediley Hanas", 2024));
    insertChild(L_Child, alokasiChild("103032400096", "Muhammad Naelfadly", 2024));
    insertChild(L_Child, alokasiChild("103032400100", "Naira Cahaya Putri Darmawan Sinaga", 2024));
    insertChild(L_Child, alokasiChild("103032400107", "Dhafy Ahmad Zubaidi", 2024));
    insertChild(L_Child, alokasiChild("103032400117", "Muhammad Faris Dhiyaylhaq Sarbini", 2024));
    insertChild(L_Child, alokasiChild("103032400123", "Nursyadira", 2024));
    insertChild(L_Child, alokasiChild("103032400124", "Rayfito Krisna Wijaya", 2024));
    insertChild(L_Child, alokasiChild("103032400129", "Mochammad Rafis Qullah", 2024));
    insertChild(L_Child, alokasiChild("103032400134", "I putu Gede Agastyakrisna Widartha", 2024));
    insertChild(L_Child, alokasiChild("103032400137", "Rendil", 2024));
    insertChild(L_Child, alokasiChild("103032400138", "Muhammad Ariq Azzaki", 2024));
    insertChild(L_Child, alokasiChild("103032400155", "Edmund Yulius Gantur", 2024));
    insertChild(L_Child, alokasiChild("103032400157", "Muhammad Sayyid Huwaidi", 2024));
    insertChild(L_Child, alokasiChild("103032400160", "Muh Dzul Jalali Wali Ikram Jalil", 2024));
    insertChild(L_Child, alokasiChild("103032430003", "Ramadhan Tangguh Defennder", 2024));
    insertChild(L_Child, alokasiChild("103032430025", "Adzkiya Putri Rahmawan", 2024));
    insertChild(L_Child, alokasiChild("103032430029", "Fathimah Radhiyya", 2024));
    insertChild(L_Child, alokasiChild("103032430034", "Rakan Ghazian Adi Wijaya", 2024));
    insertChild(L_Child, alokasiChild("103032430037", "Jihan Nabila Mubarakah", 2024));
    // Data Lamaran (relasi) - manual karena butuh struktur khusus
    // Lamaran 1: Alwan -> TechCorp (Data Scientist)
    address_parent P1 = findParent(L_Parent, 101);
    address_child C1 = findChildByNIM(L_Child, "103032400104");
    if (P1 && C1) {
        address_relasi R1 = new NodeRelasi;
        R1->info.id_lamaran = 1;
        R1->info.status_dosen = 1;  // Disetujui dosen
        R1->info.status_perusahaan = 0; // Menunggu
        R1->info.cv_path = "mock_cv_alwan.pdf";
        R1->info.cv_score = 70;
        R1->ptr_parent = P1;
        R1->ptr_child = C1;
        R1->next = nullptr;
        P1->first_relasi = R1;
    }
    
    // Lamaran 2: Fathur -> TechCorp (Data Scientist)
    address_child C2 = findChildByNIM(L_Child, "103032400035");
    if (P1 && C2) {
        address_relasi R2 = new NodeRelasi;
        R2->info.id_lamaran = 2;
        R2->info.status_dosen = 1;
        R2->info.status_perusahaan = 1; // Diterima!
        R2->info.cv_path = "mock_cv_fathur.pdf";
        R2->info.cv_score = 85;
        R2->ptr_parent = P1;
        R2->ptr_child = C2;
        R2->next = nullptr;
        // Insert after R1
        if (P1->first_relasi) {
            address_relasi lastR = P1->first_relasi;
            while (lastR->next) lastR = lastR->next;
            lastR->next = R2;
        }
    }
    
    // Lamaran 3: Azriel -> GameDev (Mobile Dev)
    address_parent P2 = findParent(L_Parent, 102);
    address_child C3 = findChildByNIM(L_Child, "103032400010");
    if (P2 && C3) {
        address_relasi R3 = new NodeRelasi;
        R3->info.id_lamaran = 3;
        R3->info.status_dosen = 0; // Menunggu
        R3->info.status_perusahaan = 0;
        R3->info.cv_path = "mock_cv_azriel.pdf";
        R3->info.cv_score = 60;
        R3->ptr_parent = P2;
        R3->ptr_child = C3;
        R3->next = nullptr;
        P2->first_relasi = R3;
    }
    
    cout << "\033[32m[MOCK DATA] 3 Lowongan, 5 Mahasiswa, 3 Lamaran berhasil dimuat!\033[0m" << endl;
    // --- END MOCK DATA ---

    // --- LOGIN SYSTEM INTEGRATION ---
    initUsers(); // Initialize hardcoded users
    
    // Create uploads directory if not exists
    system("mkdir uploads 2> NUL");

    int counter_lowongan = 107; // Start after hardcoded ones
    int counter_lamaran = 4; // Start after mock data

    while (true) { // Outer loop for Login/Logout
        string activeUser, activeRole, activeNIM;  // Tambah activeNIM
        if (!Login(activeUser, activeRole, activeNIM)) {
            Cleanup();
            cout << "Aplikasi ditutup." << endl;
            return 0;
        }
        // --------------------------------

        int pilihan_menu_utama;

        cout << "========================================" << endl;
        cout << " SISTEM LAYANAN MAGANG (MLL TIPE B) " << endl;
        cout << "========================================" << endl;
        cout << "Selamat Datang, " << activeUser << " (" << activeRole << ")" << endl;

        // Tambahkan beberapa data awal Lowongan (Parent) untuk pengujian Relasi
        // Hardcoded 6 vacancies (sudah diinisiasi di awal main, ini backup jika kosong)
        if (L_Parent.first == nullptr) { 
             insertParent(L_Parent, alokasiParent(101, "Data Scientist", "TechCorp", 3.50, 5));
             insertParent(L_Parent, alokasiParent(102, "Mobile Developer", "GameDev", 3.00, 3));
             insertParent(L_Parent, alokasiParent(103, "Backend Engineer", "ServerX", 3.20, 4));
             insertParent(L_Parent, alokasiParent(104, "UI/UX Designer", "CreativeStudio", 3.00, 2));
             insertParent(L_Parent, alokasiParent(105, "DevOps Engineer", "CloudSys", 3.50, 3));
             insertParent(L_Parent, alokasiParent(106, "Product Manager", "StartUpInc", 3.00, 2));
        }
        
        // Gunakan activeNIM langsung untuk mahasiswa (tidak perlu cari manual lagi)
        string currentNIM = activeNIM;
        
        // AUTO-REGISTER: Jika mahasiswa dengan NIM ini belum ada di L_Child, tambahkan otomatis
        if (activeRole == "mahasiswa" && !currentNIM.empty()) {
            if (findChildByNIM(L_Child, currentNIM) == nullptr) {
                insertChild(L_Child, alokasiChild(currentNIM, activeUser, 2024));
                cout << "\033[32m[AUTO] Data mahasiswa otomatis ditambahkan ke sistem.\033[0m" << endl;
                Loading(1000);
            }
        }
        
        while (true) {
            int notifCount = 0;
            if (activeRole == "mahasiswa" && !currentNIM.empty()) {
                notifCount = countNotifikasi(L_Parent, currentNIM);
            }

            cout << "\n";
            cout << "\033[36m"; // Cyan color
            cout << "+============================================+" << endl;
            cout << "|           MENU UTAMA (" << activeRole << ")" << endl;
            cout << "+============================================+" << endl;
            cout << "\033[0m"; // Reset color
            
            // Display Menu based on Role using Switch
            switch (getRoleEnum(activeRole)) {
                case ROLE_MAHASISWA:
                    cout << "\033[33m"; // Yellow
                    cout << "| [1] " << char(175) << " Lihat Daftar Lowongan        |" << endl;
                    cout << "| [2] " << char(175) << " Ajukan Lamaran (Upload CV)   |" << endl;
                    cout << "| [3] " << char(175) << " Cek Status Lamaran           |" << endl;
                    cout << "| [4] " << char(175) << " Cari Lowongan (by ID)        |" << endl;
                    cout << "| [5] " << char(175) << " Batalkan Lamaran             |" << endl;
                    cout << "| [6] " << char(175) << " Pesan (" << notifCount << ")                    |" << endl;
                    cout << "\033[31m"; // Red
                    cout << "| [0] " << char(174) << " Keluar (Logout)              |" << endl;
                    cout << "\033[0m";
                    break;
                case ROLE_DOSEN:
                    cout << "\033[33m";
                    cout << "| [1] " << char(175) << " Verifikasi Lamaran           |" << endl;
                    cout << "| [2] " << char(175) << " Lihat Daftar Lowongan        |" << endl;
                    cout << "| [3] " << char(175) << " Rekap Lamaran Mahasiswa      |" << endl;
                    cout << "| [4] " << char(175) << " Cari Mahasiswa (by NIM)      |" << endl;
                    cout << "| [5] " << char(175) << " Hapus Mahasiswa (by NIM)     |" << endl;
                    cout << "| [6] " << char(175) << " Cek Relasi Lowongan-Mhs      |" << endl;
                    cout << "| [7] " << char(175) << " Lihat Semua Mhs + Lowongan   |" << endl;
                    cout << "| [8] " << char(175) << " Hitung Lowongan per Mhs      |" << endl;
                    cout << "| [9] " << char(175) << " Mhs Belum Melamar            |" << endl;
                    cout << "\033[31m";
                    cout << "| [0] " << char(174) << " Keluar (Logout)              |" << endl;
                    cout << "\033[0m";
                    break;
                case ROLE_PERUSAHAAN:
                    cout << "\033[33m";
                    cout << "| [1] " << char(175) << " Input Lowongan Baru          |" << endl;
                    cout << "| [2] " << char(175) << " Berikan Keputusan Lamaran    |" << endl;
                    cout << "| [3] " << char(175) << " Rekap Lamaran (ATS Score)    |" << endl;
                    cout << "| [4] " << char(175) << " Cari Lowongan Online (API)   |" << endl;
                    cout << "| [5] " << char(175) << " Cari Mahasiswa (by NIM)      |" << endl;
                    cout << "| [6] " << char(175) << " Hapus Lowongan (by ID)       |" << endl;
                    cout << "| [7] " << char(175) << " Cek Relasi Lowongan-Mhs      |" << endl;
                    cout << "| [8] " << char(175) << " Hitung Pelamar per Lowongan  |" << endl;
                    cout << "| [9] " << char(175) << " Lowongan Tanpa Pelamar       |" << endl;
                    cout << "\033[31m";
                    cout << "| [0] " << char(174) << " Keluar (Logout)              |" << endl;
                    cout << "\033[0m";
                    break;
                case ROLE_ADMIN:
                    PrintAdminHeader(); // Tampilkan ASCII Art ADMIN merah
                    cout << "\033[33m";
                    cout << "| === KELOLA PENGGUNA ===               |" << endl;
                    cout << "| [1] " << char(175) << " Tambah Pengguna Baru         |" << endl;
                    cout << "| [2] " << char(175) << " Hapus Pengguna               |" << endl;
                    cout << "| [3] " << char(175) << " Ubah Data Pengguna           |" << endl;
                    cout << "| [4] " << char(175) << " Cetak Daftar Pengguna        |" << endl;
                    cout << "| [5] " << char(175) << " Reset Password Pengguna      |" << endl;
                    cout << "| [6] " << char(175) << " Ganti Password Admin         |" << endl;
                    cout << "| [7] " << char(175) << " Lihat Riwayat Admin          |" << endl;
                    cout << "| === KELOLA DATA ===                   |" << endl;
                    cout << "| [8] " << char(175) << " Lihat Semua Data (M:N)       |" << endl;
                    cout << "| [9] " << char(175) << " Rekap Semua Lamaran          |" << endl;
                    cout << "| [10]" << char(175) << " Cari Mahasiswa (by NIM)      |" << endl;
                    cout << "| [11]" << char(175) << " Cari Lowongan (by ID)        |" << endl;
                    cout << "| [12]" << char(175) << " Hapus Mahasiswa (by NIM)     |" << endl;
                    cout << "| [13]" << char(175) << " Hapus Lowongan (by ID)       |" << endl;
                    cout << "| [14]" << char(175) << " Batalkan/Hapus Lamaran       |" << endl;
                    cout << "| [15]" << char(175) << " Cek Relasi Lowongan-Mhs      |" << endl;
                    cout << "| [16]" << char(175) << " Lihat Semua Mhs + Lowongan   |" << endl;
                    cout << "| [17]" << char(175) << " Statistik Lengkap            |" << endl;
                    cout << "| [18]" << char(175) << " Edit Relasi                  |" << endl;
                    cout << "\033[31m";
                    cout << "| [0] " << char(174) << " Keluar (Logout)              |" << endl;
                    cout << "\033[0m";
                    break;
                default:
                    cout << "Role tidak dikenali." << endl;
                    break;
            }
            cout << "\033[36m";
            cout << "+============================================+" << endl;
            cout << "\033[0m";

            cout << "Pilihan Anda: ";
            if (!(cin >> pilihan_menu_utama)) {
                cout << "Input tidak valid." << endl;
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (pilihan_menu_utama == 0) {
                cout << "Logging out..." << endl;
                // Fix buffer issue: Clear buffer before returning to Login (which uses getline)
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                Loading(1000);
                break; // Breaks the inner loop, goes back to Login
            }

            // --- LOGIKA MENU BERDASARKAN ROLE ---
            
            switch (getRoleEnum(activeRole)) {
                case ROLE_MAHASISWA: {
                    // currentNIM sudah didapat dari login (activeNIM), tidak perlu cari lagi
                    
                    switch (pilihan_menu_utama) {
                        case 1: { // Lihat Daftar Lowongan
                            showLowongan(L_Parent);
                            break;
                        }
                        case 2: { // Ajukan Lamaran + CV (PDF Picker)
                            if (currentNIM.empty()) {
                                cout << "NIM tidak ditemukan. Pastikan Anda login dengan akun yang memiliki NIM." << endl;
                                break;
                            }
                            
                            int id_lowongan_input;
                            string cv_path;
                            
                            cout << "--- AJUKAN LAMARAN ---" << endl;
                            cout << "NIM Anda: " << currentNIM << " (Otomatis)" << endl;
                            cout << "1. Pilih File CV (PDF/TXT) akan terbuka otomatis..." << endl;
                            system("pause"); 
                            
                            cv_path = OpenFileDialog();
                            if (cv_path.empty()) {
                                cout << "Tidak ada file yang dipilih. Lamaran dibatalkan." << endl;
                                break;
                            }
                            cout << "File terpilih: " << cv_path << endl;

                            cout << "Masukkan ID Lowongan: "; cin >> id_lowongan_input;
                            cout << "Nama Lengkap: " << activeUser << " (Otomatis)" << endl;
                            
                            insertRelasi(L_Parent, L_Child, id_lowongan_input, currentNIM, activeUser, counter_lamaran++, cv_path);
                            break;
                        }
                        case 3: { // Status Lamaran - OTOMATIS
                            if (currentNIM.empty()) {
                                cout << "NIM tidak ditemukan. Pastikan Anda login dengan akun yang memiliki NIM." << endl;
                                break;
                            }
                            cout << "\n--- STATUS LAMARAN ---" << endl;
                            cout << "NIM Anda: " << currentNIM << " (Otomatis)" << endl;
                            showStatusLamaranMahasiswa(L_Parent, currentNIM);
                            break;
                        }
                        case 4: { // Cari Lowongan (by ID)
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            menuCariLowongan(L_Parent);
                            break;
                        }
                        case 5: { // Batalkan Lamaran
                            if (currentNIM.empty()) {
                                cout << "NIM tidak ditemukan. Pastikan Anda login dengan akun yang memiliki NIM." << endl;
                                break;
                            }
                            
                            // Tampilkan daftar lamaran milik mahasiswa ini
                            showLamaranMahasiswaUntukHapus(L_Parent, currentNIM);
                            
                            int id_batal;
                            cout << "\nMasukkan ID Lamaran yang ingin dibatalkan (0 untuk batal): ";
                            cin >> id_batal;
                            
                            if (id_batal == 0) {
                                cout << "Pembatalan dibatalkan." << endl;
                                break;
                            }
                            
                            // Gunakan fungsi yang memvalidasi kepemilikan
                            deleteRelasiMahasiswa(L_Parent, id_batal, currentNIM);
                            break;
                        }
                        case 6: { // Pesan / Notifikasi
                            if (currentNIM.empty()) {
                                cout << "NIM tidak ditemukan. Pastikan Anda login dengan akun yang memiliki NIM." << endl;
                            } else {
                                showNotifikasi(L_Parent, currentNIM);
                            }
                            system("pause");
                            break;
                        }
                        default: cout << "Pilihan tidak valid." << endl;
                    }
                    break;
                } 
                case ROLE_DOSEN: {
                    switch (pilihan_menu_utama) {
                        case 1: { // Verifikasi
                            int id_lamaran_target, status_baru;
                            cout << "Masukkan ID Lamaran (BUKAN ID Lowongan): "; cin >> id_lamaran_target;
                            cout << "Status Verifikasi (1=DISETUJUI, 2=DITOLAK): "; cin >> status_baru;
                            editStatusDosen(L_Parent, id_lamaran_target, status_baru);
                            break;
                        }
                        case 2: { // Lihat Lowongan
                            showLowongan(L_Parent);
                            break;
                        }
                        case 3: { // Rekap Lamaran
                            showRekapLamaranDosen(L_Parent);
                            system("pause");
                            break;
                        }
                        case 4: { // Cari Mahasiswa (by NIM)
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            menuCariMahasiswa(L_Child);
                            break;
                        }
                        case 5: { // Hapus Mahasiswa (by NIM)
                            string nim_hapus;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Masukkan NIM Mahasiswa yang akan dihapus: ";
                            getline(cin, nim_hapus);
                            deleteChildByNIM(L_Child, L_Parent, nim_hapus);
                            break;
                        }
                        case 6: { // Cek Relasi Parent-Child
                            int id_low;
                            string nim_cek;
                            cout << "Masukkan ID Lowongan: "; cin >> id_low;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Masukkan NIM Mahasiswa: "; getline(cin, nim_cek);
                            findRelasi(L_Parent, id_low, nim_cek);
                            break;
                        }
                        case 7: { // Lihat Semua Mhs + Lowongan
                            showAllChildWithParent(L_Parent, L_Child);
                            system("pause");
                            break;
                        }
                        case 8: { // Hitung Lowongan per Mhs
                            string nim_count;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Masukkan NIM Mahasiswa: "; getline(cin, nim_count);
                            countParentPerChild(L_Parent, nim_count);
                            break;
                        }
                        case 9: { // Mhs Belum Melamar
                            countChildWithoutRelasi(L_Parent, L_Child);
                            system("pause");
                            break;
                        }
                        default: cout << "Pilihan tidak valid." << endl;
                    }
                    break;
                }
                case ROLE_PERUSAHAAN: {
                    switch (pilihan_menu_utama) {
                        case 1: { // Input Lowongan
                            int pilihan_sub;
                            do {
                                menuInsertParent(L_Parent, counter_lowongan);
                                cout << "1. Input Lagi, 2. Kembali: "; cin >> pilihan_sub;
                            } while (pilihan_sub == 1);
                            break;
                        }
                        case 2: { // Keputusan
                            int id_lamaran_target;
                            cout << "Masukkan ID Lamaran (BUKAN ID Lowongan): "; cin >> id_lamaran_target;
                            // Status input moved inside the function for better UX (CV Review)
                            editStatusPerusahaan(L_Parent, id_lamaran_target);
                            break;
                        }
                        case 3: { // Rekap
                            showRekapLamaranPerusahaan(L_Parent);
                            break;
                        }
                        case 4: { // Cari Lowongan Online (C++ Native)
                            string keyword;
                            cout << "Masukkan Kata Kunci Lowongan (misal: Java, Marketing): ";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            getline(cin, keyword);

                            // Langsung panggil fungsi C++ (tidak perlu Python lagi)
                            searchJobsOnline(L_Parent, keyword);
                            break;
                        }
                        case 5: { // Cari Mahasiswa (by NIM)
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            menuCariMahasiswa(L_Child);
                            break;
                        }
                        case 6: { // Hapus Lowongan (by ID)
                            int id_hapus;
                            cout << "Masukkan ID Lowongan yang akan dihapus: ";
                            cin >> id_hapus;
                            deleteParentByID(L_Parent, id_hapus);
                            break;
                        }
                        case 7: { // Cek Relasi Parent-Child
                            int id_low;
                            string nim_cek;
                            cout << "Masukkan ID Lowongan: "; cin >> id_low;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Masukkan NIM Mahasiswa: "; getline(cin, nim_cek);
                            findRelasi(L_Parent, id_low, nim_cek);
                            break;
                        }
                        case 8: { // Hitung Pelamar per Lowongan
                            int id_count;
                            cout << "Masukkan ID Lowongan: "; cin >> id_count;
                            countChildPerParent(L_Parent, id_count);
                            break;
                        }
                        case 9: { // Lowongan Tanpa Pelamar
                            countParentWithoutRelasi(L_Parent);
                            system("pause");
                            break;
                        }
                        default: cout << "Pilihan tidak valid." << endl;
                    }
                    break;
                }
                case ROLE_ADMIN: {
                    switch (pilihan_menu_utama) {
                        case 1: { // Tambah Pengguna Baru
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            TambahPenggunaBaru("admin");
                            break;
                        }
                        case 2: { // Hapus Pengguna
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            HapusPengguna("admin");
                            break;
                        }
                        case 3: { // Ubah Data Pengguna
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            UbahDataPengguna("admin");
                            break;
                        }
                        case 4: { // Cetak Daftar Pengguna
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            CetakPengguna();
                            break;
                        }
                        case 5: { // Reset Password Pengguna
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            ResetPasswordPengguna("admin");
                            break;
                        }
                        case 6: { // Ganti Password Admin
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            GantiPasswordAdmin("admin");
                            break;
                        }
                        case 7: { // Lihat Riwayat Admin
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            LihatRiwayatAdmin();
                            break;
                        }
                        case 8: { // Lihat Semua Data (M:N)
                            showLowonganDanPelamar(L_Parent);
                            CatatRiwayatAdmin("admin", "Melihat semua data (M:N)");
                            break;
                        }
                        case 9: { // Rekap Semua Lamaran
                            showRekapLamaranPerusahaan(L_Parent);
                            CatatRiwayatAdmin("admin", "Melihat rekap semua lamaran");
                            break;
                        }
                        case 10: { // Cari Mahasiswa (by NIM)
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            menuCariMahasiswa(L_Child);
                            CatatRiwayatAdmin("admin", "Mencari mahasiswa");
                            break;
                        }
                        case 11: { // Cari Lowongan (by ID)
                            menuCariLowongan(L_Parent);
                            CatatRiwayatAdmin("admin", "Mencari lowongan");
                            break;
                        }
                        case 12: { // Hapus Mahasiswa (by NIM)
                            string nim_hapus;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Masukkan NIM Mahasiswa yang akan dihapus: ";
                            getline(cin, nim_hapus);
                            deleteChildByNIM(L_Child, L_Parent, nim_hapus);
                            CatatRiwayatAdmin("admin", "Menghapus mahasiswa NIM: " + nim_hapus);
                            break;
                        }
                        case 13: { // Hapus Lowongan (by ID)
                            int id_hapus;
                            cout << "Masukkan ID Lowongan yang akan dihapus: ";
                            cin >> id_hapus;
                            deleteParentByID(L_Parent, id_hapus);
                            CatatRiwayatAdmin("admin", "Menghapus lowongan ID: " + to_string(id_hapus));
                            break;
                        }
                        case 14: { // Batalkan/Hapus Lamaran
                            int id_batal;
                            cout << "Masukkan ID Lamaran yang ingin dihapus: ";
                            cin >> id_batal;
                            deleteRelasi(L_Parent, id_batal);
                            CatatRiwayatAdmin("admin", "Menghapus lamaran ID: " + to_string(id_batal));
                            break;
                        }
                        case 15: { // Cek Relasi Parent-Child
                            int id_low;
                            string nim_cek;
                            cout << "Masukkan ID Lowongan: "; cin >> id_low;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Masukkan NIM Mahasiswa: "; getline(cin, nim_cek);
                            findRelasi(L_Parent, id_low, nim_cek);
                            CatatRiwayatAdmin("admin", "Cek relasi lowongan ID: " + to_string(id_low) + " - NIM: " + nim_cek);
                            break;
                        }
                        case 16: { // Lihat Semua Mhs + Lowongan
                            showAllChildWithParent(L_Parent, L_Child);
                            CatatRiwayatAdmin("admin", "Melihat semua mahasiswa + lowongan");
                            system("pause");
                            break;
                        }
                        case 17: { // Statistik Lengkap
                            cout << "\n\033[36m"; // Cyan
                            cout << "+============================================+" << endl;
                            cout << "|        \033[33mSTATISTIK LENGKAP SISTEM\033[36m           |" << endl;
                            cout << "+============================================+\033[0m" << endl;
                            
                            // Section 1: Pelamar per Lowongan
                            cout << "\n\033[36m+--------------------------------------------+\033[0m" << endl;
                            cout << "| \033[92m[1] PELAMAR PER LOWONGAN\033[0m" << endl;
                            cout << "\033[36m+--------------------------------------------+\033[0m" << endl;
                            address_parent P = L_Parent.first;
                            while (P != nullptr) {
                                countChildPerParent(L_Parent, P->info.id_lowongan);
                                P = P->next;
                            }
                            
                            // Section 2: Lowongan per Mahasiswa
                            cout << "\n\033[36m+--------------------------------------------+\033[0m" << endl;
                            cout << "| \033[93m[2] LOWONGAN PER MAHASISWA\033[0m" << endl;
                            cout << "\033[36m+--------------------------------------------+\033[0m" << endl;
                            address_child C = L_Child.first;
                            while (C != nullptr) {
                                countParentPerChild(L_Parent, C->info.nim);
                                C = C->next;
                            }
                            
                            // Section 3: Mahasiswa Belum Melamar
                            cout << "\n\033[36m+--------------------------------------------+\033[0m" << endl;
                            cout << "| \033[91m[3] MAHASISWA BELUM MELAMAR\033[0m" << endl;
                            cout << "\033[36m+--------------------------------------------+\033[0m" << endl;
                            countChildWithoutRelasi(L_Parent, L_Child);
                            
                            // Section 4: Lowongan Tanpa Pelamar
                            cout << "\n\033[36m+--------------------------------------------+\033[0m" << endl;
                            cout << "| \033[95m[4] LOWONGAN TANPA PELAMAR\033[0m" << endl;
                            cout << "\033[36m+--------------------------------------------+\033[0m" << endl;
                            countParentWithoutRelasi(L_Parent);
                            
                            cout << "\n\033[36m+============================================+\033[0m" << endl;
                            CatatRiwayatAdmin("admin", "Melihat statistik lengkap");
                            system("pause");
                            break;
                        }
                        case 18: { // Edit Relasi
                            int id_edit;
                            cout << "Masukkan ID Lamaran yang ingin diedit: ";
                            cin >> id_edit;
                            editRelasi(L_Parent, L_Child, id_edit);
                            CatatRiwayatAdmin("admin", "Edit relasi lamaran ID: " + to_string(id_edit));
                            break;
                        }
                        default: cout << "Pilihan tidak valid." << endl;
                    }
                    break;
                }
                default:
                    cout << "Role tidak valid." << endl;
                    break;
            }

        }
    }
    
    Cleanup(); // Cleanup at end of main (jaga-jaga)

    return 0;
}
