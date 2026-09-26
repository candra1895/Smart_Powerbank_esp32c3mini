# Tahap 2 - Arsitektur Charger Terpisah

Tanggal update: 26 September 2026.

## Keputusan desain

Charging baterai dipisahkan dari board powerbank bekas.

Blok sistem:

```text
5V INPUT
   |
   v
CSM40561 charger module
   |
   v
Battery pack 1S2P
2 x 4600 mAh
   |
   +--> ESP32-C3 + OLED
   |
   +--> Board powerbank bekas / boost --> USB 5V output
```

## Kenapa dipisah?

- Charging dan boost/output lebih mudah diuji terpisah.
- Troubleshooting menjadi lebih sederhana.
- ESP32 bisa membaca status sistem tanpa harus bergantung penuh pada board powerbank lama.
- Board powerbank bekas dapat difokuskan sebagai bagian output/boost bila cocok.

## Catatan penting CSM40561

Nama **CSM40561** dicatat berdasarkan modul yang tersedia pada project. Keluarga IC bernomor 40561 tersedia dari beberapa produsen dan spesifikasinya tidak selalu identik.

Sebelum penyambungan permanen, wajib verifikasi:
1. Pin input + dan -.
2. Pin battery + dan -.
3. Tegangan input modul.
4. Tegangan terminasi baterai (target pack Li-ion 1S umumnya 4.2 V).
5. Arus charge modul.
6. Apakah modul memiliki proteksi over-discharge/over-current atau hanya fungsi charger.

## Kondisi baterai project

- 2 x Li-ion 4600 mAh
- Konfigurasi: paralel / 1S2P
- Kapasitas nominal total: sekitar 9200 mAh
- Tegangan pack yang sudah diukur: sekitar 3.9 V

## Belum dilakukan

Jangan sambungkan modul ke baterai hanya berdasarkan nama IC. Pinout PCB modul harus dilihat terlebih dahulu.

Tahap berikutnya adalah dokumentasi foto/pin modul CSM40561 dan pengukuran input/output dengan multimeter sebelum digabung ke sistem.
