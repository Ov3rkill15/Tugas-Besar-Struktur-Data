#ifndef MAHASISWA_H
#define MAHASISWA_H

#include "mll_structs.h"

// DEKLARASI FUNGSI MAHASISWA (CHILD)
void createListChild(ListChild &L);
address_child alokasiChild(string NIM, string Nama, int Angkatan);
void insertChild(ListChild &L, address_child C_Baru);
address_child findChildByNIM(ListChild L, string NIM_Target);
void menuCariMahasiswa(ListChild L);
void showMahasiswa(ListChild L_Child);
void handleInputMahasiswa(ListChild &L_Child, string NIM, string Nama); // Untuk input dinamis
#endif // MAHASISWA_H
