# Sistem Layanan Magang (ATS) - MLL Tipe B

![C++](https://img.shields.io/badge/Language-C++-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)

Sistem Layanan Magang adalah aplikasi berbasis konsol (CLI) yang dibangun menggunakan bahasa C++ untuk memfasilitasi proses pelamaran kerja magang mahasiswa. Sistem ini menggunakan struktur data **Multi-Linked List (MLL)** untuk mengelola relasi antara Lowongan (Parent) dan Pelamar (Child).

## 🚀 Fitur Utama

### 1. Multi-Role System
- **Mahasiswa**: Input data diri, melihat lowongan, upload CV, cari lowongan, dan menerima notifikasi penerimaan.
- **Dosen**: Memverifikasi lamaran, cari mahasiswa, hapus mahasiswa, dan melihat rekapitulasi pelamar.
- **Perusahaan**: Membuka lowongan, melihat skor ATS pelamar, review CV, cari mahasiswa, hapus lowongan, dan memberikan keputusan (Terima/Tolak).
- **Admin**: Mengelola user, cari/hapus mahasiswa, cari/hapus lowongan, dan melihat seluruh data sistem.

### 2. Applicant Tracking System (ATS) 🤖
- **CV Scoring**: Sistem otomatis memindai file CV (PDF/TXT) untuk mencari kata kunci relevan (misal: "C++", "Teamwork", "Python").
- **Binary Scanning**: Mendukung pembacaan raw content dari file PDF.
- **Skor**: Setiap kata kunci memberikan poin tambahan pada skor pelamar.

### 3. Manajemen File & CV 📂
- **Upload CV**: Mendukung pemilihan file native Windows (File Picker) untuk format `.pdf` dan `.txt`.
- **File Storage Simulation**: File yang diupload otomatis disalin ke folder lokal `uploads/` dengan penamaan terstruktur.
- **Auto-Cleanup**: Folder `uploads/` otomatis dibersihkan saat aplikasi ditutup untuk menghemat penyimpanan.
- **Integrated Review**: Perusahaan dapat membuka file CV pelamar langsung dari aplikasi sebelum memberikan keputusan.

### 4. Notifikasi Real-time 🔔
- Mahasiswa mendapatkan notifikasi "Pesan" di menu utama jika lamaran mereka diterima oleh perusahaan.

## 🛠️ Teknologi yang Digunakan
- **Bahasa**: C++ (Standard Library)
- **Struktur Data**: Multi-Linked List (MLL) Tipe B
- **OS API**: Windows API (`windows.h`, `commdlg.h`) untuk File Dialog dan File Operations.

## 📦 Cara Menjalankan

### Prasyarat
- Compiler C++ (MinGW / G++)
- Sistem Operasi Windows

### Instalasi & Run
1.  Clone repository ini.
2.  **Cara Mudah (Recommended):**
    Cukup klik dua kali file `run.bat` atau jalankan via terminal:
    ```bash
    ./run.bat
    ```
    Script ini akan otomatis melakukan compile dan menjalankan aplikasi.

3.  **Cara Manual (Alternatif):**
    Jika ingin compile sendiri:
    ```bash
    g++ src/*.cpp -Iinclude -o app.exe -lcomdlg32
    ./app.exe
    ```

## 👥 Akun Demo (Login)

| Role | Username | Password |
| :--- | :--- | :--- |
| **Mahasiswa** | `muhamadalwansuryadi` | `0104` |
| **Dosen** | `dosen1` | `dosen123` |
| **Perusahaan** | `techcorp` | `123` |
| **Admin** | `admin` | `admin123` |

## 📝 Struktur Data (MLL Tipe B)

Sistem ini menggunakan **Multi-Linked List Tipe B**, di mana setiap Parent node memiliki pointer ke list relasi-nya sendiri.

### Komponen Utama
- **Parent (Lowongan)**: Linked List berisi data lowongan pekerjaan
- **Child (Mahasiswa)**: Linked List berisi data mahasiswa
- **Relasi (Lamaran)**: List yang *menempel* pada setiap Parent, menghubungkan ke Child

### Visualisasi Struktur MLL Tipe B

```mermaid
graph TB
    subgraph ListParent ["List Parent (Lowongan)"]
        P1["P1<br/>Data Scientist<br/>TechCorp"]
        P2["P2<br/>Mobile Dev<br/>GameDev"]
        P3["P3<br/>Backend Eng<br/>ServerX"]
        P1 -->|next| P2 -->|next| P3
    end

    subgraph ListChild ["List Child (Mahasiswa)"]
        C1["C1<br/>103032400104<br/>Alwan"]
        C2["C2<br/>103032400105<br/>Fathur"]
        C3["C3<br/>103032400106<br/>Azriel"]
        C1 -->|next| C2 -->|next| C3
    end

    subgraph Relasi1 ["Relasi P1"]
        R1["R1<br/>Lamaran #1"]
        R2["R2<br/>Lamaran #2"]
        R1 -->|next| R2
    end

    subgraph Relasi2 ["Relasi P2"]
        R3["R3<br/>Lamaran #3"]
    end

    %% Parent to Relasi (first_relasi pointer)
    P1 -.->|first_relasi| R1
    P2 -.->|first_relasi| R3
    P3 -.->|first_relasi| NULL1((null))

    %% Relasi to Child (ptr_child pointer)
    R1 ==>|ptr_child| C1
    R2 ==>|ptr_child| C2
    R3 ==>|ptr_child| C1

    %% Styling
    classDef parent fill:#4fc3f7,stroke:#0288d1,color:#000
    classDef child fill:#81c784,stroke:#388e3c,color:#000
    classDef relasi fill:#ffb74d,stroke:#f57c00,color:#000
    classDef null fill:#e0e0e0,stroke:#9e9e9e,color:#666

    class P1,P2,P3 parent
    class C1,C2,C3 child
    class R1,R2,R3 relasi
    class NULL1 null
```

### Penjelasan Pointer
| Pointer | Dari | Ke | Fungsi |
|---------|------|-----|--------|
| `next` | Parent/Child/Relasi | Node sejenis berikutnya | Traversal horizontal |
| `first_relasi` | Parent | Relasi pertama | Akses lamaran di lowongan tsb |
| `ptr_child` | Relasi | Child | Menunjuk mahasiswa pelamar |

> **Mengapa Tipe B?** Setiap lowongan (Parent) langsung memiliki akses ke daftar pelamarnya tanpa perlu traversal list relasi global.

## � Fungsi Utama per Modul

### `mahasiswa.cpp`
| Fungsi | Deskripsi |
|--------|-----------|  
| `createListChild()` | Inisialisasi list mahasiswa |
| `alokasiChild()` | Alokasi node baru mahasiswa |
| `insertChild()` | Insert mahasiswa ke list |
| `findChildByNIM()` | Cari mahasiswa berdasarkan NIM |
| `menuCariMahasiswa()` | Menu interaktif pencarian mahasiswa |
| `deleteChildByNIM()` | Hapus mahasiswa beserta semua relasi |
| `cleanupRelasiBeforeChildDeletion()` | Cleanup relasi sebelum hapus child |

### `lowongan.cpp`
| Fungsi | Deskripsi |
|--------|-----------|  
| `createListParent()` | Inisialisasi list lowongan |
| `alokasiParent()` | Alokasi node baru lowongan |
| `insertParent()` | Insert lowongan ke list |
| `findParent()` | Cari lowongan berdasarkan ID |
| `menuCariLowongan()` | Menu interaktif pencarian lowongan |
| `deleteParentByID()` | Hapus lowongan beserta semua lamaran |
| `importLowonganFromAPI()` | Import lowongan dari file hasil API |

### `lamaran.cpp`
| Fungsi | Deskripsi |
|--------|-----------|  
| `insertRelasi()` | Insert lamaran baru (relasi Parent-Child) |
| `editStatusDosen()` | Update status verifikasi dosen |
| `editStatusPerusahaan()` | Update keputusan perusahaan |
| `showStatusLamaranMahasiswa()` | Tampilkan status lamaran per mahasiswa |
| `showRekapLamaranPerusahaan()` | Rekap lamaran dengan ATS Score |
| `countNotifikasi()` | Hitung jumlah notifikasi penerimaan |
| `showNotifikasi()` | Tampilkan detail notifikasi |

## 📊 Alur Program

```mermaid
graph LR
    %% Setup Styling High Contrast
    classDef default fill:#ffffff,stroke:#333333,stroke-width:1px,color:#000000;
    classDef menu fill:#e1f5fe,stroke:#01579b,stroke-width:2px,color:#000000;
    classDef term fill:#f3e5f5,stroke:#4a148c,stroke-width:2px,color:#000000;
    classDef auth fill:#fff3e0,stroke:#e65100,stroke-width:2px,color:#000000;
    classDef new fill:#c8e6c9,stroke:#2e7d32,stroke-width:2px,color:#000000;

    %% Main Flow
    Start([Start]) --> Init[Init Data] --> LoginStart{Login Menu}

    subgraph Auth_System [Authentication]
        LoginStart -- "1. Masuk" --> Auth{Cek User}
        LoginStart -- "2. Daftar" --> Register[Register] --> LoginStart
        Auth -- "Gagal" --> LoginStart
    end

    Auth -- "Sukses" --> RoleCheck{Cek Role}

    %% Role Branches
    RoleCheck --> M_Menu
    RoleCheck --> D_Menu
    RoleCheck --> P_Menu
    RoleCheck --> A_Menu

    subgraph R_M [Mahasiswa]
        M_Menu[Menu Mahasiswa] --> M1[1. Input Data]
        M_Menu --> M2[2. Lihat Lowongan]
        M_Menu --> M3[3. Ajukan Lamaran]
        M_Menu --> M4[4. Cek Status]
        M_Menu --> M5[5. Cari Lowongan]:::new
        M_Menu --> M6[6. Notifikasi]
    end

    subgraph R_D [Dosen]
        D_Menu[Menu Dosen] --> D1[1. Verifikasi]
        D_Menu --> D2[2. Lihat Lowongan]
        D_Menu --> D3[3. Rekap Mhs]
        D_Menu --> D4[4. Cari Mhs]:::new
        D_Menu --> D5[5. Hapus Mhs]:::new
    end

    subgraph R_P [Perusahaan]
        P_Menu[Menu Perusahaan] --> P1[1. Input Lowongan]
        P_Menu --> P2[2. Keputusan]
        P_Menu --> P3[3. Rekap ATS]
        P_Menu --> P4[4. Cari API]
        P_Menu --> P5[5. Cari Mhs]:::new
        P_Menu --> P6[6. Hapus Lowongan]:::new
    end

    subgraph R_A [Admin]
        A_Menu[Menu Admin] --> A1[1. Kelola User]
        A_Menu --> A2[2. Lihat Data M:N]
        A_Menu --> A3[3. Rekap Total]
        A_Menu --> A4[4. Cari Mhs]:::new
        A_Menu --> A5[5. Cari Lowongan]:::new
        A_Menu --> A6[6. Hapus Mhs]:::new
        A_Menu --> A7[7. Hapus Lowongan]:::new
    end

    %% Logout Routing
    M_Menu -.-> Logout(Logout)
    D_Menu -.-> Logout
    P_Menu -.-> Logout
    A_Menu -.-> Logout
    
    Logout --> LoginStart
    LoginStart -- "0. Keluar" --> End([End])

    %% Apply Styles
    class M_Menu,D_Menu,P_Menu,A_Menu menu;
    class Start,End,Logout term;
    class LoginStart,Auth,Register,RoleCheck auth;
```

---
*Dibuat untuk Tugas Besar Struktur Data - Telkom University.*
