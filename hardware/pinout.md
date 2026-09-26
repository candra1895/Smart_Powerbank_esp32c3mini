# Pinout Smart Powerbank

Pin yang sudah dialokasikan:

| Fungsi | Pin ESP32-C3 Mini | Status |
|---|---:|---|
| OLED SDA | GPIO 5 | Tested |
| OLED SCL | GPIO 6 | Tested |
| OLED VCC | 3V3 | Tested |
| OLED GND | GND | Tested |
| Battery ADC | GPIO 4 / ADC1_CH4 | Tahap 3 |
| Charge status | Belum ditentukan | Planned |
| Button input | Belum ditentukan | Planned |

## Battery ADC

GPIO4 dipakai untuk membaca tegangan battery pack melalui resistor divider.

```text
OUT+ -- 10k --+-- GPIO4
              |
             10k
              |
OUT- ---------+-- GND ESP32
```

Jangan menghubungkan battery pack langsung ke GPIO4.

GPIO2, GPIO8, dan GPIO9 dibiarkan tidak dipakai untuk fungsi yang dapat mengganggu proses boot karena merupakan strapping pins ESP32-C3.
