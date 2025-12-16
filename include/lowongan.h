#ifndef LOWONGAN_H
#define LOWONGAN_H
#include "mll_structs.h" // <-- PENTING! Agar ListParent dikenali
void createListParent(ListParent &L);
address_parent alokasiParent(int ID, string Posisi, string Perusahaan, float Kuota);
void insertParent(ListParent &L, address_parent P_Baru);
address_parent findParent(ListParent L, int ID_Target);
void menuCariLowongan(ListParent L);
void showLowongan(ListParent L_Parent);
void showMahasiswa(ListChild L_Child);
void menuInsertParent(ListParent &L, int &ID_Counter);
void deleteParentByID(ListParent &L_Parent, int ID_Lowongan_Target);
void importLowonganFromAPI(ListParent &L, string filePath);
void searchJobsOnline(ListParent &L, string keyword);  // Fungsi C++ pengganti Python
#endif // LOWONGAN_H
