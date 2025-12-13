#include "mahasiswa.h"
#include <cstring>

using namespace std;

// Implementasi fungsi dasar List Child (Mahasiswa)

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

// Fungsi (b) Insert element child
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

// Tambahkan di mahasiswa.cpp, di bawah handleInputMahasiswa:

void showMahasiswa(ListChild L_Child) {
    address_child C = L_Child.first;
    cout << "\n--- DAFTAR MAHASISWA (CHILD LIST) ---" << endl;
    if (C == nullptr) { cout << "List Mahasiswa kosong." << endl; return; }
    while (C != nullptr) {
        cout << " - NIM: " << C->info.nim << " | Nama: " << C->info.nama << " | Angkatan: " << C->info.angkatan << endl;
        C = C->next;
    }
}

// Fungsi utama untuk input dinamis
void handleInputMahasiswa(ListChild &L_Child, string NIM, string Nama) {
    address_child C_Target = findChildByNIM(L_Child, NIM);

    if (C_Target == nullptr) {
        // Angkatan diset 0 (default)
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

        // Traverse the Relasi sub-list of the current Parent (P)
        while (R_Current != nullptr) {
            bool is_deleted = false;

            // Cek apakah Node Relasi ini menunjuk ke Mahasiswa yang akan dihapus
            if (R_Current->ptr_child->info.nim == NIM_Target) {
                is_deleted = true;

                if (R_Current == P->first_relasi) {
                    // Kasus 1: Node Relasi Pertama
                    P->first_relasi = R_Current->next;
                    delete R_Current;
                    R_Current = P->first_relasi;
                } else {
                    // Kasus 2: Node Tengah/Akhir
                    R_Before->next = R_Current->next;
                    delete R_Current;
                    R_Current = R_Before->next;
                }
            }

            // Pindah ke node berikutnya HANYA jika node saat ini TIDAK dihapus
            if (!is_deleted) {
                R_Before = R_Current;
                R_Current = R_Current->next;
            }
        }
        P = P->next;
    }
}


void deleteChildByNIM(ListChild &L_Child, ListParent &L_Parent, string NIM_Target) {
    // 1. CLEANUP: Hapus semua Node Relasi yang terikat (GLOBAL TRAVERSAL)
    cleanupRelasiBeforeChildDeletion(L_Parent, NIM_Target);

    // 2. CARI C_TARGET dan C_BEFORE di List Child
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

    // A. Kasus 1: Mahasiswa tidak ditemukan
    if (!target_found) {
        cout << " Gagal menghapus. Mahasiswa dengan NIM " << NIM_Target << " tidak ditemukan." << endl;
        return;
    }

    // 3. HAPUS NODE CHILD (C_Target)
    if (C_Target == L_Child.first) {
        // Menghapus Node Pertama
        L_Child.first = C_Target->next;
    } else {
        // Menghapus Node Tengah/Akhir
        if (C_Before != nullptr) {
            C_Before->next = C_Target->next;
        }
    }

    // 4. Dealokasi Node Child
    cout << " Mahasiswa dengan NIM " << NIM_Target
         << " berhasil dihapus beserta semua Lamaran terkait." << endl;
    delete C_Target;
}

