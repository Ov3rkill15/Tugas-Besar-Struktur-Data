#include "lowongan.h"
#include <cstring>
#include <limits>
#include <iomanip>

using namespace std;

// --- IMPLEMENTASI DASAR (DIPERLUKAN OLEH MENU) ---

void createListParent(ListParent &L) {
    L.first = nullptr;
}

address_parent alokasiParent(int ID, string Posisi, string Perusahaan, float Kuota) {
    address_parent P = new NodeParent;
    if (P != nullptr) {
        P->info.id_lowongan = ID;
        P->info.posisi = Posisi;
        P->info.nama_perusahaan = Perusahaan;
        P->info.kuota = Kuota;
        P->next = nullptr;
        P->first_relasi = nullptr;
    }
    return P;
}

void insertParent(ListParent &L, address_parent P_Baru) {
    if (L.first == nullptr) L.first = P_Baru;
    else { address_parent Last = L.first; while (Last->next != nullptr) Last = Last->next; Last->next = P_Baru; }
    cout << "   [INFO] Lowongan ID " << P_Baru->info.id_lowongan << " (" << P_Baru->info.posisi << ") berhasil ditambahkan." << endl;
}

address_parent findParent(ListParent L, int ID_Target) {
    address_parent P = L.first;
    while (P != nullptr) {
        if (P->info.id_lowongan == ID_Target) return P;
        P = P->next;
    }
    return nullptr;
}

// Tambahkan di lowongan.cpp, di bawah findParent:

void showLowongan(ListParent L_Parent) {
    address_parent P = L_Parent.first;
    cout << "\n--- DAFTAR LOWONGAN (PARENT LIST) ---" << endl;
    if (P == nullptr) { cout << "List Lowongan kosong." << endl; return; }
    while (P != nullptr) {
        cout << " - ID: " << P->info.id_lowongan << " | Posisi: " << P->info.posisi
             << " | Perusahaan: " << P->info.nama_perusahaan
             << " | IPK Min: " << fixed << setprecision(2) << P->info.kuota << endl;
        P = P->next;
    }
}

// --- FUNGSI INPUT UTAMA (MENU) ---

void menuInsertParent(ListParent &L, int &ID_Counter) {
    char posisi_input[50];
    char perusahaan_input[100];
    float kuota_input;
    int id_baru;

    cout << "\n--- INPUT LOWONGAN BARU ---" << endl;

    // 1. Ambil ID dari Counter
    id_baru = ID_Counter;
    cout << "ID Lowongan Otomatis: " << id_baru << endl;

    // 2. INPUT POSISI/ROLE (STRING)
    cout << "Masukkan Posisi/Role: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Membersihkan buffer sebelum getline
    cin.getline(posisi_input, 50);

    // 3. INPUT NAMA PERUSAHAAN (STRING)
    cout << "Masukkan Nama Perusahaan: ";
    cin.getline(perusahaan_input, 100);

    // 4. INPUT SYARAT IPK/KUOTA (FLOAT)
    cout << "Masukkan Syarat IPK Minimum (Contoh: 3.00): ";
    if (!(cin >> kuota_input)) {
        cout << "Input IPK tidak valid. Lowongan dibatalkan." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    // 5. ALOKASI DAN INSERT
    address_parent P_Baru = alokasiParent(id_baru, posisi_input, perusahaan_input, kuota_input);
    insertParent(L, P_Baru);

    ID_Counter++; // Tambah counter untuk Lowongan berikutnya
}
