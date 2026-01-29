#include <Arduino.h>
#include <AccelStepper.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>


// stepper pins (match your wiring)
#define STEP_ARM 26
#define DIR_ARM 16

#define STEP_BICEP 25
#define DIR_BICEP 27

#define STEP_SHOULDER 17
#define DIR_SHOULDER 14

// control buttons (buttons to GND, use internal pullups)
const int ARM_UP = 2;   
const int ARM_DOWN = 34;   
const int BICEP_UP = 35;   
const int BICEP_DOWN = 4; 
const int SHOULDER_UP = 36;   
const int SHOULDER_DOWN = 39; 

AccelStepper stepperX(AccelStepper::DRIVER, STEP_ARM, DIR_ARM);
AccelStepper stepperY(AccelStepper::DRIVER, STEP_BICEP, DIR_BICEP);
AccelStepper stepperZ(AccelStepper::DRIVER, STEP_SHOULDER, DIR_SHOULDER);

MFRC522DriverPinSimple ss_pin(5);
MFRC522DriverSPI driver{ss_pin};
MFRC522 mfrc522{driver};

// runtime parameters
const float RUN_SPEED = 1200.0f;   // steps per second (magnitude)
const unsigned long DEBOUNCE_MS = 50UL;

// debounce state for both buttons
int lastRawCW = HIGH, lastRawCCW = HIGH;
int stableCW = HIGH, stableCCW = HIGH;
unsigned long lastDebounceCW = 0, lastDebounceCCW = 0;

void setup() {
  Serial.begin(115200);
    while (!Serial);

  pinMode(ARM_UP, INPUT);
  pinMode(ARM_DOWN, INPUT);
  pinMode(BICEP_UP, INPUT);
  pinMode(BICEP_DOWN, INPUT);
  pinMode(SHOULDER_UP, INPUT);
  pinMode(SHOULDER_DOWN, INPUT);

  mfrc522.PCD_Init();
  Serial.println("RFID + Button Test Started");

  // configure steppers (using runSpeed() => setSpeed() controls direction/velocity)
  stepperX.setMaxSpeed(RUN_SPEED);
  stepperX.setCurrentPosition(0);

  stepperY.setMaxSpeed(RUN_SPEED);
  stepperY.setCurrentPosition(0);

  stepperZ.setMaxSpeed(RUN_SPEED);
  stepperZ.setCurrentPosition(0);

  Serial.println("Button control ready. BTN_CW=GPIO13, BTN_CCW=GPIO5");
}

void loop() {
  // read raw
  int rawCW = digitalRead(BTN_CW);
  int rawCCW = digitalRead(BTN_CCW);

  // debounce CW button
  if (rawCW != lastRawCW) lastDebounceCW = millis();
  if (millis() - lastDebounceCW > DEBOUNCE_MS) {
    if (rawCW != stableCW) {
      stableCW = rawCW;
      Serial.print("BTN_CW ");
      Serial.println(stableCW == HIGH ? "pressed" : "released"); // INPUT_PULLUP: LOW = pressed
    }
  }
  lastRawCW = rawCW;

  // debounce CCW button
  if (rawCCW != lastRawCCW) lastDebounceCCW = millis();
  if (millis() - lastDebounceCCW > DEBOUNCE_MS) {
    if (rawCCW != stableCCW) {
      stableCCW = rawCCW;
      Serial.print("BTN_CCW ");
      Serial.println(stableCCW == HIGH ? "pressed" : "released"); // INPUT_PULLUP: LOW = pressed
    }
  }
  lastRawCCW = rawCCW;

  // If both buttons have the same debounced state -> STOP all steppers
  if (stableCW == stableCCW) {
    stepperX.setSpeed(0);
    stepperY.setSpeed(0);
    stepperZ.setSpeed(0);
  } else {
    // Buttons differ: determine which one is pressed (LOW)
    if (stableCW == LOW) {
      // CW pressed -> clockwise (positive)
      stepperX.setSpeed(RUN_SPEED);
      stepperY.setSpeed(RUN_SPEED);
      stepperZ.setSpeed(RUN_SPEED);
    } else if (stableCCW == LOW) {
      // CCW pressed -> counter-clockwise (negative)
      stepperX.setSpeed(-RUN_SPEED);
      stepperY.setSpeed(-RUN_SPEED);
      stepperZ.setSpeed(-RUN_SPEED);

    }
  }

  // Run steppers at the setSpeed (non-blocking). If speed==0 they won't step.
  stepperX.runSpeed();
  stepperY.runSpeed();
  stepperZ.runSpeed();
  //Test upload
}