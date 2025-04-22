# Aplikasi Biocare (C)

## Deskripsi
Aplikasi konsol sederhana untuk manajemen data penyakit beserta rekomendasi obat dan terapi. Dibangun menggunakan bahasa C, aplikasi ini mendukung akses oleh Admin (untuk CRUD data penyakit dan pemantauan statistik) serta akses sebagai User (untuk melihat dan mencari penyakit).

## Fitur
- **Login Admin**: Autentikasi dengan username `admin` dan password `admin123`.
- **Manajemen Penyakit**: Tambah, hapus, edit, dan tampilkan daftar penyakit.
- **Statistik Aplikasi**: Pantau jumlah total akses, penyakit ditambahkan, dihapus, pencarian (berhasil/gagal), login Admin, dan akses User.
- **Penyimpanan Persisten**: Data penyakit disimpan di file `penyakit.txt`.
- **Data Bawaan**: Terdapat 17 penyakit bawaan (fisik dan mental) yang dimuat otomatis saat pertama kali dijalankan.
- **Mode User**: Lihat penyakit fisik, lihat penyakit mental, cari penyakit untuk mendapatkan rekomendasi obat/terapi.

## Prasyarat
- Compiler bahasa C (misal: GCC atau Clang).
- Sistem operasi dengan dukungan I/O file (Windows, Linux, macOS).

## Instalasi
1. Clone repository ini:
   ```bash
   git clone https://github.com/fapzarz/biocare.git
   cd biocare
   ```
2. Kompilasi kode sumber:
   ```bash
   gcc -o biocare biocaresrc.c
   ```
3. Jalankan aplikasi:
   ```bash
   ./biocare
   ```

## Cara Menggunakan
1. Setelah dijalankan, pilih opsi:
   - `1` untuk login sebagai Admin
   - `2` untuk akses sebagai User
   - `3` untuk keluar

### Login Admin
- Masukkan username: `admin`
- Masukkan password: `admin123`
- Menu Admin akan muncul dengan opsi:
  1. Tambah Penyakit
  2. Hapus Penyakit
  3. Edit Penyakit
  4. Lihat Semua Penyakit
  5. Lihat Statistik Aplikasi
  6. Keluar (Logout)

### Akses User
- Pilih:
  1. Lihat Penyakit Fisik
  2. Lihat Penyakit Mental
  3. Cari Penyakit (dapatkan rekomendasi obat/terapi)
  4. Keluar

## Struktur File
- `biocaresrc.c`: Kode sumber C aplikasi Biocare.
- `penyakit.txt`: File data penyimpanan penyakit (dibuat otomatis jika belum ada).

## Lisensi
Proyek ini dilisensikan di bawah MIT License. Lihat file `LICENSE` untuk detail.

## Kontribusi
Pull request dan laporan bug dipersilakan melalui halaman Issues di repository ini.

