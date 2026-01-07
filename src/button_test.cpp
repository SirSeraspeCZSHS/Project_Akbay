#include <Arduino.h>

// Buttons to GND, using internal pull-ups
const int BTN1_PIN = 13;
const int BTN2_PIN = 5;

const unsigned long DEBOUNCE_MS = 50;

int lastRaw1 = HIGH, lastRaw2 = HIGH;
int stable1 = HIGH, stable2 = HIGH;
unsigned long lastDebounce1 = 0, lastDebounce2 = 0;

void setup() {
  Serial.begin(115200);
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  Serial.println("Button test ready. BTN1=GPIO12, BTN2=GPIO5 (press to GND).");
}

void checkButton(int pin, int &lastRaw, int &stable, unsigned long &lastDebounce, const char* name) {
  int raw = digitalRead(pin);
  if (raw != lastRaw) lastDebounce = millis();
  if (millis() - lastDebounce > DEBOUNCE_MS) {
    if (raw != stable) {
      stable = raw;
      if (stable == LOW) {
        Serial.printf("%s pressed (pin %d)\n", name, pin);
      } else {
        Serial.printf("%s released (pin %d)\n", name, pin);
      }
    }
  }
  lastRaw = raw;
}

void loop() {
  checkButton(BTN1_PIN, lastRaw1, stable1, lastDebounce1, "BTN1");
  checkButton(BTN2_PIN, lastRaw2, stable2, lastDebounce2, "BTN2");
  delay(5);
}
