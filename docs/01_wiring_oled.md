# Tahap 1 - Wiring OLED ke ESP32-C3 Mini

Status: **SUDAH DITEST DAN BERHASIL**

## Sambungan

| OLED I2C | ESP32-C3 Mini |
|---|---|
| GND | GND |
| VCC | 3V3 |
| SDA | GPIO 5 |
| SCL | GPIO 6 |

Diagram sederhana:

```text
OLED 0.9/0.91"
+---------+
| GND  -------- GND
| VCC  -------- 3V3
| SDA  -------- GPIO 5
| SCL  -------- GPIO 6
+---------+
             ESP32-C3 Mini
```

## Parameter OLED yang berhasil dipakai

- Controller: SSD1306
- Resolusi: 128 x 32
- Interface: I2C
- Address: 0x3C
- Tegangan VCC saat test: 3.3 V

## Library Arduino

Install melalui Library Manager:

1. Adafruit GFX Library
2. Adafruit SSD1306

## Catatan

Jika nanti menggunakan OLED lain dan layar tidak tampil:
- cek kembali VCC dan GND,
- cek SDA/SCL,
- jalankan I2C scanner,
- cek kemungkinan address 0x3D,
- cek apakah panel menggunakan resolusi 128x64.
