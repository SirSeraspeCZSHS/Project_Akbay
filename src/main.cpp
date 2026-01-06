#include <Arduino.h>
#include <AccelStepper.h>

const int LED_PIN = 2;

// Set these pins to match your wiring
#define stepperYStep_pin 25 // pin for stepper Y axis' driver
#define stepperYDir_pin 27 // pin for stepper Y axis' direction

AccelStepper stepperY(AccelStepper::DRIVER, stepperYStep_pin, stepperYDir_pin); // create an object for stepper X axis

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("ESP32 PlatformIO AccelStepper example");

  stepperY.setMaxSpeed(1000);
  stepperY.setAcceleration(200);
  stepperY.moveTo(1000);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  // Non-blocking: run() steps the motor toward the target
  if (stepperY.distanceToGo() != 0) {
    stepperY.run();
  } else {
    // reverse direction when target reached
    stepperY.moveTo(-stepperY.currentPosition());
    delay(200);
  }
  digitalWrite(LED_PIN, LOW);
  delay(10);
  //test commit
}
