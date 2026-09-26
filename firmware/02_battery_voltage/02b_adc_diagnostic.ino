#include <Arduino.h>

#define BATTERY_ADC_PIN 4

void setup() {
  Serial.begin(115200);
  delay(1000);

  analogReadResolution(12);
  analogSetPinAttenuation(BATTERY_ADC_PIN, ADC_11db);

  Serial.println("=== SMART POWERBANK ADC DIAGNOSTIC ===");
  Serial.println("GPIO4 harus menerima tegangan dari titik tengah divider 10k/10k.");
  Serial.println("Jika ADC node > 2.3V, hentikan test dan cek wiring.");
}

void loop() {
  int raw = analogRead(BATTERY_ADC_PIN);
  int mv = analogReadMilliVolts(BATTERY_ADC_PIN);
  float adcV = mv / 1000.0f;
  float estimatedBattery = adcV * 2.0f;

  Serial.printf("RAW=%d | ADC=%d mV (%.3f V) | x2=%.3f V",
                raw, mv, adcV, estimatedBattery);

  if (adcV > 2.30f) {
    Serial.print("  <-- WARNING: ADC terlalu tinggi / divider salah / ADC saturasi");
  }

  Serial.println();
  delay(1000);
}
