# Tahap 4 - Charging Indicator OLED

## Tujuan

Menambahkan indikator dan animasi charging pada OLED tanpa menambah hardware charge-detect terlebih dahulu.

## Cara deteksi pada versi ini

ESP32 hanya membaca tegangan battery pack di GPIO4. Karena belum ada kabel khusus dari charger ke ESP32, firmware **belum dapat mengetahui keberadaan USB charger secara langsung**.

Untuk pengujian awal, status charging dideteksi dari tren tegangan:
- tegangan difilter dengan EMA,
- setiap 10 detik dibandingkan dengan nilai sebelumnya,
- bila naik >= 10 mV, status CHARGING aktif,
- status ditahan 90 detik agar animasi tidak berkedip,
- penurunan cukup besar membatalkan status charging.

Ini hanya metode sementara untuk test OLED.

## Tampilan

Saat idle:

```text
BATTERY        87%
[██████████--] 4.09V
```

Saat charging terdeteksi:

```text
CHARGING       87%
[animasi isi ] >>4.09V
```

Battery bar akan bergerak dari kosong menuju penuh berulang-ulang.

## Mode test paksa

Di firmware:

```cpp
const bool FORCE_CHARGING_ANIMATION = false;
```

Ubah menjadi:

```cpp
const bool FORCE_CHARGING_ANIMATION = true;
```

untuk memastikan animasi OLED bekerja tanpa perlu charger.

Setelah test, kembalikan ke `false`.

## Cara test charging sebenarnya

1. Pastikan battery divider GPIO4 sudah benar.
2. Pasang battery pack.
3. Nyalakan ESP32.
4. Catat voltage awal.
5. Colok 5V ke USB-C CSM4056T.
6. Tunggu sekitar 10-30 detik.
7. Jika voltage pack naik secara stabil, OLED akan masuk mode CHARGING.

## Keterbatasan

Metode tren tegangan bisa:
- terlambat mendeteksi,
- salah trigger akibat perubahan beban,
- gagal ketika baterai sudah hampir penuh dan voltage tidak banyak berubah.

Versi final sebaiknya memakai **hardware charger-present/status detection** ke GPIO terpisah agar status charging langsung dan akurat.
