#include <Arduino.h>
#include <AccelStepper.h>

const int LED_PIN = 2;

// Set these pins to match your wiring
#define stepperYStep_pin 25 // pin for stepper Y axis' driver
#define stepperYDir_pin 27 // pin for stepper Y axis' direction

AccelStepper stepperX(AccelStepper::DRIVER, stepperXStep_pin, stepperXDir_pin); // create an object for stepper X axis

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
  //test commit
}
