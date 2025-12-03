#include "login.h"
#include <iostream>
#include <vector>
#include <conio.h> // For getch() on Windows
#include <windows.h> // For Sleep()
#include <ctime>
#include <sstream>

using namespace std;

// Global Variables Definition
User users[NMAX_USER];
int jumlahPengguna = 0;
RiwayatAdmin adminRiwayat[MAX_RIWAYAT];
int jumlahRiwayat = 0;

// Helper Functions
void ClearScreen() {
    system("cls");
}

void Loading(int ms) {
    Sleep(ms);
}

string ReadPassword() {
    string password = "";
    char ch;
    while ((ch = _getch()) != 13) { // 13 is Enter
        if (ch == 8) { // 8 is Backspace
            if (password.length() > 0) {
                cout << "\b \b";
                password.pop_back();
            }
        } else {
            cout << "*";
            password += ch;
        }
    }
    cout << endl;
    return password;
}

string GetTimestamp() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}

// ASCII Art Helpers
void PrintLoginHeader() {
    cout << "\033[32m"; // Green
    cout << R"(
 _     ___   ____ ___ _   _
| |   / _ \ / ___|_ _| \ | |
| |  | | | | |  _ | ||  \| |
| |__| |_| | |_| || || |\  |
|_____\___/ \____|___|_| \_|
)" <<endl;
    cout << "\033[0m";
}

void PrintMasukHeader() {
    cout << "\033[32m";
    cout << R"(
 __  __    _    ____  _   _ _  __
|  \/  |  / \  / ___|| | | | |/ /
| |\/| | / _ \ \___ \| | | | ' /
| |  | |/ ___ \ ___) | |_| | . \
|_|  |_/_/   \_\____/ \___/|_|\_\
)" <<endl;
    cout << "\033[0m";
}

void PrintDaftarHeader() {
    cout << "\033[32m";
    cout << R"(
 ____  _____ ____ ___ ____ _____ _____ ____
|  _ \| ____/ ___|_ _/ ___|_   _| ____|  _ \
| |_) |  _|| |  _ | |\___ \ | | |  _| | |_) |
|  _ <| |__| |_| || | ___) || | | |___|  _ <
|_| \_\_____\____|___|____/ |_| |_____|_| \_\
)" << endl;
    cout << "\033[0m";
}

void PrintAdminHeader() {
    cout << "\033[31m"; // Red
    cout << R"(
    _    ____  __  __ ___ _   _
   / \  |  _ \|  \/  |_ _| \ | |
  / _ \ | | | | |\/| || ||  \| |
 / ___ \| |_| | |  | || || |\  |
/_/   \_\____/|_|  |_|___|_| \_|
)" << endl;
    cout << "\033[0m";
}

// Core Functions
void initUsers() {
    // Data awal pengguna hardcoded
    struct InitData { string u; string p; string r; };
    vector<InitData> data = {
        {"admin", "admin123", "admin"},
        {"dosen1", "dosen123", "dosen"},
        {"techcorp", "123", "perusahaan"},
        {"nathasyayuanmaharani", "0001", "mahasiswa"},
        {"theodoreelvisestrada", "0006", "mahasiswa"},
        {"dyahkusumawardani", "0009", "mahasiswa"},
        {"azrielraihaneldovahartoto", "0010", "mahasiswa"},
        {"muhammadilhamalifianda", "0022", "mahasiswa"},
        {"alyaazizaputeri", "0026", "mahasiswa"},
        {"ahmadabdansyakuro", "0029", "mahasiswa"},
        {"fathurrahmanalfarizi", "0035", "mahasiswa"},
        {"nuswantorosetyomukti", "0040", "mahasiswa"},
        {"anggitacahyatihidayat", "0041", "mahasiswa"},
        {"wibnuhijrahfranstio", "0048", "mahasiswa"},
        {"meyshaprimiandita", "0050", "mahasiswa"},
        {"muhammadfiqrihabibi", "0056", "mahasiswa"},
        {"fitriacahyani", "0060", "mahasiswa"},
        {"triansyahdaniswaraibrahim", "0062", "mahasiswa"},
        {"rakhaabdillahalkautsar", "0068", "mahasiswa"},
        {"avicenanaufallathif", "0073", "mahasiswa"},
        {"naylaassyifa", "0078", "mahasiswa"},
        {"williampetervanxnajoan", "0084", "mahasiswa"},
        {"rayvanalifarlomahesworo", "0087", "mahasiswa"},
        {"zaidansalamrojab", "0088", "mahasiswa"},
        {"audreyfredileyhanas", "0093", "mahasiswa"},
        {"muhammadnaelfadly", "0096", "mahasiswa"},
        {"nairacahayaputridarmawansinaga", "0100", "mahasiswa"},
        {"muhamadalwansuryadi", "0104", "mahasiswa"},
        {"dhafyahmadzubaidi", "0107", "mahasiswa"},
        {"muhammadfarisdhiyaylhaqsarbini", "0117", "mahasiswa"},
        {"nursyadira", "0123", "mahasiswa"},
        {"rayfitokrisnawijaya", "0124", "mahasiswa"},
        {"mochammadrafirisqullah", "0129", "mahasiswa"},
        {"iputugedeagastyakrisnawidartha", "0134", "mahasiswa"},
        {"rendil", "0137", "mahasiswa"},
        {"muhammadariqazzaki", "0138", "mahasiswa"},
        {"edmundyuliusgantur", "0155", "mahasiswa"},
        {"muhammadsayyidhuwaidi", "0157", "mahasiswa"},
        {"muhdzuljalalwaliikramjalil", "0160", "mahasiswa"},
        {"ramadhantangguhdefennder", "0003", "mahasiswa"},
        {"adzkiyaputrirahmawan", "0025", "mahasiswa"},
        {"fathimahradhiyya", "0029", "mahasiswa"},
        {"rakanghazianadiwjaya", "0034", "mahasiswa"},
        {"jihannabilamubarakah", "0037", "mahasiswa"}
    };

    jumlahPengguna = 0;
    for (const auto& d : data) {
        if (jumlahPengguna < NMAX_USER) {
            users[jumlahPengguna].username = d.u;
            users[jumlahPengguna].password = d.p;
            users[jumlahPengguna].role = d.r;
            jumlahPengguna++;
        }
    }
}

bool AuthenticateUser(string username, string password) {
    for (int i = 0; i < jumlahPengguna; i++) {
        if (users[i].username == username && users[i].password == password) {
            return true;
        }
    }
    return false;
}

bool isUsernameExists(string username) {
    for (int i = 0; i < jumlahPengguna; i++) {
        if (users[i].username == username) {
            return true;
        }
    }
    return false;
}

bool Login(string &activeUser, string &activeRole) {
    string pilihan;
    bool running = true;

    while (running) {
        ClearScreen();
        PrintLoginHeader();
        cout << "1. Masuk" << endl;
        cout << "2. Daftar" << endl;
        cout << "0. Keluar" << endl;
        cout << "Pilih yang mana: ";
        getline(cin, pilihan);

        if (pilihan == "1" || pilihan == "masuk") {
            ClearScreen();
            PrintMasukHeader();
            int attempts = 0;
            while (attempts < 3) {
                string u, p;
                cout << "Masukkan Username: "; getline(cin, u);
                cout << "Masukkan Password: "; p = ReadPassword();

                if (AuthenticateUser(u, p)) {
                    activeUser = u;
                    
                    // Find role
                    for(int i=0; i<jumlahPengguna; i++) {
                        if(users[i].username == u) {
                            activeRole = users[i].role;
                            break;
                        }
                    }

                    ClearScreen();
                    cout << "Login berhasil! Role: " << activeRole << endl;
                    cout << "Menuju Aplikasi..." << endl;
                    Loading(1200);

                    // Admin specific check removed here, will be handled in main.cpp
                    return true; 
                } else {
                    cout << "Username atau password salah." << endl;
                    attempts++;
                    cout << "Tersisa " << (3 - attempts) << " kesempatan." << endl;
                }
            }
        } else if (pilihan == "2" || pilihan == "daftar") {
            ClearScreen();
            PrintDaftarHeader();
            if (jumlahPengguna >= NMAX_USER) {
                cout << "Maaf, jumlah maksimum pengguna telah tercapai." << endl;
                Loading(1500);
                continue;
            }

            string u, kota, p1, p2;
            cout << "Masukkan Username: "; getline(cin, u);
            if (isUsernameExists(u)) {
                cout << "Username sudah digunakan." << endl;
                Loading(1500);
                continue;
            }

            cout << "Masukkan Asal Kota: "; getline(cin, kota);
            cout << "Selamat datang, " << u << " dari " << kota << endl;

            int attempts = 0;
            bool success = false;
            while (attempts < 3) {
                cout << "Masukkan Password: "; p1 = ReadPassword();
                cout << "Konfirmasi Password: "; p2 = ReadPassword();

                if (p1 == p2) {
                    users[jumlahPengguna].username = u;
                    users[jumlahPengguna].password = p1;
                    users[jumlahPengguna].role = "mahasiswa"; // Default role
                    jumlahPengguna++;
                    cout << "Signup berhasil! Silakan login." << endl;
                    success = true;
                    Loading(1500);
                    break;
                } else {
                    cout << "Password tidak cocok." << endl;
                    attempts++;
                }
            }
        } else if (pilihan == "0" || pilihan == "keluar") {
            cout << "Keluar dari aplikasi..." << endl;
            
            running = false;
            return false; // Return false to signal exit in main
        } else {
            cout << "Pilihan tidak valid." << endl;
            Loading(1000);
        }
    }
    return false;
}

// --- ADMIN FUNCTIONS ---

void MainAdmin(string adminUsername, bool &logout) {
    bool running = true;
    while (running) {
        ClearScreen();
        PrintAdminHeader();
        cout << "=========================================" << endl;
        cout << "          ADMIN PERMISSIONS" << endl;
        cout << "=========================================" << endl;
        cout << "1. Kelola Pengguna (Tambah/Hapus/Ubah)" << endl;
        cout << "2. Cetak Daftar Pengguna" << endl;
        cout << "3. Lihat Riwayat Aktivitas Admin" << endl;
        cout << "4. Reset Password Pengguna" << endl;
        cout << "5. Ganti Password Admin" << endl;
        cout << "6. Logout" << endl;
        cout << "0. Keluar Aplikasi" << endl;
        cout << "Masukkan pilihan: ";

        string pilihan;
        getline(cin, pilihan);

        if (pilihan == "1") KelolaPengguna(adminUsername);
        else if (pilihan == "2") CetakPengguna();
        else if (pilihan == "3") LihatRiwayatAdmin();
        else if (pilihan == "4") ResetPasswordPengguna(adminUsername);
        else if (pilihan == "5") GantiPasswordAdmin(adminUsername);
        else if (pilihan == "6") {
            cout << "Logout dari mode admin." << endl;
            logout = true;
            running = false;
            Loading(1000);
        } else if (pilihan == "0") {
            exit(0);
        } else {
            cout << "Pilihan tidak valid." << endl;
            Loading(1000);
        }
    }
}

void CetakPengguna() {
    ClearScreen();
    cout << "\n--- Daftar Pengguna ---" << endl;
    if (jumlahPengguna == 0) cout << "Tidak ada pengguna." << endl;
    else {
        for (int i = 0; i < jumlahPengguna; i++) {
            cout << (i + 1) << ". Username: " << users[i].username << endl;
        }
    }
    cout << "\nTekan Enter untuk kembali...";
    cin.get();
}

void KelolaPengguna(string adminUsername) {
    bool managing = true;
    while (managing) {
        ClearScreen();
        cout << "=========================================" << endl;
        cout << "            KELOLA PENGGUNA" << endl;
        cout << "=========================================" << endl;
        cout << "1. Tambah Pengguna Baru" << endl;
        cout << "2. Hapus Pengguna" << endl;
        cout << "3. Ubah Data Pengguna" << endl;
        cout << "0. Kembali ke Menu Admin" << endl;
        cout << "Masukkan pilihan: ";
        string p; getline(cin, p);

        if (p == "1") TambahPenggunaBaru(adminUsername);
        else if (p == "2") HapusPengguna(adminUsername);
        else if (p == "3") UbahDataPengguna(adminUsername);
        else if (p == "0") managing = false;
    }
}

bool TambahPenggunaBaru(string adminUsername) {
    cout << "Masukkan Username baru: ";
    string u; getline(cin, u);
    if (isUsernameExists(u)) {
        cout << "Username sudah ada." << endl;
        Loading(1000);
        return false;
    }
    cout << "Masukkan Password: ";
    string p = ReadPassword();

    if (jumlahPengguna < NMAX_USER) {
        users[jumlahPengguna].username = u;
        users[jumlahPengguna].password = p;
        jumlahPengguna++;
        cout << "Pengguna berhasil ditambahkan." << endl;
        CatatRiwayatAdmin(adminUsername, "Menambahkan pengguna: " + u);
        Loading(1000);
        return true;
    }
    return false;
}

bool HapusPengguna(string adminUsername) {
    CetakPengguna();
    cout << "Masukkan username yang akan dihapus: ";
    string u; getline(cin, u);

    if (u == "admin") {
        cout << "Tidak bisa menghapus admin utama." << endl;
        Loading(1000);
        return false;
    }

    int idx = -1;
    for (int i = 0; i < jumlahPengguna; i++) {
        if (users[i].username == u) {
            idx = i;
            break;
        }
    }

    if (idx != -1) {
        for (int j = idx; j < jumlahPengguna - 1; j++) {
            users[j] = users[j+1];
        }
        jumlahPengguna--;
        cout << "Pengguna berhasil dihapus." << endl;
        CatatRiwayatAdmin(adminUsername, "Menghapus pengguna: " + u);
        Loading(1000);
        return true;
    }
    cout << "Pengguna tidak ditemukan." << endl;
    Loading(1000);
    return false;
}

bool UbahDataPengguna(string adminUsername) {
    CetakPengguna();
    cout << "Masukkan username yang ingin diubah: ";
    string oldU; getline(cin, oldU);

    int idx = -1;
    for (int i = 0; i < jumlahPengguna; i++) {
        if (users[i].username == oldU) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        cout << "Pengguna tidak ditemukan." << endl;
        Loading(1000);
        return false;
    }

    cout << "Username baru (kosongkan jika tetap): ";
    string newU; getline(cin, newU);
    if (newU.empty()) newU = users[idx].username;
    else if (newU != oldU && isUsernameExists(newU)) {
        cout << "Username baru sudah dipakai." << endl;
        Loading(1000);
        return false;
    }

    cout << "Password baru (kosongkan jika tetap): ";
    string newP = ReadPassword();
    if (newP.empty()) newP = users[idx].password;

    users[idx].username = newU;
    users[idx].password = newP;
    cout << "Data berhasil diubah." << endl;
    CatatRiwayatAdmin(adminUsername, "Mengubah data pengguna: " + oldU);
    Loading(1000);
    return true;
}

bool ResetPasswordPengguna(string adminUsername) {
    CetakPengguna();
    cout << "Username untuk reset password: ";
    string u; getline(cin, u);

    int idx = -1;
    for (int i = 0; i < jumlahPengguna; i++) {
        if (users[i].username == u) {
            idx = i;
            break;
        }
    }

    if (idx != -1) {
        cout << "Password baru: ";
        string p = ReadPassword();
        users[idx].password = p;
        cout << "Password berhasil direset." << endl;
        CatatRiwayatAdmin(adminUsername, "Reset password: " + u);
        Loading(1000);
        return true;
    }
    cout << "User tidak ditemukan." << endl;
    Loading(1000);
    return false;
}

bool GantiPasswordAdmin(string adminUsername) {
    cout << "Password lama: ";
    string oldP = ReadPassword();

    // Find admin index (usually 0, but search to be safe)
    int idx = -1;
    for(int i=0; i<jumlahPengguna; i++) {
        if(users[i].username == adminUsername) {
            idx = i;
            break;
        }
    }

    if (idx != -1 && users[idx].password == oldP) {
        cout << "Password baru: "; string newP = ReadPassword();
        cout << "Konfirmasi: "; string confP = ReadPassword();
        if (newP == confP) {
            users[idx].password = newP;
            cout << "Password admin berhasil diubah." << endl;
            CatatRiwayatAdmin(adminUsername, "Ganti password admin");
            Loading(1000);
            return true;
        }
        cout << "Password tidak cocok." << endl;
    } else {
        cout << "Password lama salah." << endl;
    }
    Loading(1000);
    return false;
}

void CatatRiwayatAdmin(string adminUsername, string aktivitas) {
    if (jumlahRiwayat < MAX_RIWAYAT) {
        adminRiwayat[jumlahRiwayat].timestamp = GetTimestamp();
        adminRiwayat[jumlahRiwayat].aktivitas = aktivitas;
        adminRiwayat[jumlahRiwayat].oleh_admin = adminUsername;
        jumlahRiwayat++;
    }
}

void LihatRiwayatAdmin() {
    ClearScreen();
    cout << "\n--- Riwayat Aktivitas Admin ---" << endl;
    if (jumlahRiwayat == 0) cout << "Kosong." << endl;
    else {
        for (int i = 0; i < jumlahRiwayat; i++) {
            cout << (i+1) << ". [" << adminRiwayat[i].timestamp << "] "
                 << adminRiwayat[i].oleh_admin << " - " << adminRiwayat[i].aktivitas << endl;
        }
    }
    cout << "\nTekan Enter kembali...";
    cin.get();
}
