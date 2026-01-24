#include <Arduino.h>
#include <AccelStepper.h>

// stepper pins (match your wiring)
#define STEP_X 26
#define DIR_X 16

#define STEP_Y 25
#define DIR_Y 27

#define STEP_Z 17
#define DIR_Z 14

// control buttons (buttons to GND, use internal pullups)
const int BTN_CW = 13;   // clockwise
const int BTN_CCW = 5;   // counter-clockwise

AccelStepper stepperX(AccelStepper::DRIVER, STEP_X, DIR_X);
AccelStepper stepperY(AccelStepper::DRIVER, STEP_Y, DIR_Y);
AccelStepper stepperZ(AccelStepper::DRIVER, STEP_Z, DIR_Z);

// runtime parameters
const float RUN_SPEED = 1200.0f;   // steps per second (magnitude)
const unsigned long DEBOUNCE_MS = 50UL;

// debounce state for both buttons
int lastRawCW = HIGH, lastRawCCW = HIGH;
int stableCW = HIGH, stableCCW = HIGH;
unsigned long lastDebounceCW = 0, lastDebounceCCW = 0;

void setup() {
  Serial.begin(115200);

  pinMode(BTN_CW, INPUT_PULLUP);
  pinMode(BTN_CCW, INPUT_PULLUP);

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