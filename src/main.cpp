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


AccelStepper stepperX(AccelStepper::DRIVER, stepperX_elbow_Step_pin, stepperX_elbow_Dir_pin); // create an object for stepper X axis
AccelStepper stepperY(AccelStepper::DRIVER, stepperY_shoulder_flexion_Step_pin, stepperY_shoulder_flexion_Dir_pin); // create an object for stepper X axis
AccelStepper stepperZ(AccelStepper::DRIVER, stepperZ_shoulder_abduction_Step_pin, stepperZ_shoulder_abduction_Dir_pin);

long maxSpeed = 5000.0; // max speed possible for stepper motor
long accel = 5000.0; // how fast does the stepper motor moves

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("ESP32 PlatformIO AccelStepper example");

  stepperX.setMaxSpeed(maxSpeed);
  stepperX.setAcceleration(accel);

  stepperY.setMaxSpeed(maxSpeed);
  stepperY.setAcceleration(accel);

  stepperZ.setMaxSpeed(maxSpeed);
  stepperZ.setAcceleration(accel);

  stepperX.moveTo(2000);
  stepperY.moveTo(2000);
  stepperZ.moveTo(2000);


}


void loop() {
  if (stepperY.distanceToGo() != 0 || stepperX.distanceToGo() != 0 || stepperZ.distanceToGo() != 0) {
    stepperY.run();
    stepperX.run();
    stepperZ.run();
  } else {
    // reverse direction when target reached
    stepperX.moveTo(-stepperY.currentPosition());
     stepperY.moveTo(-stepperY.currentPosition()); 
     stepperZ.moveTo(-stepperY.currentPosition());
  
  }
}

