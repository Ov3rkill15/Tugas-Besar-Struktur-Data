#include "mahasiswa.h"
#include <cstring>
using namespace std;
void createListChild(ListChild &L) {
    L.first = nullptr;
}

address_child alokasiChild(string NIM, string Nama, int Angkatan) {
    address_child C = new NodeChild;
    if (C != nullptr) {
        C->info.nim = NIM;
        C->info.nama = Nama;
        C->info.angkatan = Angkatan;
        C->next = nullptr;
    }
    return C;
}

void insertChild(ListChild &L, address_child C_Baru) {
    if (L.first == nullptr) L.first = C_Baru;
    else {
        address_child Last = L.first;
        while (Last->next != nullptr) Last = Last->next;
        Last->next = C_Baru;
    }
}

address_child findChildByNIM(ListChild L, string NIM_Target) {
    address_child P = L.first;
    while (P != nullptr) {
        if (P->info.nim == NIM_Target) return P;
        P = P->next;
    }
    return nullptr;
}

void menuCariMahasiswa(ListChild L) {
    string nim_cari;

    cout << "\n\033[36m+============================================+\033[0m" << endl;
    cout << "|     \033[33mCARI MAHASISWA BERDASARKAN NIM\033[0m        |" << endl;
    cout << "\033[36m+============================================+\033[0m" << endl;
    cout << "| Masukkan NIM: ";

    getline(cin, nim_cari);

    address_child C_Found = findChildByNIM(L, nim_cari);

    if (C_Found != nullptr) {
        cout << "\033[32m| [DITEMUKAN]\033[0m" << endl;
        cout << "\033[36m+--------------------------------------------+\033[0m" << endl;
        cout << "| NIM      : \033[97m" << C_Found->info.nim << "\033[0m" << endl;
        cout << "| Nama     : \033[92m" << C_Found->info.nama << "\033[0m" << endl;
        cout << "| Angkatan : \033[93m" << C_Found->info.angkatan << "\033[0m" << endl;
        cout << "\033[36m+============================================+\033[0m" << endl;
    } else {
        cout << "\033[91m| [TIDAK DITEMUKAN] NIM " << nim_cari << "\033[0m" << endl;
        cout << "\033[36m+============================================+\033[0m" << endl;
    }
}

void showMahasiswa(ListChild L_Child) {
    address_child C = L_Child.first;
    int count = 0;
    
    cout << "\n\033[36m+============================================+\033[0m" << endl;
    cout << "|        \033[33mDAFTAR MAHASISWA TERDAFTAR\033[0m         |" << endl;
    cout << "\033[36m+============================================+\033[0m" << endl;
    
    if (C == nullptr) { 
        cout << "| \033[91mBelum ada mahasiswa terdaftar.\033[0m" << endl;
        cout << "\033[36m+============================================+\033[0m" << endl;
        return; 
    }
    
    while (C != nullptr) {
        count++;
        cout << "\033[36m+--------------------------------------------+\033[0m" << endl;
        cout << "| \033[92m" << C->info.nama << "\033[0m" << endl;
        cout << "|   NIM: \033[97m" << C->info.nim << "\033[0m | Angkatan: \033[93m" << C->info.angkatan << "\033[0m" << endl;
        C = C->next;
    }
    cout << "\033[36m+============================================+\033[0m" << endl;
    cout << "| Total: \033[92m" << count << "\033[0m mahasiswa terdaftar" << endl;
    cout << "\033[36m+============================================+\033[0m" << endl;
}

void handleInputMahasiswa(ListChild &L_Child, string NIM, string Nama) {
    address_child C_Target = findChildByNIM(L_Child, NIM);

    if (C_Target == nullptr) {
        address_child C_Baru = alokasiChild(NIM, Nama, 0);
        insertChild(L_Child, C_Baru);
        cout << "   [INFO] Data Mahasiswa '" << Nama << "' ditambahkan ke List Child." << endl;
    }
}

void cleanupRelasiBeforeChildDeletion(ListParent &L_Parent, string NIM_Target) {
    address_parent P = L_Parent.first;

    while (P != nullptr) {
        address_relasi R_Before = nullptr;
        address_relasi R_Current = P->first_relasi;

        while (R_Current != nullptr) {
            bool is_deleted = false;

            if (R_Current->ptr_child->info.nim == NIM_Target) {
                is_deleted = true;

                if (R_Current == P->first_relasi) {
                    P->first_relasi = R_Current->next;
                    delete R_Current;
                    R_Current = P->first_relasi;
                } else {
                    R_Before->next = R_Current->next;
                    delete R_Current;
                    R_Current = R_Before->next;
                }
            }

            if (!is_deleted) {
                R_Before = R_Current;
                R_Current = R_Current->next;
            }
        }
        P = P->next;
    }
}


void deleteChildByNIM(ListChild &L_Child, ListParent &L_Parent, string NIM_Target) {
    cleanupRelasiBeforeChildDeletion(L_Parent, NIM_Target);

    address_child C_Before = nullptr;
    address_child C_Target = L_Child.first;
    bool target_found = false;

    while (C_Target != nullptr && !target_found) {
        if (C_Target->info.nim == NIM_Target) {
            target_found = true;
        } else {
            C_Before = C_Target;
            C_Target = C_Target->next;
        }
    }

    if (!target_found) {
        cout << " Gagal menghapus. Mahasiswa dengan NIM " << NIM_Target << " tidak ditemukan." << endl;
        return;
    }

    if (C_Target == L_Child.first) {
        L_Child.first = C_Target->next;
    } else {
        if (C_Before != nullptr) {
            C_Before->next = C_Target->next;
        }
    }

    cout << " Mahasiswa dengan NIM " << NIM_Target
         << " berhasil dihapus beserta semua Lamaran terkait." << endl;
    delete C_Target;
}

