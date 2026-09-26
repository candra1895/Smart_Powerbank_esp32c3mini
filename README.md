# Smart Powerbank ESP32-C3 Mini

Smart powerbank DIY berbasis **ESP32-C3 Mini** dengan OLED I2C dan board powerbank bekas.

## Status proyek
**Tahap 1 selesai — OLED berhasil menyala dan berjalan.**

Tanggal progress: 26 September 2026.

## Hardware yang dipakai
- ESP32-C3 Mini / SuperMini
- OLED I2C 0.9/0.91 inch
- Driver OLED: SSD1306
- Resolusi yang dipakai saat test: 128x32
- Alamat I2C: 0x3C
- Board powerbank bekas bertuliskan `229-V8.2S 2021-3-30 3511`
- 2 x baterai Li-ion 4600 mAh
- Konfigurasi baterai: 1S2P / paralel
- Kapasitas nominal pack: sekitar 9200 mAh @ 3.7 V
- Tegangan pack terukur: sekitar 3.9 V

## Wiring OLED yang sudah dites
| OLED | ESP32-C3 Mini |
|---|---|
| GND | GND |
| VCC | 3V3 |
| SDA | GPIO 5 |
| SCL | GPIO 6 |

## Software
Arduino IDE:
- Board: **ESP32C3 Dev Module**
- Serial Monitor: **115200 baud**

Library:
- Adafruit GFX Library
- Adafruit SSD1306

## Target fitur
- Persentase baterai
- Tegangan baterai real-time
- Status charging / discharging
- Animasi charging
- Low battery warning
- OLED auto sleep
- ESP32 low-power mode
- Fitur tambahan akan ditambahkan bertahap

## Progress
- [x] ESP32-C3 board package terpasang
- [x] ESP32-C3 dapat di-upload
- [x] OLED I2C tersambung
- [x] OLED berhasil menampilkan teks
- [ ] Pembacaan tegangan baterai
- [ ] Persentase baterai
- [ ] Status charging
- [ ] Tampilan UI smart powerbank
- [ ] Optimasi konsumsi daya
- [ ] Final assembly

## Struktur repository
- `firmware/` — kode Arduino per tahap
- `docs/` — wiring, tutorial, dan troubleshooting
- `hardware/` — catatan pinout dan hardware

## Safety
Jangan sambungkan baterai Li-ion langsung ke pin ADC ESP32-C3. Pembacaan tegangan baterai nanti menggunakan resistor divider.

Dua sel yang dipasang paralel harus memiliki tegangan yang sangat dekat sebelum disatukan.
