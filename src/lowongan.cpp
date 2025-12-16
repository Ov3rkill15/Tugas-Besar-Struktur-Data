#include "lowongan.h"
#include <cstring>
#include <limits>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>  // Untuk srand(time(nullptr))

using namespace std;

void createListParent(ListParent &L) {
    L.first = nullptr;
}

address_parent alokasiParent(int ID, string Posisi, string Perusahaan, float IpkMin, int Kuota) {
    address_parent P = new NodeParent;
    if (P != nullptr) {
        P->info.id_lowongan = ID;
        P->info.posisi = Posisi;
        P->info.nama_perusahaan = Perusahaan;
        P->info.ipk_min = IpkMin;
        P->info.kuota = Kuota;
        P->next = nullptr;
        P->first_relasi = nullptr;
    }
    return P;
}

void insertParent(ListParent &L, address_parent P_Baru) {
    if (L.first == nullptr) L.first = P_Baru;
    else { address_parent Last = L.first; while (Last->next != nullptr) Last = Last->next; Last->next = P_Baru; }
    cout << "   [INFO] Lowongan ID " << P_Baru->info.id_lowongan << " (" << P_Baru->info.posisi << ") berhasil ditambahkan." << endl;
}

address_parent findParent(ListParent L, int ID_Target) {
    address_parent P = L.first;
    while (P != nullptr) {
        if (P->info.id_lowongan == ID_Target) return P;
        P = P->next;
    }
    return nullptr;
}

void menuCariLowongan(ListParent L) {
    int id_cari;

    cout << "\n\033[36m+============================================+\033[0m" << endl;
    cout << "|      \033[33mCARI LOWONGAN BERDASARKAN ID\033[0m         |" << endl;
    cout << "\033[36m+============================================+\033[0m" << endl;
    cout << "| Masukkan ID Lowongan: ";

    if (!(cin >> id_cari)) {
        cout << "\033[91m| [ERROR] Input tidak valid!\033[0m" << endl;
        cout << "\033[36m+============================================+\033[0m" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    address_parent P_Found = findParent(L, id_cari);

    if (P_Found != nullptr) {
        cout << "\033[32m| [DITEMUKAN]\033[0m" << endl;
        cout << "\033[36m+--------------------------------------------+\033[0m" << endl;
        cout << "| ID        : \033[97m" << P_Found->info.id_lowongan << "\033[0m" << endl;
        cout << "| Posisi    : \033[93m" << P_Found->info.posisi << "\033[0m" << endl;
        cout << "| Perusahaan: \033[95m" << P_Found->info.nama_perusahaan << "\033[0m" << endl;
        cout << "| IPK Min   : \033[92m" << fixed << setprecision(2) << P_Found->info.kuota << "\033[0m" << endl;
        cout << "\033[36m+============================================+\033[0m" << endl;
    } else {
        cout << "\033[91m| [TIDAK DITEMUKAN] ID " << id_cari << "\033[0m" << endl;
        cout << "\033[36m+============================================+\033[0m" << endl;
    }
}

void showLowongan(ListParent L_Parent) {
    address_parent P = L_Parent.first;
    int count = 0;
    
    cout << "\n\033[36m+============================================+\033[0m" << endl;
    cout << "|        \033[33mDAFTAR LOWONGAN TERSEDIA\033[0m           |" << endl;
    cout << "\033[36m+============================================+\033[0m" << endl;
    
    if (P == nullptr) { 
        cout << "| \033[91mBelum ada lowongan tersedia.\033[0m" << endl;
        cout << "\033[36m+============================================+\033[0m" << endl;
        return; 
    }
    
    while (P != nullptr) {
        count++;
        cout << "\033[36m+--------------------------------------------+\033[0m" << endl;
        cout << "| \033[92m[ID: " << P->info.id_lowongan << "]\033[0m " << P->info.posisi << endl;
        cout << "|   @ \033[95m" << P->info.nama_perusahaan << "\033[0m" << endl;
        cout << "|   IPK Min: \033[93m" << fixed << setprecision(2) << P->info.ipk_min << "\033[0m | Kuota: \033[92m" << P->info.kuota << "\033[0m posisi" << endl;
        P = P->next;
    }
    cout << "\033[36m+============================================+\033[0m" << endl;
    cout << "| Total: \033[92m" << count << "\033[0m lowongan tersedia" << endl;
    cout << "\033[36m+============================================+\033[0m" << endl;
}

void menuInsertParent(ListParent &L, int &ID_Counter) {
    char posisi_input[50];
    char perusahaan_input[100];
    float ipk_min_input;
    int kuota_input;
    int id_baru;

    cout << "\n\033[36m+============================================+\033[0m" << endl;
    cout << "|         \033[33mINPUT LOWONGAN BARU\033[0m               |" << endl;
    cout << "\033[36m+============================================+\033[0m" << endl;

    id_baru = ID_Counter;
    cout << "| ID Lowongan Otomatis: \033[92m" << id_baru << "\033[0m" << endl;
    cout << "\033[36m+--------------------------------------------+\033[0m" << endl;

    cout << "| Posisi/Role: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(posisi_input, 50);

    cout << "| Nama Perusahaan: ";
    cin.getline(perusahaan_input, 100);

    cout << "| IPK Minimum (0.00-4.00): ";
    if (!(cin >> ipk_min_input) || ipk_min_input < 0 || ipk_min_input > 4) {
        cout << "\033[91m| [ERROR] IPK tidak valid (harus 0.00-4.00).\033[0m" << endl;
        cout << "\033[36m+============================================+\033[0m" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cout << "| Kuota Posisi: ";
    if (!(cin >> kuota_input) || kuota_input < 1) {
        cout << "\033[91m| [ERROR] Kuota tidak valid (minimal 1).\033[0m" << endl;
        cout << "\033[36m+============================================+\033[0m" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    address_parent P_Baru = alokasiParent(id_baru, posisi_input, perusahaan_input, ipk_min_input, kuota_input);
    insertParent(L, P_Baru);

    cout << "\033[36m+============================================+\033[0m" << endl;
    ID_Counter++;
}

void deleteParentByID(ListParent &L_Parent, int ID_Lowongan_Target) {
    address_parent P_Before = nullptr;
    address_parent P_Current = L_Parent.first;
    bool target_found = false;

    while (P_Current != nullptr && !target_found) {
        if (P_Current->info.id_lowongan == ID_Lowongan_Target) {
            target_found = true;
        } else {
            P_Before = P_Current;
            P_Current = P_Current->next;
        }
    }

    if (!target_found) {
        cout << " Gagal menghapus. Lowongan ID " << ID_Lowongan_Target << " tidak ditemukan." << endl;
        return;
    }
    address_relasi R_Del = P_Current->first_relasi;
    while (R_Del != nullptr) {
        address_relasi R_Next = R_Del->next;
        delete R_Del; // Dealokasi Node Relasi
        R_Del = R_Next;
    }

    if (P_Current == L_Parent.first) {
        L_Parent.first = P_Current->next;
    } else {
        if (P_Before != nullptr) {
            P_Before->next = P_Current->next;
        }
    }

    cout << "Lowongan ID " << ID_Lowongan_Target << " berhasil dihapus beserta semua Lamaran terkait." << endl;
    delete P_Current;
}
void importLowonganFromAPI(ListParent &L, string filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cout << "Gagal membuka file hasil pencarian API." << endl;
        return;
    }

    string line;
    int count = 0;
    struct JobTemp {
        string title;
        string company;
        double gpa;
    } jobs[20]; // Max 20 results

    cout << "\n--- HASIL PENCARIAN API ---" << endl;
    while (getline(file, line) && count < 20) {
        stringstream ss(line);
        string segment;
        vector<string> seglist;

        while (getline(ss, segment, '|')) {
            seglist.push_back(segment);
        }

        if (seglist.size() >= 3) {
            jobs[count].title = seglist[0];
            jobs[count].company = seglist[1];
            jobs[count].gpa = stod(seglist[2]);
            
            cout << count + 1 << ". " << jobs[count].title << " at " << jobs[count].company << " (Min GPA: " << jobs[count].gpa << ")" << endl;
            count++;
        }
    }
    file.close();

    if (count == 0) {
        cout << "Tidak ada lowongan ditemukan." << endl;
        return;
    }

    int choice;
    cout << "\nPilih nomor lowongan untuk ditambahkan (0 untuk batal): ";
    cin >> choice;

    if (choice > 0 && choice <= count) {
        // Generate sequential ID
        int maxID = 100;
        address_parent P = L.first;
        while (P != nullptr) {
            if (P->info.id_lowongan > maxID) maxID = P->info.id_lowongan;
            P = P->next;
        }
        int newID = maxID + 1;

        insertParent(L, alokasiParent(newID, jobs[choice-1].title, jobs[choice-1].company, jobs[choice-1].gpa, 5)); // Default kuota = 5
        cout << "Lowongan berhasil ditambahkan dengan ID " << newID << "!" << endl;
    } else {
        cout << "Dibatalkan." << endl;
    }
}

// Fungsi C++ untuk menggantikan Python script search_jobs.py
void searchJobsOnline(ListParent &L, string keyword) {
    cout << "\n\033[36m+============================================+\033[0m" << endl;
    cout << "| \033[33mPENCARI LOWONGAN ONLINE\033[0m" << endl;
    cout << "\033[36m+============================================+\033[0m" << endl;
    cout << "| Mencari lowongan dengan kata kunci: \033[92m" << keyword << "\033[0m" << endl;
    cout << "\033[36m+--------------------------------------------+\033[0m" << endl;
    
    // Seed random dengan waktu
    srand(time(nullptr));
    
    // Mock data - prefix positions
    string prefixes[] = {"Junior", "Senior", "Lead", "Staff", "Associate"};
    string suffixes[] = {"Developer", "Engineer", "Specialist", "Analyst", "Architect"};
    string companies[] = {"TechCorp Indonesia", "InnovateX Labs", "GlobalSystems", "StartUpOne", "FutureNet", "DigiCorp", "CloudMaster"};
    
    int numPrefixes = 5;
    int numSuffixes = 5;
    int numCompanies = 7;
    
    // Generate 3-6 mock results
    int numResults = 3 + rand() % 4;
    
    struct JobResult {
        string title;
        string company;
        double gpa;
        int kuota;
    } results[10];
    
    cout << "| \033[95mDitemukan " << numResults << " lowongan:\033[0m" << endl;
    cout << "\033[36m+--------------------------------------------+\033[0m" << endl;
    
    for (int i = 0; i < numResults; i++) {
        // Generate job title berdasarkan keyword
        string prefix = prefixes[rand() % numPrefixes];
        string suffix = suffixes[rand() % numSuffixes];
        results[i].title = prefix + " " + keyword + " " + suffix;
        results[i].company = companies[rand() % numCompanies];
        results[i].gpa = 2.5 + (rand() % 16) / 10.0; // GPA 2.5 - 4.0
        results[i].kuota = 2 + rand() % 8; // Kuota 2-9
        
        cout << "| \033[92m[" << (i + 1) << "]\033[0m " << results[i].title << endl;
        cout << "|     @ " << results[i].company << " (IPK: " << fixed << setprecision(2) << results[i].gpa << " | Kuota: " << results[i].kuota << ")" << endl;
    }
    
    cout << "\033[36m+--------------------------------------------+\033[0m" << endl;
    cout << "| Pilih nomor untuk menambahkan (\033[91m0 = batal\033[0m): ";
    
    int choice;
    cin >> choice;
    
    if (choice > 0 && choice <= numResults) {
        // Generate sequential ID (cari ID tertinggi + 1)
        int maxID = 100; // Start dari 100 jika list kosong
        address_parent P = L.first;
        while (P != nullptr) {
            if (P->info.id_lowongan > maxID) {
                maxID = P->info.id_lowongan;
            }
            P = P->next;
        }
        int newID = maxID + 1;
        
        insertParent(L, alokasiParent(newID, results[choice-1].title, results[choice-1].company, results[choice-1].gpa, results[choice-1].kuota));
        
        cout << "\033[32m| [BERHASIL] Lowongan ditambahkan dengan ID: " << newID << "\033[0m" << endl;
        cout << "\033[36m+============================================+\033[0m" << endl;
    } else {
        cout << "\033[33m| Dibatalkan.\033[0m" << endl;
        cout << "\033[36m+============================================+\033[0m" << endl;
    }
}
