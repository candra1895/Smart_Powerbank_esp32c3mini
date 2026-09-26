# Tahap 3 - Membaca Tegangan Baterai + Persentase OLED

## Pin yang dipakai

- OLED SDA: GPIO 5
- OLED SCL: GPIO 6
- Battery ADC: **GPIO 4 / ADC1_CH4**

## WAJIB: dua resistor untuk voltage divider

Firmware menggunakan divider:
- R1 = 10k ohm
- R2 = 10k ohm

**Satu resistor 10k saja tidak menurunkan tegangan ADC.** Resistor tunggal hanya menjadi resistor seri karena input ADC sangat tinggi impedansinya.

Wiring yang benar:

```text
CSM4056T OUT+ / Battery+
          |
         10k  R1
          |
          +----------> GPIO4 ESP32-C3
          |
         10k  R2
          |
CSM4056T OUT- --------> GND ESP32-C3
```

Dengan divider 1:1:
- 4.20V -> sekitar 2.10V pada GPIO4
- 3.90V -> sekitar 1.95V pada GPIO4

### Sebelum GPIO4 disambungkan

Ukur dengan multimeter terlebih dahulu:
1. OUT+ terhadap OUT- harus sesuai tegangan pack, misalnya ~3.9V.
2. Titik tengah dua resistor terhadap OUT-/GND harus ~setengahnya, misalnya ~1.95V.
3. Baru hubungkan titik tengah ke GPIO4.

## Power saat pengujian

1. ESP32-C3 boleh tetap diberi daya dari USB PC.
2. Baterai tersambung ke charger/protection board.
3. OUT- charger harus common dengan GND ESP32.
4. Titik tengah 10k/10k masuk GPIO4.

## ADC

Firmware menggunakan:
- resolusi 12-bit
- attenuasi ADC_11db
- analogReadMilliVolts()
- averaging 32 sampel

ESP32-C3 ADC pada attenuasi tinggi tetap mempunyai batas rentang ukur. Jika input terlalu tinggi, pembacaan dapat saturasi dan angka hasil perkalian divider menjadi tidak masuk akal.

## Kasus nyata project: 6.027V

Saat serial menampilkan:

```text
Battery: 6.027 V | 100%
```

angka tersebut bukan tegangan baterai sebenarnya. Nilai itu berarti ADC sekitar 3.013V kemudian dikalikan rasio 2.0. Ini indikator kuat bahwa divider salah / tidak lengkap / input ADC over-range.

Gunakan `02b_adc_diagnostic.ino` sebelum melanjutkan.

## Kalibrasi

Setelah wiring benar dan hasil berada sekitar 3.3-4.2V, bandingkan dengan multimeter.

Contoh:
- multimeter = 3.90V
- OLED = 3.84V

```text
CALIBRATION = 3.90 / 3.84 = 1.0156
```

Lalu ubah:

```cpp
const float CALIBRATION = 1.0156f;
```

## Persentase

Persentase adalah estimasi berdasarkan kurva tegangan Li-ion 1S. Saat charging atau beban besar, angka dapat berubah sementara.

## Tahap selanjutnya

Setelah tegangan stabil:
1. charging detection,
2. charging animation,
3. low-battery warning,
4. OLED auto sleep,
5. power optimization.
