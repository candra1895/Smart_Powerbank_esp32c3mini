# Tahap 2 - Charger Terpisah CSM4056T / TP4056-style

Tanggal update: 26 September 2026.

## Identifikasi dari foto

Modul yang digunakan cocok dengan board charger USB Type-C 1S berbasis **CSM4056T / keluarga TP4056-style** yang memiliki rangkaian proteksi baterai.

Ciri yang terlihat:
- USB Type-C.
- Dua LED status.
- Empat terminal baterai/output di sisi berlawanan USB.
- Pad input alternatif `IN+` dan `IN-`.
- Terminal `B+`, `B-`, `OUT+`, dan `OUT-`.

## Pinout fisik

Posisikan board:
- USB-C di bawah.
- Sisi komponen menghadap ke atas.

Empat pad bagian atas dari kiri ke kanan:

```text
+---------+---------+---------+---------+
|  OUT+   |   B+    |   B-    |  OUT-   |
+---------+---------+---------+---------+
```

Di dekat USB-C:
- USB-C = input 5V.
- `IN+` / `IN-` = alternatif input 5V bila tidak memakai konektor USB-C.

## Wiring ke baterai 1S2P

```text
Battery #1 (+) ---+
                 +---- B+
Battery #2 (+) ---+

Battery #1 (-) ---+
                 +---- B-
Battery #2 (-) ---+
```

Baterai project:
- 2 x Li-ion 4600 mAh.
- Paralel / 1S2P.
- Total nominal sekitar 9200 mAh @ 3.7 V.
- Tegangan pack terakhir sekitar 3.9 V.

## Wiring ke board boost/powerbank lama

```text
CSM4056T OUT+ ----> B+ / input positif board powerbank lama
CSM4056T OUT- ----> B- / input negatif board powerbank lama
```

Gunakan `OUT+` dan `OUT-`, bukan langsung `B+` dan `B-`, supaya jalur beban melewati proteksi baterai.

## Input charging

Pakai charger USB 5V ke port USB-C.

Jangan memberi input 9V/12V USB PD secara paksa ke pad input. Modul charger linear ini ditujukan untuk input sekitar 5V.

## Pengujian sebelum assembly

1. Pastikan polaritas baterai benar.
2. Sambungkan pack ke B+ / B-.
3. Ukur tegangan OUT+ terhadap OUT-.
4. Masukkan 5V melalui USB-C.
5. Pastikan LED charge menyala.
6. Pantau suhu modul selama beberapa menit.
7. Setelah mendekati penuh, tegangan baterai tidak boleh terus naik melewati kisaran 4.2V.
8. Baru sambungkan board powerbank lama ke OUT+ / OUT-.

## Catatan charge + load

CSM4056T/TP4056 adalah linear charger dan bukan IC power-path/load-sharing. Beban besar yang tetap aktif saat charging dapat mengganggu terminasi charge dan membuat modul lebih panas.

Untuk tahap awal project:
- charge baterai dengan beban powerbank/ESP32 dimatikan atau minimal,
- setelah sistem stabil baru kita evaluasi apakah perlu power-path terpisah.

## Tahap selanjutnya

Setelah charger lolos uji multimeter, lanjut:
1. resistor divider untuk ADC,
2. baca tegangan baterai,
3. tampilkan voltage + percentage di OLED,
4. deteksi status charging dari LED/status pin bila memungkinkan.
