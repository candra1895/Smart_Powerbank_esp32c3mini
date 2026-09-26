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

const float DIVIDER_RATIO = 2.0f;
const float CALIBRATION = 1.0000f;

const bool FORCE_CHARGING_ANIMATION = false;

const float BATTERY_MIN_VALID = 2.50f;
const float BATTERY_MAX_VALID = 4.35f;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float filteredVoltage = 0.0f;
float compareVoltage = 0.0f;
unsigned long lastCompareMs = 0;
unsigned long chargingHoldUntil = 0;

const unsigned long COMPARE_INTERVAL_MS = 10000;
const unsigned long CHARGING_HOLD_MS = 90000;
const float CHARGE_RISE_THRESHOLD = 0.010f;
const float DROP_CANCEL_THRESHOLD = 0.025f;

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
      return percents[i] +
             (int)((percents[i + 1] - percents[i]) * ratio + 0.5f);
    }
  }

  return 0;
}

bool updateChargingDetection(float voltage) {
  if (FORCE_CHARGING_ANIMATION) return true;

  unsigned long now = millis();

  if (filteredVoltage <= 0.1f) {
    filteredVoltage = voltage;
    compareVoltage = voltage;
    lastCompareMs = now;
  } else {
    filteredVoltage = filteredVoltage * 0.85f + voltage * 0.15f;
  }

  if (now - lastCompareMs >= COMPARE_INTERVAL_MS) {
    float delta = filteredVoltage - compareVoltage;

    if (delta >= CHARGE_RISE_THRESHOLD && filteredVoltage < 4.19f) {
      chargingHoldUntil = now + CHARGING_HOLD_MS;
    }

    if (delta <= -DROP_CANCEL_THRESHOLD) {
      chargingHoldUntil = 0;
    }

    compareVoltage = filteredVoltage;
    lastCompareMs = now;
    Serial.printf("Trend: %.3f V | delta=%+.3f V\n", filteredVoltage, delta);
  }

  return now < chargingHoldUntil;
}

void drawNormalBattery(int x, int y, int w, int h, int percent) {
  display.drawRect(x, y, w, h, SSD1306_WHITE);
  display.fillRect(x + w, y + h / 3, 2, h / 3, SSD1306_WHITE);

  int innerW = w - 4;
  int fillW = map(percent, 0, 100, 0, innerW);

  if (fillW > 0) {
    display.fillRect(x + 2, y + 2, fillW, h - 4, SSD1306_WHITE);
  }
}

void drawChargingBattery(int x, int y, int w, int h) {
  display.drawRect(x, y, w, h, SSD1306_WHITE);
  display.fillRect(x + w, y + h / 3, 2, h / 3, SSD1306_WHITE);

  int frame = (millis() / 300) % 5;
  int innerW = w - 4;
  int fillW = map(frame, 0, 4, 2, innerW);

  display.fillRect(x + 2, y + 2, fillW, h - 4, SSD1306_WHITE);
}

void showSensorError(float voltage) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("CHECK BAT SENSE");
  display.setCursor(0, 12);
  display.print("READ: ");
  display.print(voltage, 2);
  display.println("V");
  display.setCursor(0, 23);
  display.print("USE PRE-BOOST");
  display.display();
}

void showNoBattery() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(20, 6);
  display.println("NO BATTERY");
  display.setCursor(12, 19);
  display.println("CHECK B+/OUT+");
  display.display();
}

void setup() {
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED tidak terdeteksi!");
    while (true) delay(100);
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

  if (batteryVoltage > BATTERY_MAX_VALID) {
    chargingHoldUntil = 0;
    filteredVoltage = 0.0f;

    Serial.printf(
      "SENSOR ERROR: %.3f V | Sense kemungkinan terhubung ke 5V boost/output\n",
      batteryVoltage
    );

    showSensorError(batteryVoltage);
    delay(500);
    return;
  }

  if (batteryVoltage < BATTERY_MIN_VALID) {
    chargingHoldUntil = 0;
    filteredVoltage = 0.0f;

    Serial.printf("NO BATTERY / LOW SENSE: %.3f V\n", batteryVoltage);

    showNoBattery();
    delay(500);
    return;
  }

  int percent = batteryPercent(batteryVoltage);
  bool charging = updateChargingDetection(batteryVoltage);

  Serial.printf(
    "Battery: %.3f V | %d%% | %s\n",
    batteryVoltage,
    percent,
    charging ? "CHARGING" : "IDLE"
  );

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);

  if (charging) {
    display.print("CHARGING");
    display.setCursor(94, 0);
    display.print(percent);
    display.print("%");

    drawChargingBattery(0, 11, 68, 14);

    display.setCursor(75, 12);
    display.print(">>");
    display.setCursor(92, 12);
    display.print(batteryVoltage, 2);
    display.print("V");
  } else {
    display.print("BATTERY");
    display.setCursor(94, 0);
    display.print(percent);
    display.print("%");

    drawNormalBattery(0, 11, 68, 14, percent);

    display.setCursor(78, 14);
    display.print(batteryVoltage, 2);
    display.print("V");
  }

  display.display();
  delay(250);
}
