#ifndef LOGIN_H
#define LOGIN_H
#include <windows.h>  // Harus sebelum <string> untuk menghindari konflik std::byte
#include <string>
#include <iostream>
using namespace std;
const int NMAX_USER = 45;
const int MAX_RIWAYAT = 100;
struct User {
    string username;
    string password;
    string role;
    string nim;  // NIM untuk mahasiswa, kosong untuk role lainnya
};
struct RiwayatAdmin {
    string timestamp;
    string aktivitas;
    string oleh_admin;
};
extern User users[NMAX_USER];
extern int jumlahPengguna;
extern RiwayatAdmin adminRiwayat[MAX_RIWAYAT];
extern int jumlahRiwayat;
void initUsers();
bool Login(string &activeUser, string &activeRole, string &activeNIM);  // Ditambahkan activeNIM
bool AuthenticateUser(string username, string password);
bool isUsernameExists(string username);
string getNIMByUsername(string username);  // Fungsi helper baru
void MainAdmin(string adminUsername, bool &logout);
void KelolaPengguna(string adminUsername);
void CetakPengguna();
void LihatRiwayatAdmin();
void CatatRiwayatAdmin(string adminUsername, string aktivitas);
bool TambahPenggunaBaru(string adminUsername);
bool HapusPengguna(string adminUsername);
bool UbahDataPengguna(string adminUsername);
bool ResetPasswordPengguna(string adminUsername);
bool GantiPasswordAdmin(string adminUsername);
void ClearScreen();
void Loading(int ms);
void PrintAdminHeader();  // ASCII Art ADMIN merah
string ReadPassword();
string GetTimestamp();
#endif // LOGIN_H
