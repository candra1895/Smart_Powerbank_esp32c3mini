# Smart Powerbank ESP32-C3 Mini

Smart powerbank DIY berbasis **ESP32-C3 Mini** dengan OLED I2C, charger baterai terpisah, dan board powerbank bekas sebagai bagian output/boost.

## Status proyek
**Tahap 1 selesai — OLED berhasil menyala dan berjalan.**

**Tahap 2 — charger terpisah:** modul yang difoto cocok dengan board **CSM4056T / TP4056-style Type-C 1S dengan proteksi**, bukan sekadar charger tanpa proteksi. Nama awal project ditulis CSM40561, tetapi dokumentasi sekarang mencatat bentuk board dan fungsi terminal yang sudah teridentifikasi.

Tanggal progress: 26 September 2026.

## Hardware yang dipakai
- ESP32-C3 Mini / SuperMini
- OLED I2C 0.9/0.91 inch
- Driver OLED: SSD1306
- Resolusi test: 128x32
- Alamat I2C: 0x3C
- Modul charger Type-C **CSM4056T / TP4056-style dengan protection**
- Board powerbank bekas `229-V8.2S 2021-3-30 3511` untuk boost/output 5V
- 2 x baterai Li-ion 4600 mAh
- Konfigurasi baterai: 1S2P / paralel
- Kapasitas nominal pack: sekitar 9200 mAh @ 3.7 V
- Tegangan pack terukur: sekitar 3.9 V

## Arsitektur daya
```text
USB-C 5V
   |
   v
+-----------------------------+
| CSM4056T / TP4056-style     |
| charger + battery protection|
+-----------------------------+
   | B+ / B-
   v
BATTERY PACK 1S2P
2 x 4600 mAh
   |
   | protected path melalui OUT+ / OUT-
   v
BOARD POWERBANK / BOOST ---> USB 5V OUTPUT
   |
   +---> supply ESP32-C3 + OLED
```

### Terminal charger
Dengan **USB-C berada di bawah** dan sisi komponen menghadap ke atas, empat pad di sisi atas dibaca dari kiri ke kanan:

```text
OUT+   B+   B-   OUT-
```

- `B+` / `B-`: hanya untuk battery pack 1S2P.
- `OUT+` / `OUT-`: menuju beban/board powerbank agar proteksi over-discharge, over-current, dan short-circuit tetap berada di jalur.
- USB-C: input charger 5V.
- Pad `IN+` / `IN-` di dekat USB-C adalah alternatif input 5V dan tidak perlu dipakai jika menggunakan USB-C.

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

## Progress
- [x] ESP32-C3 board package terpasang
- [x] ESP32-C3 dapat di-upload
- [x] OLED I2C tersambung
- [x] OLED berhasil menampilkan teks
- [x] Charger dipisahkan dari boost/output
- [x] Board charger Type-C dengan B+/B-/OUT+/OUT- teridentifikasi
- [ ] Uji charger dengan multimeter
- [ ] Pembacaan tegangan baterai
- [ ] Persentase baterai
- [ ] Status charging
- [ ] Tampilan UI smart powerbank
- [ ] Optimasi konsumsi daya
- [ ] Final assembly

## Safety
- Pack tetap **1S2P**. Jangan dibuat seri.
- Jangan sambungkan baterai langsung ke ADC ESP32-C3; gunakan resistor divider.
- Jangan mengambil beban dari `B+` / `B-`; gunakan `OUT+` / `OUT-` supaya proteksi charger tetap efektif.
- Modul ini bukan power-path/load-sharing controller. Untuk pengujian awal, hindari charging sambil menarik beban besar dari output.
