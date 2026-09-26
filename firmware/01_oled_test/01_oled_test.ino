#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SDA_PIN 5
#define SCL_PIN 6

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

void setup() {
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED tidak terdeteksi!");
    while (true) {
      delay(100);
    }
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(20, 2);
  display.println("SMART POWERBANK");

  display.setTextSize(2);
  display.setCursor(35, 15);
  display.println("TEST");

  display.display();

  Serial.println("OLED berhasil dijalankan.");
}

void loop() {
}
