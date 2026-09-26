# Troubleshooting

## 1. Unknown FQBN: platform esp32:esp32 is not installed

Penyebab:
ESP32 board package belum terpasang di Arduino IDE.

Solusi:
1. Buka Boards Manager.
2. Cari **esp32**.
3. Install **esp32 by Espressif Systems**.
4. Pilih board **ESP32C3 Dev Module**.

## 2. Could not connect to COM serial port

Periksa:
- kabel USB harus mendukung data,
- pilih COM yang muncul ketika ESP32-C3 dicolok,
- coba port USB lain,
- gunakan tombol BOOT bila upload gagal saat Connecting.

## 3. OLED blank

Setup yang sudah terbukti bekerja di project ini:
- SDA = GPIO 5
- SCL = GPIO 6
- VCC = 3V3
- address = 0x3C
- SSD1306 128x32

Jika masih blank:
- cek sambungan,
- restart ESP32,
- jalankan I2C scanner,
- coba address 0x3D jika modul berbeda.

## 4. Battery terbaca 6.027V / 100% padahal pack 1S sekitar 3.9V

Gejala yang pernah terjadi pada project:

```text
Battery: 6.027 V | 100%
```

Untuk pack Li-ion 1S, angka 6V tidak mungkin berasal dari baterai normal. Firmware mengalikan tegangan ADC dengan rasio divider 2x. Nilai 6.027V berarti ADC membaca sekitar 3.013V sebelum dikali dua, yang menunjukkan input ADC terlalu tinggi / saturasi.

Penyebab paling mungkin:
- hanya memakai **satu resistor 10k** sebagai resistor seri; ini BUKAN voltage divider,
- resistor bawah 10k belum tersambung ke GND,
- titik GPIO4 diambil dari sisi yang salah,
- ground battery/protection board belum common dengan GND ESP32,
- GPIO4 tidak terhubung ke titik tengah divider,
- ADC tersambung ke jalur 5V/USB output secara tidak sengaja.

### Wiring yang benar: WAJIB dua resistor 10k

```text
Battery/OUT+ ---- 10k ----+---- GPIO4
                          |
                         10k
                          |
Battery/OUT- -------------+---- GND ESP32
```

Titik GPIO4 adalah PERSIMPANGAN dua resistor.

Dengan battery 3.9V, ukur memakai multimeter:
- OUT+ ke OUT-: sekitar 3.9V
- GPIO4 ke GND: sekitar 1.95V

Dengan battery full 4.2V:
- GPIO4 ke GND: sekitar 2.10V

### Tindakan keselamatan

Jika GPIO4 terbaca lebih dari kira-kira 2.3V pada rangkaian divider 10k/10k, cabut sambungan GPIO4 dan cek wiring sebelum melanjutkan.

Jangan menghubungkan battery 3.7-4.2V langsung ke GPIO ESP32-C3. ESP32-C3 bekerja di domain 3.3V dan input di atas spesifikasi dapat merusak chip.

Gunakan sketch:
`firmware/02_battery_voltage/02b_adc_diagnostic.ino`

untuk melihat RAW ADC dan millivolt sebelum memakai UI battery percentage.

## 5. Baterai belum dipasang tetapi terbaca sekitar 4.1V / 87-90%

Gejala:

```text
Battery: 4.091 V | 87%
Battery: 4.108 V | 90%
```

Jika battery pack benar-benar belum terpasang, pembacaan ini biasanya berasal dari salah satu dari dua kondisi:

1. **USB-C charger CSM4056T sedang diberi 5V.** Charger dapat menaikkan node BAT/OUT mendekati tegangan terminasi sekitar 4.2V walaupun tidak ada sel. ADC lalu mengira node tersebut adalah baterai penuh.
2. **Node ADC/OUT mendapat tegangan dari jalur lain atau floating/coupling.** Ini harus dicek bila charger USB-C juga tidak terhubung.

### Tes pembeda

Lepas battery pack.

A. Lepas juga input USB-C charger:
- ukur OUT+ ke OUT-,
- ukur titik tengah divider ke GND.

Harapan:
- OUT+ ke OUT- mendekati 0V,
- titik tengah divider mendekati 0V.

B. Colok USB-C 5V ke charger tanpa battery:
- OUT/BAT dapat naik sekitar 4.0-4.2V pada charger linear jenis ini,
- karena itu voltage-only detection tidak bisa membedakan "battery full" dan "battery absent while charger powered".

### Implikasi firmware

Threshold sederhana seperti "di bawah 2.8V = NO BATTERY" hanya bekerja ketika charger input tidak sedang memberi node BAT/OUT tegangan.

Untuk deteksi battery-present yang benar saat charger juga terhubung, diperlukan sinyal tambahan, misalnya:
- membaca status charger dan input charger,
- atau rangkaian battery-presence/load-test khusus.

Untuk tahap saat ini, jangan ubah kurva persentase. Verifikasi dulu apakah pembacaan 4.1V hanya muncul saat USB-C charger terhubung.


## 6. Battery terbaca sekitar 5.24V setelah baterai dipasang

Contoh gejala:

```text
Battery: 5.244 V | 100% | IDLE
```

Pack project adalah Li-ion 1S, jadi battery asli tidak boleh berada di 5.24V. Nilai ini sangat mirip dengan **output boost 5V board powerbank**, terutama karena boost tanpa beban dapat berada sedikit di atas 5V.

Kemungkinan utama: resistor divider/ADC dipasang ke output USB 5V setelah boost, bukan ke tegangan battery sebelum boost.

### Titik sense yang benar

```text
Battery/CSM4056T OUT+ (sebelum boost)
          |
         10k
          |
          +------ GPIO4
          |
         10k
          |
CSM4056T OUT- ---- GND ESP32
```

ESP32 boleh tetap diberi daya dari output boost 5V, tetapi **GPIO4 harus mengukur jalur battery sebelum boost**.

### Tes multimeter

Ukur tiga titik:

1. Battery B+ ke B-: sekitar 3.0-4.2V.
2. CSM4056T OUT+ ke OUT-: normalnya hampir sama dengan tegangan battery.
3. USB 5V board powerbank: sekitar 5V dan bisa sedikit lebih tinggi tanpa beban.

Jika titik yang masuk ke R1 10k terbaca sekitar 5.2V, kabel sense berada di sisi yang salah.

Firmware charging indicator sekarang juga menampilkan `CHECK BAT SENSE` jika pembacaan melebihi 4.35V agar nilai 5V tidak lagi ditampilkan sebagai 100%.
