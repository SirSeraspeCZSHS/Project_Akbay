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

long maxSpeed = 10000.0; // max speed possible for stepper motor
long accel = 50000.0; // how fast does the stepper motor moves

// NEW: back-and-forth params using setSpeed() + runSpeed()
long travelDistance = 8000;     // travel limit in steps
long runSpeedValue = 10000;     // steps per second (use positive magnitude)
int direction = 1;              // 1 = forward, -1 = backward

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

  // start from zero and set initial constant speed (will be stepped by runSpeed)
  stepperX.setSpeed(runSpeedValue * direction);
  stepperY.setSpeed(runSpeedValue * direction);
  stepperZ.setSpeed(runSpeedValue * direction);

  // remove moveTo() usage since runSpeed() drives continuous motion
  // stepperX.moveTo(8000);
  // stepperY.moveTo(8000);
  // stepperZ.moveTo(8000);
}


void loop() {
  // set speeds each loop to ensure direction change is applied immediately
  stepperX.setSpeed(runSpeedValue * direction);
  stepperY.setSpeed(runSpeedValue * direction);
  stepperZ.setSpeed(runSpeedValue * direction);

  // runSpeed executes stepping at the set constant speed (non-blocking)
  stepperX.runSpeed();
  stepperY.runSpeed();
  stepperZ.runSpeed();

  // check primary axis (Y) for limit crossing and toggle direction when reached
  long pos = stepperY.currentPosition();
  if (direction > 0 && pos >= travelDistance) {
    direction = -1;
    Serial.println("Reversing direction -> BACK");
  } else if (direction < 0 && pos <= -travelDistance) {
    direction = 1;
    Serial.println("Reversing direction -> PORT");
  }
}