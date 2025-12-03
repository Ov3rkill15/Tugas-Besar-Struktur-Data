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

// Enum for Role-based Switch Case
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

// Helper function to open File Dialog
string OpenFileDialog() {
    OPENFILENAME ofn;
    char szFile[260];
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "PDF Files\0*.pdf\0Text Files\0*.txt\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        return string(ofn.lpstrFile);
    }
    return "";
}

// --- FUNGSI CLEANUP ---
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

// --- FUNGSI UTAMA ---
int main() {
    ListParent L_Parent;
    ListChild L_Child;
    createListParent(L_Parent);
    createListChild(L_Child);

    // --- LOGIN SYSTEM INTEGRATION ---
    initUsers(); // Initialize hardcoded users
    
    // Create uploads directory if not exists
    system("mkdir uploads 2> NUL");

    int counter_lowongan = 107; // Start after hardcoded ones
    int counter_lamaran = 1;

    while (true) { // Outer loop for Login/Logout
        string activeUser, activeRole;
        if (!Login(activeUser, activeRole)) {
            Cleanup(); // <--- CALL CLEANUP HERE
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
        // Hardcoded 5+ vacancies as requested
        if (L_Parent.first == nullptr) { 
             insertParent(L_Parent, alokasiParent(101, "Data Scientist", "TechCorp", 3.5));
             insertParent(L_Parent, alokasiParent(102, "Mobile Developer", "GameDev", 3.0));
             insertParent(L_Parent, alokasiParent(103, "Backend Engineer", "ServerX", 3.2));
             insertParent(L_Parent, alokasiParent(104, "UI/UX Designer", "CreativeStudio", 3.0));
             insertParent(L_Parent, alokasiParent(105, "DevOps Engineer", "CloudSys", 3.5));
             insertParent(L_Parent, alokasiParent(106, "Product Manager", "StartUpInc", 3.0));
        }
        
        while (true) {
            // --- PRE-CALCULATE NOTIFICATIONS FOR MENU ---
            int notifCount = 0;
            if (activeRole == "mahasiswa") {
                string currentNIM = "";
                address_child C = L_Child.first;
                while (C != nullptr) {
                    if (C->info.nama == activeUser) { 
                        currentNIM = C->info.nim;
                        break;
                    }
                    C = C->next;
                }
                if (!currentNIM.empty()) {
                    notifCount = countNotifikasi(L_Parent, currentNIM);
                }
            }
            // -------------------------------------------

            cout << "\n\n--- MENU UTAMA (" << activeRole << ") ---" << endl;
            
            // Display Menu based on Role using Switch
            switch (getRoleEnum(activeRole)) {
                case ROLE_MAHASISWA:
                    cout << "1. Input Data Diri" << endl;
                    cout << "2. Lihat Daftar Lowongan" << endl; 
                    cout << "3. Ajukan Lamaran (Upload CV)" << endl;
                    cout << "4. Cek Status Lamaran" << endl;
                    cout << "5. Pesan (" << notifCount << ")" << endl;
                    cout << "0. Keluar (Logout)" << endl;
                    break;
                case ROLE_DOSEN:
                    cout << "1. Verifikasi Lamaran" << endl;
                    cout << "2. Lihat Daftar Lowongan" << endl;
                    cout << "3. Rekap Lamaran Mahasiswa" << endl;
                    cout << "0. Keluar (Logout)" << endl;
                    break;
                case ROLE_PERUSAHAAN:
                    cout << "1. Input Lowongan Baru" << endl;
                    cout << "2. Berikan Keputusan Lamaran" << endl;
                    cout << "3. Rekap Lamaran Masuk (ATS Score)" << endl;
                    cout << "4. Cari Lowongan Online (API)" << endl;
                    cout << "0. Keluar (Logout)" << endl;
                    break;
                case ROLE_ADMIN:
                    cout << "1. Kelola User (Admin Panel)" << endl;
                    cout << "2. Lihat Semua Data (M:N)" << endl;
                    cout << "3. Rekap Semua Lamaran" << endl;
                    cout << "0. Keluar (Logout)" << endl;
                    break;
                default:
                    cout << "Role tidak dikenali." << endl;
                    break;
            }

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
                    // Try to find NIM based on activeUser (assuming Name == Username)
                    string currentNIM = "";
                    address_child C = L_Child.first;
                    while (C != nullptr) {
                        if (C->info.nama == activeUser) { // Simple matching
                            currentNIM = C->info.nim;
                            break;
                        }
                        C = C->next;
                    }
                    
                    int notifCount = 0;
                    if (!currentNIM.empty()) {
                        notifCount = countNotifikasi(L_Parent, currentNIM);
                    }

                    switch (pilihan_menu_utama) {
                        case 1: { // Input Data Diri
                            int pilihan_sub;
                            do {
                                string nim_input;
                                char nama_input[100];
                                int angkatan_input;
                                cout << "\n--- INPUT DATA DIRI ---" << endl;
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Masukkan NIM: "; getline(cin, nim_input);
                                // Auto-fill name with activeUser to ensure matching
                                cout << "Nama Lengkap (Otomatis): " << activeUser << endl;
                                strcpy(nama_input, activeUser.c_str()); 
                                
                                cout << "Masukkan Angkatan (Tahun): ";
                                if (!(cin >> angkatan_input)) {
                                    cout << "Input Angkatan tidak valid." << endl;
                                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    pilihan_sub = 1; continue;
                                }
                                if (findChildByNIM(L_Child, nim_input) != nullptr) {
                                    cout << "Data Mahasiswa sudah ada." << endl;
                                } else {
                                    insertChild(L_Child, alokasiChild(nim_input, nama_input, angkatan_input));
                                    cout << "Data berhasil disimpan." << endl;
                                }
                                cout << "1. Input Lagi, 2. Kembali: "; cin >> pilihan_sub;
                            } while (pilihan_sub == 1);
                            break;
                        }
                        case 2: { // Lihat Daftar Lowongan
                            showLowongan(L_Parent);
                            break;
                        }
                        case 3: { // Ajukan Lamaran + CV (PDF Picker)
                            int id_lowongan_input;
                            string nim_input, cv_path;
                            char nama_input[100];
                            
                            cout << "--- AJUKAN LAMARAN ---" << endl;
                            cout << "1. Pilih File CV (PDF/TXT) akan terbuka otomatis..." << endl;
                            system("pause"); 
                            
                            cv_path = OpenFileDialog();
                            if (cv_path.empty()) {
                                cout << "Tidak ada file yang dipilih. Lamaran dibatalkan." << endl;
                                break;
                            }
                            cout << "File terpilih: " << cv_path << endl;

                            cout << "Masukkan ID Lowongan: "; cin >> id_lowongan_input;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            
                            cout << "Masukkan NIM Anda: "; getline(cin, nim_input);
                            // Auto-fill name
                            cout << "Nama Lengkap: " << activeUser << endl;
                            
                            insertRelasi(L_Parent, L_Child, id_lowongan_input, nim_input, activeUser, counter_lamaran++, cv_path);
                            break;
                        }
                        case 4: { // Status Lamaran
                            string nim_input;
                            cout << "\n--- STATUS LAMARAN ---" << endl;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Masukkan NIM Anda: "; getline(cin, nim_input);
                            showStatusLamaranMahasiswa(L_Parent, nim_input);
                            break;
                        }
                        case 5: { // Pesan / Notifikasi
                            if (currentNIM.empty()) {
                                cout << "Silakan 'Input Data Diri' terlebih dahulu agar sistem mengenali NIM Anda." << endl;
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
                        case 4: { // API Search
                            string keyword;
                            cout << "Masukkan Kata Kunci Lowongan (misal: Java, Marketing): ";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            getline(cin, keyword);

                            string command = "python tools/search_jobs.py \"" + keyword + "\"";
                            system(command.c_str());

                            importLowonganFromAPI(L_Parent, "search_results.txt");
                            break;
                        }
                        default: cout << "Pilihan tidak valid." << endl;
                    }
                    break;
                }
                case ROLE_ADMIN: {
                    switch (pilihan_menu_utama) {
                        case 1: { // Admin Panel
                            bool logout = false;
                            MainAdmin("admin", logout); 
                            break;
                        }
                        case 2: { // Show All
                            showLowonganDanPelamar(L_Parent);
                            break;
                        }
                        case 3: { // Rekap
                            showRekapLamaranPerusahaan(L_Parent);
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
    
    Cleanup(); // Cleanup at end of main (just in case)

    return 0;
}
