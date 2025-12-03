#ifndef LOGIN_H
#define LOGIN_H

#include <string>
#include <iostream>

// Removed 'using namespace std;' to avoid conflicts with windows.h (specifically 'byte')

const int NMAX_USER = 45;
const int MAX_RIWAYAT = 100;

struct User {
    std::string username;
    std::string password;
    std::string role;
};

struct RiwayatAdmin {
    std::string timestamp;
    std::string aktivitas;
    std::string oleh_admin;
};

// Global variables (extern)
extern User users[NMAX_USER];
extern int jumlahPengguna;
extern RiwayatAdmin adminRiwayat[MAX_RIWAYAT];
extern int jumlahRiwayat;

// Function Prototypes
void initUsers();
bool Login(std::string &activeUser, std::string &activeRole);
bool AuthenticateUser(std::string username, std::string password);
bool isUsernameExists(std::string username);

// Admin Functions
void MainAdmin(std::string adminUsername, bool &logout);
void KelolaPengguna(std::string adminUsername);
void CetakPengguna();
void LihatRiwayatAdmin();
void CatatRiwayatAdmin(std::string adminUsername, std::string aktivitas);
bool TambahPenggunaBaru(std::string adminUsername);
bool HapusPengguna(std::string adminUsername);
bool UbahDataPengguna(std::string adminUsername);
bool ResetPasswordPengguna(std::string adminUsername);
bool GantiPasswordAdmin(std::string adminUsername);

// Helper Functions
void ClearScreen();
void Loading(int ms);
std::string ReadPassword();
std::string GetTimestamp();

#endif // LOGIN_H
