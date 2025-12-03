#ifndef LAMARAN_H
#define LAMARAN_H

#include "MLL_STRUCTS.h"

// DEKLARASI FUNGSI LAMARAN (RELASI)
// --- FUNGSI TAMPILAN SPESIFIK BARU ---
void insertRelasi(ListParent &L_Parent, ListChild &L_Child, int ID_Lowongan, string NIM_Mhs, string Nama_Mhs, int ID_Lamaran_Baru, string CV_Path);
int hitungSkorATS(string filePath);
void showStatusLamaranMahasiswa(ListParent L_Parent, string NIM_Target);
void showRekapLamaranPerusahaan(ListParent L_Parent);
void editStatusDosen(ListParent &L_Parent, int ID_Lamaran_Target, int Status_Baru);
void editStatusPerusahaan(ListParent &L_Parent, int ID_Lamaran_Target);
void showLowonganDanPelamar(ListParent L_Parent);
void showRekapLamaranDosen(ListParent L_Parent);
int countNotifikasi(ListParent L_Parent, string NIM);
void showNotifikasi(ListParent L_Parent, string NIM);

#endif // LAMARAN_H
