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
