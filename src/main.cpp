#include <Arduino.h>
#include <AccelStepper.h>

const int LED_PIN = 2;

// Set these pins to match your wiring

#define stepperX_elbow_Step_pin 26 // pin for stepper X axis' driver
#define stepperX_elbow_Dir_pin 16 // pin for stepper X axis' direction/dating 16

#define stepperY_shoulder_flexion_Step_pin 25 // pin for stepper Y axis' driver
#define stepperY_shoulder_flexion_Dir_pin 27 // pin for stepper Y axis' direction

#define stepperZ_shoulder_abduction_Step_pin 17 // pin for stepper Z axis' driver, dating 17
#define stepperZ_shoulder_abduction_Dir_pin 14 // pin for stepper Z axis' direction


AccelStepper stepperX(AccelStepper::DRIVER, stepperX_elbow_Step_pin, stepperX_elbow_Dir_pin); 
AccelStepper stepperY(AccelStepper::DRIVER, stepperY_shoulder_flexion_Step_pin, stepperY_shoulder_flexion_Dir_pin); 
AccelStepper stepperZ(AccelStepper::DRIVER, stepperZ_shoulder_abduction_Step_pin, stepperZ_shoulder_abduction_Dir_pin);

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
