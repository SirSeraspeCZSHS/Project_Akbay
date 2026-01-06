#include <Arduino.h>
#include <AccelStepper.h>

const int LED_PIN = 2;

// Set these pins to match your wiring
const int STEP_PIN = 18;
const int DIR_PIN = 19;

AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("ESP32 PlatformIO AccelStepper example");

  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(200);
  stepper.moveTo(1000);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  // Non-blocking: run() steps the motor toward the target
  if (stepper.distanceToGo() != 0) {
    stepper.run();
  } else {
    // reverse direction when target reached
    stepper.moveTo(-stepper.currentPosition());
    delay(200);
  }
  digitalWrite(LED_PIN, LOW);
  delay(10);
}
