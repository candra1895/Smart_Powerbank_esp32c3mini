# Tahap 3 - Membaca Tegangan Baterai + Persentase OLED

## Pin yang dipakai

- OLED SDA: GPIO 5
- OLED SCL: GPIO 6
- Battery ADC: **GPIO 4 / ADC1_CH4**

GPIO4 dipilih karena merupakan ADC1_CH4 pada ESP32-C3 dan tidak termasuk strapping pin GPIO2/GPIO8/GPIO9.

## Penting: resistor divider

Firmware tahap ini diasumsikan memakai:

- R1 (atas): 10k ohm
- R2 (bawah): 10k ohm

Wiring:

```text
CSM4056T OUT+
      |
     10k   <- R1
      |
      +----------> GPIO4 ESP32-C3
      |
     10k   <- R2
      |
CSM4056T OUT- ----> GND ESP32-C3
```

Dengan divider 1:1:
- baterai 4.20 V menjadi sekitar 2.10 V di GPIO4
- baterai 3.90 V menjadi sekitar 1.95 V di GPIO4

**Jangan sambungkan 4.2 V baterai langsung ke GPIO ESP32-C3.**

Jika saat ini hanya tersedia satu resistor 10k dan satu 1M, jangan gunakan kombinasi itu untuk versi ini. Tambahkan satu resistor 10k lagi atau pasangan resistor bernilai sama.

## Power saat pengujian awal

Cara paling sederhana:
1. ESP32-C3 tetap diberi daya dari USB PC.
2. Baterai tetap terhubung ke charger/protection board.
3. Sambungkan OUT- charger ke GND ESP32.
4. Sambungkan OUT+ melalui divider 10k/10k ke GPIO4.

Ground harus common agar pembacaan ADC punya referensi yang sama.

## ADC

Firmware menggunakan:
- resolusi 12-bit
- attenuasi ADC_11db
- analogReadMilliVolts()
- averaging 32 sampel

## Kalibrasi

Setelah sketch berjalan, bandingkan tegangan di OLED dengan multimeter.

Contoh:
- multimeter = 3.90 V
- OLED = 3.84 V

Hitung:

```text
CALIBRATION = 3.90 / 3.84
            = 1.0156
```

Lalu ubah di firmware:

```cpp
const float CALIBRATION = 1.0156f;
```

## Persentase baterai

Persentase dibuat dari tabel kurva tegangan Li-ion 1S, bukan rumus linear sederhana.

Catatan:
- saat charging, tegangan terlihat lebih tinggi
- saat beban besar, tegangan dapat turun sementara
- karena itu persen masih merupakan estimasi

Untuk akurasi tingkat lanjut nanti bisa ditambah fuel gauge seperti MAX17048.

## Target tampilan

```text
BATTERY        65%
[████████----] 3.91V
```

## Tahap selanjutnya

Setelah voltage + percentage stabil:
1. deteksi status charging,
2. animasi charge,
3. low battery warning,
4. auto sleep OLED,
5. power optimization.
