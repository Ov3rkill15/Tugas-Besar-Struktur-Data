# Sistem Layanan Magang (ATS) - MLL Tipe B

![C++](https://img.shields.io/badge/Language-C++-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)

Sistem Layanan Magang adalah aplikasi berbasis konsol (CLI) yang dibangun menggunakan bahasa C++ untuk memfasilitasi proses pelamaran kerja magang mahasiswa. Sistem ini menggunakan struktur data **Multi-Linked List (MLL)** untuk mengelola relasi antara Lowongan (Parent) dan Pelamar (Child).

## 🚀 Fitur Utama

### 1. Multi-Role System
- **Mahasiswa**: Input data diri, melihat lowongan, upload CV, dan menerima notifikasi penerimaan.
- **Dosen**: Memverifikasi lamaran mahasiswa dan melihat rekapitulasi pelamar.
- **Perusahaan**: Membuka lowongan, melihat skor ATS pelamar, review CV, dan memberikan keputusan (Terima/Tolak).
- **Admin**: Mengelola user dan melihat seluruh data sistem.

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

## 📝 Struktur Data (MLL)
- **Parent**: Lowongan Pekerjaan (Double Linked List)
- **Child**: Data Mahasiswa (Single Linked List)
- **Relasi**: Lamaran (Single Linked List yang menghubungkan Parent & Child)

## 📊 Alur Program

```mermaid
graph TD
    %% Nodes
    Start([Start Application])
    Init["Initialize Data Structures\n(ListParent, ListChild, Users)"]
    MkDir[Create 'uploads' Directory]
    
    %% Login System
    subgraph Login_System [Login System]
        LoginStart{Login Menu}
        InputLogin[/"Input Username & Password"/]
        InputRegister[/"Input New User Data"/]
        Auth{Authenticate?}
        RegisterAction["Add to Users Array\n(Default: Mahasiswa)"]
        CheckExit{Exit?}
    end

    %% Main Logic
    Cleanup[Cleanup Temporary Files]
    End([End Application])

    %% Role Branching
    subgraph Dashboard [Main Dashboard]
        RoleCheck{Check User Role}
        
        %% Mahasiswa Flow
        subgraph Role_Mahasiswa [Role: Mahasiswa]
            M_Menu[Mahasiswa Menu]
            M_Input[1. Input Data Diri]
            M_View[2. Lihat Lowongan]
            M_Apply[3. Ajukan Lamaran + Upload CV]
            M_Status[4. Cek Status Lamaran]
            M_Msg[5. Pesan / Notifikasi]
        end

        %% Dosen Flow
        subgraph Role_Dosen [Role: Dosen]
            D_Menu[Dosen Menu]
            D_Verify[1. Verifikasi Lamaran]
            D_View[2. Lihat Lowongan]
            D_Recap[3. Rekap Lamaran Mahasiswa]
        end

        %% Perusahaan Flow
        subgraph Role_Perusahaan [Role: Perusahaan]
            P_Menu[Perusahaan Menu]
            P_Input[1. Input Lowongan Baru]
            P_Decide[2. Keputusan Lamaran]
            P_Recap[3. Rekap Lamaran Masuk]
            P_API["4. Cari Lowongan Online (API)"]
        end

        %% Admin Flow
        subgraph Role_Admin [Role: Admin]
            A_Menu[Admin Menu]
            A_Manage["1. Kelola User (Admin Panel)"]
            A_ViewAll["2. Lihat Semua Data (M:N)"]
            A_Recap[3. Rekap Semua Lamaran]
            
            subgraph Admin_Panel [Admin Panel]
                AP_Add[Tambah User]
                AP_Edit[Ubah User]
                AP_Del[Hapus User]
                AP_Log[Lihat Riwayat]
            end
        end
    end

    Logout{Logout?}

    %% Connections
    Start --> Init --> MkDir --> LoginStart
    
    %% Login Logic
    LoginStart -- "1. Masuk" --> InputLogin
    LoginStart -- "2. Daftar" --> InputRegister
    LoginStart -- "0. Keluar" --> CheckExit
    
    InputRegister --> RegisterAction --> LoginStart
    InputLogin --> Auth
    Auth -- "Success" --> RoleCheck
    Auth -- "Fail" --> LoginStart
    CheckExit -- "Yes" --> Cleanup --> End

    %% Role Routing
    RoleCheck -- "MAHASISWA" --> M_Menu
    RoleCheck -- "DOSEN" --> D_Menu
    RoleCheck -- "PERUSAHAAN" --> P_Menu
    RoleCheck -- "ADMIN" --> A_Menu

    %% Mahasiswa Actions
    M_Menu --> M_Input
    M_Menu --> M_View
    M_Menu --> M_Apply
    M_Menu --> M_Status
    M_Menu --> M_Msg

    %% Dosen Actions
    D_Menu --> D_Verify
    D_Menu --> D_View
    D_Menu --> D_Recap

    %% Perusahaan Actions
    P_Menu --> P_Input
    P_Menu --> P_Decide
    P_Menu --> P_Recap
    P_Menu --> P_API

    %% Admin Actions
    A_Menu --> A_Manage
    A_Menu --> A_ViewAll
    A_Menu --> A_Recap
    A_Manage --> Admin_Panel

    %% Loop Back / Logout
    M_Input & M_View & M_Apply & M_Status & M_Msg --> Logout
    D_Verify & D_View & D_Recap --> Logout
    P_Input & P_Decide & P_Recap & P_API --> Logout
    A_Manage & A_ViewAll & A_Recap --> Logout

    Logout -- "No (Continue)" --> RoleCheck
    Logout -- "Yes (Logout)" --> LoginStart

    %% Styling
    classDef startend fill:#f9f,stroke:#333,stroke-width:2px;
    classDef process fill:#e1f5fe,stroke:#01579b,stroke-width:2px;
    classDef decision fill:#fff9c4,stroke:#fbc02d,stroke-width:2px;
    classDef role fill:#e8f5e9,stroke:#2e7d32,stroke-width:2px;

    class Start,End,Cleanup startend;
    class Init,MkDir,RegisterAction,M_Input,M_View,M_Apply,M_Status,M_Msg,D_Verify,D_View,D_Recap,P_Input,P_Decide,P_Recap,P_API,A_Manage,A_ViewAll,A_Recap process;
    class LoginStart,Auth,CheckExit,RoleCheck,Logout decision;
    class M_Menu,D_Menu,P_Menu,A_Menu role;
```

---
*Dibuat untuk Tugas Besar Struktur Data - Telkom University.*
