#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define maksPenyakit 200
#define fileData "penyakit.txt"
#define jumlahPenyakitBawaan 17

 // Struktur untuk data penyakit
typedef struct {
    char nama[50];        // Nama penyakit
    char jenis[10];       // "fisik" atau "mental"
    char obat[100];       // Rekomendasi obat
    char terapi[100];     // Rekomendasi terapi (hanya untuk penyakit mental, "-" untuk fisik)
} Penyakit;

// Daftar penyakit
Penyakit daftarPenyakit[maksPenyakit];
int jumlahPenyakit = 0;

// Variabel untuk menghitung statistik aplikasi
int totalAkses = 0;
int penyakitDitambahkan = 0;
int penyakitDihapus = 0;
int pencarianDilakukan = 0;
int pencarianBerhasil = 0;
int pencarianGagal = 0;
int jumlahLoginAdmin = 0;
int jumlahAksesUser = 0;

// Data login admin 
char usernameAdmin[] = "admin";
char passwordAdmin[] = "admin123";

// --------------------- DATA PENYAKIT BAWANAN ---------------------
Penyakit penyakitBawaan[] = { 
    // Penyakit Fisik
    {"Diabetes Melitus", "fisik", "Kayu Manis, Kunyit", "-"},
    {"Hipertensi", "fisik", "Bawang Putih, Biji Anggur", "-"},
    {"Pneumonia", "fisik", "Jahe, Madu", "-"},
    {"Flu Biasa", "fisik", "Echinacea, Peppermint", "-"},
    {"Migrain", "fisik", "Feverfew, Butterbur", "-"},
    {"Arthritis", "fisik", "Turmeric, Boswellia", "-"},
    {"Asma", "fisik", "Madu, Minyak Eucalyptus", "-"},
    {"Obesitas", "fisik", "Green Tea, Garcinia Cambogia", "-"},
    {"Asam Urat", "fisik", "Ceri, Jahe", "-"},
    {"Anemia", "fisik", "Biji Labu, Spirulina", "-"},
    {"Rheumatoid Arthritis", "fisik", "Kulit Pohon Zaitun, Omega-3", "-"},
    
    // Penyakit Mental
    {"Depresi", "mental", "Ashwagandha, St. John’s Wort", "Terapi Kognitif Perilaku"},
    {"Gangguan Kecemasan", "mental", "Lavender, Valerian Root", "Terapi Relaksasi"},
    {"Skizofrenia", "mental", "Ginkgo Biloba, Omega-3", "Terapi Psikososial"},
    {"Gangguan Bipolar", "mental", "Rhodiola Rosea, Magnesium", "Terapi Psikoterapi"},
    {"Insomnia", "mental", "Chamomile, Melatonin alami", "Terapi Relaksasi"},
    {"Gangguan Tidur", "mental", "Passionflower, Lemon Balm", "Terapi Relaksasi"}
};




// --------------------- FUNGSI PEMBANTU ---------------------

void bersihkanBuffer() {
    // Membersihkan input buffer agar scanf/fgets tidak bermasalah
    int karakter;
    while ((karakter = getchar()) != '\n' && karakter != EOF) {}
}

int apakahPenyakitAda(const char *namaPenyakit) {
    // Memeriksa apakah penyakit sudah ada
    for (int i = 0; i < jumlahPenyakit; i++) {
        if (strcasecmp(daftarPenyakit[i].nama, namaPenyakit) == 0) {
            return 1; // Ada
        }
    }
    return 0; // Tidak ada
}

int bandingkanPenyakit(const void *a, const void *b) {
    Penyakit *p1 = (Penyakit *)a;
    Penyakit *p2 = (Penyakit *)b;
    return strcasecmp(p1->nama, p2->nama);
}

void simpanData() {
    FILE *file = fopen(fileData, "w");
    if (file == NULL) {
        //Jika File tidak dapat dibuka tampilkan error
        printf("Error: Tidak bisa membuka file untuk menyimpan data.\n");
        return;
    }

    for (int i = 0; i < jumlahPenyakit; i++) {
        fprintf(file, "%s,%s,%s,%s\n",
                daftarPenyakit[i].nama,
                daftarPenyakit[i].jenis,
                daftarPenyakit[i].obat,
                daftarPenyakit[i].terapi);
    }

    fclose(file);
}

void muatData() {
    FILE *file = fopen(fileData, "r");
    if (file == NULL) {
        // Jika File tidak ada, muat data bawaan
        printf("File '%s' tidak ditemukan. Menggunakan data penyakit bawaan.\n", fileData);
        for (int i = 0; i < jumlahPenyakitBawaan && jumlahPenyakit < maksPenyakit; i++) {
            daftarPenyakit[jumlahPenyakit++] = penyakitBawaan[i];
        }
        simpanData();
        return;
    }

    while (fscanf(file, "%49[^,],%9[^,],%99[^,],%99[^\n]\n",
                  daftarPenyakit[jumlahPenyakit].nama,
                  daftarPenyakit[jumlahPenyakit].jenis,
                  daftarPenyakit[jumlahPenyakit].obat,
                  daftarPenyakit[jumlahPenyakit].terapi) != EOF) {
        jumlahPenyakit++;
        if (jumlahPenyakit >= maksPenyakit) {
            printf("Maksimum jumlah penyakit tercapai saat memuat data.\n");
            break;
        }
    }

    fclose(file);

    // Jika file ada tetapi kosong, muat data bawaan
    if (jumlahPenyakit == 0) {
        printf("File '%s' kosong. Memuat data penyakit bawaan.\n", fileData);
        for (int i = 0; i < jumlahPenyakitBawaan && jumlahPenyakit < maksPenyakit; i++) {
            daftarPenyakit[jumlahPenyakit++] = penyakitBawaan[i];
        }
        simpanData();
    }
}

void tambahPenyakit() {
    if (jumlahPenyakit >= maksPenyakit) {
        printf("Data penyakit sudah penuh.\n");
        return;
    }
    printf("=== Tambah Penyakit ===\n");

    Penyakit p;
    printf("Masukkan nama penyakit: ");
    fgets(p.nama, sizeof(p.nama), stdin);
    p.nama[strcspn(p.nama, "\n")] = '\0'; // Hapus newline

    // Cek duplikasi
    if (apakahPenyakitAda(p.nama)) {
        printf("Penyakit '%s' sudah ada dalam daftar.\n\n", p.nama);
        return;
    }

    // Jenis penyakit
    while (1) {
        printf("Jenis penyakit (fisik/mental): ");
        fgets(p.jenis, sizeof(p.jenis), stdin);
        p.jenis[strcspn(p.jenis, "\n")] = '\0';
        
        // Konversi ke huruf kecil untuk konsistensi
        for(int i = 0; p.jenis[i]; i++){
            p.jenis[i] = tolower(p.jenis[i]);
        }

        if (strcmp(p.jenis, "fisik") == 0 || strcmp(p.jenis, "mental") == 0) {
            break;
        } else {
            printf("Jenis penyakit harus 'fisik' atau 'mental'. Silakan coba lagi.\n");
        }
    }

    // Rekomendasi obat
    printf("Rekomendasi obat herbal: ");
    fgets(p.obat, sizeof(p.obat), stdin);
    p.obat[strcspn(p.obat, "\n")] = '\0';

    // Rekomendasi terapi jika mental, "-" jika fisik
    if (strcmp(p.jenis, "mental") == 0) {
        printf("Rekomendasi terapi: ");
        fgets(p.terapi, sizeof(p.terapi), stdin);
        p.terapi[strcspn(p.terapi, "\n")] = '\0';
    } else {
        strcpy(p.terapi, "-"); // Untuk penyakit fisik, set terapi "-"
    }

    // Tambahkan penyakit ke daftar
    daftarPenyakit[jumlahPenyakit++] = p;
    penyakitDitambahkan++; // Update statistik

    simpanData(); // Simpan data ke file

    printf("\nPenyakit berhasil ditambahkan!\n\n");
}

void hapusPenyakit() {
    if (jumlahPenyakit == 0) {
        printf("Belum ada data penyakit.\n");
        return;
    }

    int ulang = 1;
    while (ulang) {
        printf("=== Hapus Penyakit ===\n");

        // Buat salinan daftar penyakit untuk disortir tanpa mengubah urutan asli
        Penyakit penyakitDisortir[maksPenyakit];
        memcpy(penyakitDisortir, daftarPenyakit, sizeof(Penyakit) * jumlahPenyakit);
        qsort(penyakitDisortir, jumlahPenyakit, sizeof(Penyakit), bandingkanPenyakit);

        // Tampilkan daftar penyakit yang disortir dengan ID
        printf("Daftar Penyakit :\n");
        for (int i = 0; i < jumlahPenyakit; i++) {
            printf("%d. %s [%s]\n", i+1, penyakitDisortir[i].nama, penyakitDisortir[i].jenis);
        }

        // Meminta Admin untuk memasukkan ID penyakit yang ingin dihapus
        int idHapus;
        printf("\nMasukkan ID penyakit yang ingin dihapus: ");
        scanf("%d", &idHapus);
        bersihkanBuffer();

        if (idHapus < 1 || idHapus > jumlahPenyakit) {
            printf("ID tidak valid.\n\n");
            break;
        }

        // Cari indeks asli dari penyakit yang ingin dihapus
        char namaHapus[50];
        strcpy(namaHapus, penyakitDisortir[idHapus - 1].nama);

        int indeksHapus = -1;
        for (int i = 0; i < jumlahPenyakit; i++) {
            if (strcasecmp(daftarPenyakit[i].nama, namaHapus) == 0) {
                indeksHapus = i;
                break;
            }
        }

        if (indeksHapus == -1) {
            printf("Penyakit '%s' tidak ditemukan.\n\n", namaHapus);
            return;
        }

        // Geser data setelah indeksHapus ke depan
        for (int i = indeksHapus; i < jumlahPenyakit - 1; i++) {
            daftarPenyakit[i] = daftarPenyakit[i + 1];
        }
        jumlahPenyakit--;
        penyakitDihapus++; // Update statistik

        simpanData(); // Simpan data ke file

        printf("Penyakit '%s' berhasil dihapus.\n\n", namaHapus);
        ulang = 0;
    }
}

void editPenyakit() {
    if (jumlahPenyakit == 0) {
        printf("Belum ada data penyakit.\n");
        return;
    }
    printf("=== Edit Penyakit ===\n");

    // Buat salinan daftar penyakit untuk disortir tanpa mengubah urutan asli
    Penyakit penyakitDisortir[maksPenyakit];
    memcpy(penyakitDisortir, daftarPenyakit, sizeof(Penyakit) * jumlahPenyakit);
    qsort(penyakitDisortir, jumlahPenyakit, sizeof(Penyakit), bandingkanPenyakit);

    // Tampilkan daftar penyakit yang disortir dengan ID
    printf("Daftar Penyakit:\n");
    for (int i = 0; i < jumlahPenyakit; i++) {
        printf("%d. %s [%s]\n", i+1, penyakitDisortir[i].nama, penyakitDisortir[i].jenis);
    }

    // Meminta Admin untuk memasukkan ID penyakit yang ingin diedit
    int idEdit;
    printf("\nMasukkan ID penyakit yang ingin diedit: ");
    scanf("%d", &idEdit);
    bersihkanBuffer();

    if (idEdit < 1 || idEdit > jumlahPenyakit) {
        printf("ID tidak valid.\n\n");
        return;
    }

    // Cari indeks asli dari penyakit yang ingin diedit
    char namaEdit[50];
    strcpy(namaEdit, penyakitDisortir[idEdit - 1].nama);

    int indeksEdit = -1;
    for (int i = 0; i < jumlahPenyakit; i++) {
        if (strcasecmp(daftarPenyakit[i].nama, namaEdit) == 0) {
            indeksEdit = i;
            break;
        }
    }

    if (indeksEdit == -1) {
        printf("Penyakit '%s' tidak ditemukan.\n\n", namaEdit);
        return;
    }

    // Edit detail penyakit
    printf("=== Mengedit Penyakit: %s ===\n", daftarPenyakit[indeksEdit].nama);

    // Edit Nama Penyakit
    char masukan[100];
    printf("Masukkan nama penyakit baru (tekan Enter untuk tidak mengubah): ");
    fgets(masukan, sizeof(masukan), stdin);
    masukan[strcspn(masukan, "\n")] = '\0';
    if (strlen(masukan) > 0) {
        // Cek duplikasi
        if (apakahPenyakitAda(masukan) && strcasecmp(daftarPenyakit[indeksEdit].nama, masukan) != 0) {
            printf("Nama penyakit '%s' sudah ada dalam daftar. Tidak dapat mengubah nama.\n\n", masukan);
        } else {
            strcpy(daftarPenyakit[indeksEdit].nama, masukan);
            printf("Nama penyakit diperbarui.\n");
        }
    }

    // Edit Jenis Penyakit
    printf("Jenis penyakit saat ini: %s\n", daftarPenyakit[indeksEdit].jenis);
    printf("Masukkan jenis penyakit baru (fisik/mental) atau tekan Enter untuk tidak mengubah: ");
    fgets(masukan, sizeof(masukan), stdin);
    masukan[strcspn(masukan, "\n")] = '\0';
    if (strlen(masukan) > 0) {
        // Konversi ke huruf kecil
        for(int i = 0; masukan[i]; i++){
            masukan[i] = tolower(masukan[i]);
        }

        if (strcmp(masukan, "fisik") == 0 || strcmp(masukan, "mental") == 0) {
            strcpy(daftarPenyakit[indeksEdit].jenis, masukan);
            if (strcmp(masukan, "fisik") == 0) {
                strcpy(daftarPenyakit[indeksEdit].terapi, "-"); // Reset terapi untuk fisik
            }
            printf("Jenis penyakit diperbarui.\n");
        } else {
            printf("Jenis penyakit harus 'fisik' atau 'mental'. Tidak ada perubahan.\n");
        }
    }

    // Edit Rekomendasi Obat
    printf("Rekomendasi obat saat ini: %s\n", daftarPenyakit[indeksEdit].obat);
    printf("Masukkan rekomendasi obat baru atau tekan Enter untuk tidak mengubah: ");
    fgets(masukan, sizeof(masukan), stdin);
    masukan[strcspn(masukan, "\n")] = '\0';
    if (strlen(masukan) > 0) {
        strcpy(daftarPenyakit[indeksEdit].obat, masukan);
        printf("Rekomendasi obat diperbarui.\n");
    }

    // Edit Rekomendasi Terapi jika mental
    if (strcasecmp(daftarPenyakit[indeksEdit].jenis, "mental") == 0) {
        printf("Rekomendasi terapi saat ini: %s\n", daftarPenyakit[indeksEdit].terapi);
        printf("Masukkan rekomendasi terapi baru atau tekan Enter untuk tidak mengubah: ");
        fgets(masukan, sizeof(masukan), stdin);
        masukan[strcspn(masukan, "\n")] = '\0';
        if (strlen(masukan) > 0) {
            strcpy(daftarPenyakit[indeksEdit].terapi, masukan);
            printf("Rekomendasi terapi diperbarui.\n");
        }
    }

    simpanData(); // Simpan data ke file

    printf("\nPenyakit berhasil diedit.\n\n");
}

void tampilkanSemuaPenyakit() {
    if (jumlahPenyakit == 0) {
        printf("Belum ada data penyakit.\n\n");
        return;
    }
    printf("=== Daftar Semua Penyakit ===\n");
    for (int i = 0; i < jumlahPenyakit; i++) {
        printf("%d. %s [%s]\n", i+1, daftarPenyakit[i].nama, daftarPenyakit[i].jenis);
        printf("   Obat   : %s\n", daftarPenyakit[i].obat);
        if (strcasecmp(daftarPenyakit[i].jenis, "mental") == 0) {
            printf("   Terapi : %s\n", daftarPenyakit[i].terapi);
        }
        printf("\n");
    }
}

void tampilkanPenyakitFisik() {
    int ditemukan = 0;
    printf("=== Daftar Penyakit Fisik ===\n");
    for (int i = 0; i < jumlahPenyakit; i++) {
        if (strcasecmp(daftarPenyakit[i].jenis, "fisik") == 0) {
            printf("%d. %s\n", i+1, daftarPenyakit[i].nama);
            printf("   Obat   : %s\n\n", daftarPenyakit[i].obat);
            ditemukan = 1;
        }
    }
    if (!ditemukan) {
        printf("Tidak ada data penyakit fisik.\n\n");
    }
}

void tampilkanPenyakitMental() {
    int ditemukan = 0;
    printf("=== Daftar Penyakit Mental ===\n");
    for (int i = 0; i < jumlahPenyakit; i++) {
        if (strcasecmp(daftarPenyakit[i].jenis, "mental") == 0) {
            printf("%d. %s\n", i+1, daftarPenyakit[i].nama);
            printf("   Obat   : %s\n", daftarPenyakit[i].obat);
            printf("   Terapi : %s\n\n", daftarPenyakit[i].terapi);
            ditemukan = 1;
        }
    }
    if (!ditemukan) {
        printf("Tidak ada data penyakit mental.\n\n");
    }
}

void cariPenyakit() {
    if (jumlahPenyakit == 0) {
        printf("Belum ada data penyakit.\n\n");
        return;
    }
    printf("=== Cari Penyakit ===\n");
    char namaCari[50];
    printf("Masukkan nama penyakit yang ingin dicari: ");
    fgets(namaCari, sizeof(namaCari), stdin);
    namaCari[strcspn(namaCari, "\n")] = '\0';

    pencarianDilakukan++; // Update statistik

    int ditemukan = 0;
    for (int i = 0; i < jumlahPenyakit; i++) {
        if (strcasecmp(daftarPenyakit[i].nama, namaCari) == 0) {
            // Tampilkan detail penyakit
            printf("\nPenyakit  : %s\n", daftarPenyakit[i].nama);
            printf("Jenis     : %s\n", daftarPenyakit[i].jenis);
            printf("Obat      : %s\n", daftarPenyakit[i].obat);
            if (strcasecmp(daftarPenyakit[i].jenis, "mental") == 0) {
                printf("Terapi    : %s\n", daftarPenyakit[i].terapi);
            }
            printf("\n");
            ditemukan = 1;
            pencarianBerhasil++; // Update statistik
            break;
        }
    }
    if (!ditemukan) {
        printf("Penyakit '%s' belum terdaftar.\n", namaCari);
        printf("Apakah anda ingin menyarankan penyakit?\n(Jika ya, ketik 1.\nJika tidak ketik tombol lain.)\n");
        int saran;
        printf("Pilihan: ");
        scanf(" %d", &saran);
        bersihkanBuffer();
        if (saran == 1) {
        printf("Terima kasih atas sarannya!\n\n");
        } else {
        printf("Terimakasih!\n\n");
        }
        pencarianGagal++; // Update statistik
    }
}

void tampilkanStatistik() {
    printf("=== Statistik Aplikasi ===\n");
    printf("1. Total Akses Aplikasi          : %d kali\n", totalAkses);
    printf("2. Jumlah Penyakit Ditambahkan    : %d\n", penyakitDitambahkan);
    printf("3. Jumlah Penyakit Dihapus        : %d\n", penyakitDihapus);
    printf("4. Jumlah Pencarian Dilakukan     : %d kali\n", pencarianDilakukan);
    printf("5. Jumlah Pencarian Berhasil      : %d kali\n", pencarianBerhasil);
    printf("6. Jumlah Pencarian Gagal         : %d kali\n", pencarianGagal);
    printf("7. Jumlah Login Admin             : %d kali\n", jumlahLoginAdmin);
    printf("8. Jumlah Akses sebagai User       : %d kali\n\n", jumlahAksesUser);
}

void menuAdmin() {
    int pilihan;
    do {
        printf("=== MENU ADMIN ===\n");
        printf("1. Tambah Penyakit\n");
        printf("2. Hapus Penyakit\n");
        printf("3. Edit Penyakit\n");
        printf("4. Lihat Semua Penyakit\n");
        printf("5. Lihat Statistik Aplikasi\n");
        printf("6. Keluar (Logout)\n");
        printf("Pilihan: ");
        scanf("%d", &pilihan);
        bersihkanBuffer(); // Bersihkan buffer setelah scanf

        switch (pilihan) {
            case 1:
                tambahPenyakit();
                break;
            case 2:
                hapusPenyakit();
                break;
            case 3:
                editPenyakit();
                break;
            case 4:
                tampilkanSemuaPenyakit();
                break;
            case 5:
                tampilkanStatistik();
                break;
            case 6:
                printf("Logout Admin...\n\n");
                break;
            default:
                printf("Pilihan tidak valid.\n\n");
        }
    } while (pilihan != 6);
}

void menuUser() {
    int pilihan;
    do {
        printf("=== MENU USER ===\n");
        printf("1. Lihat Penyakit Fisik\n");
        printf("2. Lihat Penyakit Mental\n");
        printf("3. Cari Penyakit (dapatkan rekomendasi obat/terapi)\n");
        printf("4. Keluar\n");
        printf("Pilihan: ");
        scanf("%d", &pilihan);
        bersihkanBuffer(); // Bersihkan buffer setelah scanf

        switch (pilihan) {
            case 1:
                tampilkanPenyakitFisik();
                break;
            case 2:
                tampilkanPenyakitMental();
                break;
            case 3:
                cariPenyakit();
                break;
            case 4:
                printf("Terima kasih telah menggunakan aplikasi Biocare.\n\n");
                break;
            default:
                printf("Pilihan tidak valid.\n\n");
        }
    } while (pilihan != 4);
}

// --------------------- FUNGSI UTAMA ---------------------
int main() {
    int pilihanLogin;

    // Memuat data dari file saat program dimulai
    muatData();

    while (1) {
        // Setiap kali while(1) berulang, artinya aplikasi diakses lagi
        totalAkses++;

        printf("=================================\n");
        printf("       APLIKASI BIOCARE (C)      \n");
        printf("=================================\n");
        printf("1. Login sebagai Admin\n");
        printf("2. Akses sebagai User\n");
        printf("3. Keluar\n");
        printf("Pilih opsi: ");
        scanf("%d", &pilihanLogin);
        bersihkanBuffer();

        if (pilihanLogin == 1) {
            // Login admin
            char usernameInput[20], passwordInput[20];
            printf("Masukkan username Admin: ");
            fgets(usernameInput, sizeof(usernameInput), stdin);
            usernameInput[strcspn(usernameInput, "\n")] = '\0';
            printf("Masukkan password: ");
            fgets(passwordInput, sizeof(passwordInput), stdin);
            passwordInput[strcspn(passwordInput, "\n")] = '\0';

            if (strcmp(usernameInput, usernameAdmin) == 0 && 
                strcmp(passwordInput, passwordAdmin) == 0) {
                printf("\nLogin Admin berhasil!\n\n");
                jumlahLoginAdmin++; // Update statistik
                menuAdmin();
            } else {
                printf("\nUsername atau Password Admin salah.\n\n");
            }
        }
        else if (pilihanLogin == 2) {
            // Akses sebagai user tanpa login
            printf("\nAnda sekarang masuk sebagai User.\n\n");
            jumlahAksesUser++; // Update statistik
            menuUser();
        }
        else if (pilihanLogin == 3) {
            printf("Terima kasih telah menggunakan aplikasi Biocare.\n");
            break;
        }
        else {
            printf("Pilihan tidak valid.\n\n");
        }
    }

    return 0;
}
