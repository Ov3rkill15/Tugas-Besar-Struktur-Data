#include "lowongan.h"
#include <cstring>
#include <limits>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>

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

void menuCariLowongan(ListParent L) {
    int id_cari;

    cout << "\n--- CARI LOWONGAN BERDASARKAN ID ---" << endl;
    cout << "Masukkan ID Lowongan yang dicari: ";

    if (!(cin >> id_cari)) {
        cout << " Input ID tidak valid. Silakan masukkan angka." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    // Panggil fungsi findParent
    address_parent P_Found = findParent(L, id_cari);

    if (P_Found != nullptr) {
        cout << "   Ditemukan!" << endl;
        cout << "   ID: " << P_Found->info.id_lowongan
             << " | Posisi: " << P_Found->info.posisi
             << " | Perusahaan: " << P_Found->info.nama_perusahaan
             << " | IPK Min: " << fixed << setprecision(2) << P_Found->info.kuota << endl;
    } else {
        cout << "    Lowongan ID " << id_cari << " TIDAK Ditemukan." << endl;
    }
}

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

void deleteParentByID(ListParent &L_Parent, int ID_Lowongan_Target) {
    address_parent P_Before = nullptr;
    address_parent P_Current = L_Parent.first;
    bool target_found = false; // Flag pengganti break

    // 1. CARI P_TARGET dan P_BEFORE
    while (P_Current != nullptr && !target_found) {
        if (P_Current->info.id_lowongan == ID_Lowongan_Target) {
            target_found = true;
        } else {
            P_Before = P_Current;
            P_Current = P_Current->next;
        }
    }

    // A. Kasus 1: Lowongan tidak ditemukan
    if (!target_found) {
        cout << " Gagal menghapus. Lowongan ID " << ID_Lowongan_Target << " tidak ditemukan." << endl;
        return;
    }

    // 2. HAPUS SEMUA NODE RELASI yang terikat pada P_Current (Integritas MLL)
    // Walaupun ini ada di lowongan.cpp, ini adalah langkah penting MLL.
    address_relasi R_Del = P_Current->first_relasi;
    while (R_Del != nullptr) {
        address_relasi R_Next = R_Del->next;
        delete R_Del; // Dealokasi Node Relasi
        R_Del = R_Next;
    }

    // 3. HAPUS NODE PARENT (P_Current)
    if (P_Current == L_Parent.first) {
        // Kasus 2: Menghapus Node Pertama
        L_Parent.first = P_Current->next;
    } else {
        // Kasus 3: Menghapus Node Tengah/Akhir
        if (P_Before != nullptr) {
            P_Before->next = P_Current->next;
        }
    }

    // 4. Dealokasi Node Parent
    cout << "Lowongan ID " << ID_Lowongan_Target << " berhasil dihapus beserta semua Lamaran terkait." << endl;
    delete P_Current;
}


// --- FITUR API SEARCH ---
void importLowonganFromAPI(ListParent &L, string filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cout << "Gagal membuka file hasil pencarian API." << endl;
        return;
    }

    string line;
    int count = 0;
    struct JobTemp {
        string title;
        string company;
        double gpa;
    } jobs[20]; // Max 20 results

    cout << "\n--- HASIL PENCARIAN API ---" << endl;
    while (getline(file, line) && count < 20) {
        stringstream ss(line);
        string segment;
        vector<string> seglist;

        while (getline(ss, segment, '|')) {
            seglist.push_back(segment);
        }

        if (seglist.size() >= 3) {
            jobs[count].title = seglist[0];
            jobs[count].company = seglist[1];
            jobs[count].gpa = stod(seglist[2]);
            
            cout << count + 1 << ". " << jobs[count].title << " at " << jobs[count].company << " (Min GPA: " << jobs[count].gpa << ")" << endl;
            count++;
        }
    }
    file.close();

    if (count == 0) {
        cout << "Tidak ada lowongan ditemukan." << endl;
        return;
    }

    int choice;
    cout << "\nPilih nomor lowongan untuk ditambahkan (0 untuk batal): ";
    cin >> choice;

    if (choice > 0 && choice <= count) {
        // Generate Random ID for new job (simple approach)
        int newID = 200 + rand() % 1000; 
        
        // Check if ID exists (optional, but good practice)
        while (findParent(L, newID) != nullptr) {
            newID = 200 + rand() % 1000;
        }

        insertParent(L, alokasiParent(newID, jobs[choice-1].title, jobs[choice-1].company, jobs[choice-1].gpa));
        cout << "Lowongan berhasil ditambahkan dengan ID " << newID << "!" << endl;
    } else {
        cout << "Dibatalkan." << endl;
    }
}
