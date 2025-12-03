#ifndef MLL_STRUCTS_H
#define MLL_STRUCTS_H

#include <iostream>
#include <string>

using namespace std;

// Definisikan tipe data pointer agar semua struct bisa saling mengenali
typedef struct NodeRelasi* address_relasi;
typedef struct NodeParent* address_parent;
typedef struct NodeChild* address_child;

// --- 1. STRUKTUR LIST PARENT (LOWONGAN) ---
struct InfoLowongan {
    int id_lowongan;
    string posisi;
    string nama_perusahaan;
    float kuota; // Digunakan sebagai syarat IPK Minimum
};

struct NodeParent {
    InfoLowongan info;
    address_parent next;
    address_relasi first_relasi; // Pointer ke List Relasi
};

struct ListParent {
    address_parent first;
};

// --- 2. STRUKTUR LIST CHILD (MAHASISWA) ---
// Dideklarasikan di sini agar pointer relasi bisa mengenalinya
struct InfoMahasiswa {
    string nim;
    string nama;
    int angkatan;
};

struct NodeChild {
    InfoMahasiswa info;
    address_child next;
};

struct ListChild {
    address_child first;
};

// --- 3. STRUKTUR LIST RELASI (LAMARAN) ---
// Dideklarasikan di sini karena menampung address_parent dan address_child
struct InfoLamaran {
    int id_lamaran;
    int status_dosen;
    int status_perusahaan;
    string cv_path; // Menyimpan path file CV
    int cv_score;   // Menyimpan skor ATS
};

struct NodeRelasi {
    InfoLamaran info;
    address_relasi next;
    address_parent ptr_parent;
    address_child ptr_child;
};

#endif // MLL_STRUCTS_H
