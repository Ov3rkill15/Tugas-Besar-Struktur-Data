#ifndef MLL_STRUCTS_H
#define MLL_STRUCTS_H

#include <iostream>
#include <string>

using namespace std;
typedef struct NodeRelasi *address_relasi;
typedef struct NodeParent *address_parent;
typedef struct NodeChild *address_child;
struct InfoLowongan {
    int id_lowongan;
    string posisi;
    string nama_perusahaan;
    float ipk_min;    // IPK Minimum (0.00 - 4.00)
    int kuota;        // Jumlah posisi tersedia
};

struct NodeParent {
    InfoLowongan info;
    address_parent next;
    address_relasi first_relasi;
};

struct ListParent {
    address_parent first;
};

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

struct InfoLamaran {
    int id_lamaran;
    int status_dosen;
    int status_perusahaan;
    string cv_path;
    int cv_score;
};

struct NodeRelasi {
    InfoLamaran info;
    address_relasi next;
    address_parent ptr_parent;
    address_child ptr_child;
};

#endif // MLL_STRUCTS_H
