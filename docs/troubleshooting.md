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
