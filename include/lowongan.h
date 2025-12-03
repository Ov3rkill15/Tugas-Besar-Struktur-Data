#ifndef LOWONGAN_H
#define LOWONGAN_H

#include "mll_structs.h" // <-- PENTING! Agar ListParent dikenali

// DEKLARASI FUNGSI LOWONGAN (PARENT)
void createListParent(ListParent &L);
address_parent alokasiParent(int ID, string Posisi, string Perusahaan, float Kuota);
void insertParent(ListParent &L, address_parent P_Baru);
address_parent findParent(ListParent L, int ID_Target);
void showLowongan(ListParent L_Parent);
void showMahasiswa(ListChild L_Child);
// --- FUNGSI INPUT UTAMA (INTERAKTIF) ---
void menuInsertParent(ListParent &L, int &ID_Counter);
#endif // LOWONGAN_H
