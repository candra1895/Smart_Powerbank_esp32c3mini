#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SDA_PIN 5
#define SCL_PIN 6
#define BATTERY_ADC_PIN 4

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

// Divider 10k (atas) + 10k (bawah) = rasio 2.0
const float DIVIDER_RATIO = 2.0f;

// Ubah setelah dibandingkan dengan multimeter.
// Contoh: multimeter 3.90V, OLED 3.84V
// CALIBRATION = 3.90 / 3.84 = 1.0156
const float CALIBRATION = 1.0000f;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float readBatteryVoltage() {
  const int samples = 32;
  uint32_t totalMv = 0;

  for (int i = 0; i < samples; i++) {
    totalMv += analogReadMilliVolts(BATTERY_ADC_PIN);
    delay(2);
  }

  float adcVoltage = (totalMv / (float)samples) / 1000.0f;
  return adcVoltage * DIVIDER_RATIO * CALIBRATION;
}

int batteryPercent(float voltage) {
  // Estimasi kurva Li-ion 1S berdasarkan tegangan.
  // Nilai persen dapat bergeser saat charging atau saat ada beban besar.
  const float volts[] = {
    3.30, 3.45, 3.55, 3.61, 3.65,
    3.68, 3.71, 3.74, 3.77, 3.79,
    3.82, 3.85, 3.87, 3.91, 3.95,
    3.98, 4.02, 4.08, 4.11, 4.15, 4.20
  };

  const int percents[] = {
    0, 5, 10, 15, 20,
    25, 30, 35, 40, 45,
    50, 55, 60, 65, 70,
    75, 80, 85, 90, 95, 100
  };

  const int count = sizeof(volts) / sizeof(volts[0]);

  if (voltage <= volts[0]) return 0;
  if (voltage >= volts[count - 1]) return 100;

  for (int i = 0; i < count - 1; i++) {
    if (voltage >= volts[i] && voltage <= volts[i + 1]) {
      float ratio = (voltage - volts[i]) / (volts[i + 1] - volts[i]);
      return percents[i] + (int)((percents[i + 1] - percents[i]) * ratio + 0.5f);
    }
  }

  return 0;
}

void drawBatteryIcon(int x, int y, int w, int h, int percent) {
  display.drawRect(x, y, w, h, SSD1306_WHITE);
  display.fillRect(x + w, y + h / 3, 2, h / 3, SSD1306_WHITE);

  int innerWidth = w - 4;
  int fillWidth = map(percent, 0, 100, 0, innerWidth);

  if (fillWidth > 0) {
    display.fillRect(x + 2, y + 2, fillWidth, h - 4, SSD1306_WHITE);
  }
}

void setup() {
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED tidak terdeteksi!");
    while (true) {
      delay(100);
    }
  }

  analogReadResolution(12);
  analogSetPinAttenuation(BATTERY_ADC_PIN, ADC_11db);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(13, 10);
  display.println("SMART POWERBANK");
  display.display();
  delay(1200);
}

void loop() {
  float batteryVoltage = readBatteryVoltage();
  int percent = batteryPercent(batteryVoltage);

  Serial.printf("Battery: %.3f V | %d%%\n", batteryVoltage, percent);

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("BATTERY");

  display.setCursor(94, 0);
  display.print(percent);
  display.print("%");

  drawBatteryIcon(0, 11, 68, 14, percent);

  display.setTextSize(1);
  display.setCursor(78, 14);
  display.print(batteryVoltage, 2);
  display.print("V");

  display.display();

  delay(1000);
}
