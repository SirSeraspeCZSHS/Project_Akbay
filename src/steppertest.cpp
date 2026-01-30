// #include <Arduino.h>
// #include <AccelStepper.h>

// // stepper pins (match your wiring)
// #define STEP_ARM 26
// #define DIR_ARM 16

// #define STEP_BICEP 25
// #define DIR_BICEP 27

// #define STEP_SHOULDER 17
// #define DIR_SHOULDER 14

// // measurement buttons (buttons to GND, use INPUT_PULLUP)
// // Per request: START = pin 2, STOP = pin 34
// #define START_BUTTON 2
// #define STOP_BUTTON 34

// // motion & conversion (adjust per motor geometry)
// const float MEASURE_SPEED = 800.0f; // steps/sec
// const float STEPS_PER_MM_ARM = 200.0f;      // steps per mm for ARM
// const float STEPS_PER_MM_BICEP = 200.0f;    // steps per mm for BICEP
// const float STEPS_PER_MM_SHOULDER = 200.0f; // steps per mm for SHOULDER
// const unsigned long MAX_STEPS = 20000UL;    // per-motor maximum steps from initial zero

// AccelStepper stepperArm(AccelStepper::DRIVER, STEP_ARM, DIR_ARM);
// AccelStepper stepperBicep(AccelStepper::DRIVER, STEP_BICEP, DIR_BICEP);
// AccelStepper stepperShoulder(AccelStepper::DRIVER, STEP_SHOULDER, DIR_SHOULDER);

// // measurement state
// bool measuring = false;
// long lastPosArm = 0, lastPosBicep = 0, lastPosShoulder = 0;
// unsigned long stepsArm = 0, stepsBicep = 0, stepsShoulder = 0;

// // // helper: simple debounce read for active-LOW buttons
// // bool pressed(int pin) {
// //   if (digitalRead(pin) == LOW) {
// //     delay(20);
// //     if (digitalRead(pin) == LOW) return true;
// //   }
// //   return false;
// // }

// // void setup() {
// //   Serial.begin(115200);
// //   while (!Serial) ;

// //   pinMode(START_BUTTON, INPUT_PULLUP);
// //   pinMode(STOP_BUTTON, INPUT_PULLUP);

// //   stepperArm.setMaxSpeed(MEASURE_SPEED);
// //   stepperBicep.setMaxSpeed(MEASURE_SPEED);
// //   stepperShoulder.setMaxSpeed(MEASURE_SPEED);

// //   stepperArm.setCurrentPosition(0);
// //   stepperBicep.setCurrentPosition(0);
// //   stepperShoulder.setCurrentPosition(0);

// //   Serial.println("Multi-stepper distance tester");
// //   Serial.println("START (pin 2) to begin movement, STOP (pin 34) to stop and record steps.");
// //   Serial.println("Set STEPS_PER_MM_* to match your mechanics.");
// // }

// // void loop() {
// //   if (!measuring) {
// //     if (pressed(START_BUTTON)) {
// //       // begin measurement
// //       measuring = true;
// //       stepsArm = stepsBicep = stepsShoulder = 0;
// //       lastPosArm = stepperArm.currentPosition();
// //       lastPosBicep = stepperBicep.currentPosition();
// //       lastPosShoulder = stepperShoulder.currentPosition();

// //       stepperArm.setSpeed(MEASURE_SPEED);      // forward
// //       stepperBicep.setSpeed(MEASURE_SPEED);
// //       stepperShoulder.setSpeed(MEASURE_SPEED);

// //       Serial.println("Measurement started for all steppers...");
// //       delay(200); // debounce
// //     }
// //   } else {
// //     // run all steppers at constant speed and count steps
// //     stepperArm.runSpeed();
// //     stepperBicep.runSpeed();
// //     stepperShoulder.runSpeed();

// //     long p;
// //     p = stepperArm.currentPosition();
// //     if (p != lastPosArm) {
// //       stepsArm += (unsigned long)abs(p - lastPosArm);
// //       lastPosArm = p;
// //     }

// //     p = stepperBicep.currentPosition();
// //     if (p != lastPosBicep) {
// //       stepsBicep += (unsigned long)abs(p - lastPosBicep);
// //       lastPosBicep = p;
// //     }

// //     p = stepperShoulder.currentPosition();
// //     if (p != lastPosShoulder) {
// //       stepsShoulder += (unsigned long)abs(p - lastPosShoulder);
// //       lastPosShoulder = p;
// //     }

// //     // stop when any stepper reaches the configured MAX_STEPS
// //     if (stepsArm >= MAX_STEPS || stepsBicep >= MAX_STEPS || stepsShoulder >= MAX_STEPS) {
// //       measuring = false;
// //       stepperArm.setSpeed(0);
// //       stepperBicep.setSpeed(0);
// //       stepperShoulder.setSpeed(0);

// //       // capture final positions
// //       long finalArm = stepperArm.currentPosition();
// //       long finalBicep = stepperBicep.currentPosition();
// //       long finalShoulder = stepperShoulder.currentPosition();
// //       if (finalArm != lastPosArm) stepsArm += (unsigned long)abs(finalArm - lastPosArm);
// //       if (finalBicep != lastPosBicep) stepsBicep += (unsigned long)abs(finalBicep - lastPosBicep);
// //       if (finalShoulder != lastPosShoulder) stepsShoulder += (unsigned long)abs(finalShoulder - lastPosShoulder);

// //       float mmArm = stepsArm / STEPS_PER_MM_ARM;
// //       float mmBicep = stepsBicep / STEPS_PER_MM_BICEP;
// //       float mmShoulder = stepsShoulder / STEPS_PER_MM_SHOULDER;

// //       Serial.println("Measurement stopped: step limit reached (20000).");
// //       if (stepsArm >= MAX_STEPS) Serial.println("Arm reached step limit.");
// //       if (stepsBicep >= MAX_STEPS) Serial.println("Bicep reached step limit.");
// //       if (stepsShoulder >= MAX_STEPS) Serial.println("Shoulder reached step limit.");

// //       Serial.print("Arm total steps: "); Serial.print(stepsArm); Serial.print(" (" ); Serial.print(mmArm,2); Serial.println(" mm)");
// //       Serial.print("Bicep total steps: "); Serial.print(stepsBicep); Serial.print(" (" ); Serial.print(mmBicep,2); Serial.println(" mm)");
// //       Serial.print("Shoulder total steps: "); Serial.print(stepsShoulder); Serial.print(" (" ); Serial.print(mmShoulder,2); Serial.println(" mm)");
// //       Serial.println("--- Ready for next measurement ---");
// //       delay(300); // debounce
// //     }

// //     // periodic feedback every 200 combined steps
// //     static unsigned long nextReportTotal = 0;
// //     unsigned long total = stepsArm + stepsBicep + stepsShoulder;
// //     if (total >= nextReportTotal) {
// //       float mmArm = stepsArm / STEPS_PER_MM_ARM;
// //       float mmBicep = stepsBicep / STEPS_PER_MM_BICEP;
// //       float mmShoulder = stepsShoulder / STEPS_PER_MM_SHOULDER;
// //       Serial.print("Arm steps: "); Serial.print(stepsArm); Serial.print(" (" ); Serial.print(mmArm,2); Serial.println(" mm)");
// //       Serial.print("Bicep steps: "); Serial.print(stepsBicep); Serial.print(" (" ); Serial.print(mmBicep,2); Serial.println(" mm)");
// //       Serial.print("Shoulder steps: "); Serial.print(stepsShoulder); Serial.print(" (" ); Serial.print(mmShoulder,2); Serial.println(" mm)");
// //       nextReportTotal = total + 200;
// //     }

// //     // stop when STOP pressed
// //     if (pressed(STOP_BUTTON)) {
// //       measuring = false;
// //       stepperArm.setSpeed(0);
// //       stepperBicep.setSpeed(0);
// //       stepperShoulder.setSpeed(0);

// //       // capture final positions
// //       long finalArm = stepperArm.currentPosition();
// //       long finalBicep = stepperBicep.currentPosition();
// //       long finalShoulder = stepperShoulder.currentPosition();
// //       if (finalArm != lastPosArm) stepsArm += (unsigned long)abs(finalArm - lastPosArm);
// //       if (finalBicep != lastPosBicep) stepsBicep += (unsigned long)abs(finalBicep - lastPosBicep);
// //       if (finalShoulder != lastPosShoulder) stepsShoulder += (unsigned long)abs(finalShoulder - lastPosShoulder);

// //       float mmArm = stepsArm / STEPS_PER_MM_ARM;
// //       float mmBicep = stepsBicep / STEPS_PER_MM_BICEP;
// //       float mmShoulder = stepsShoulder / STEPS_PER_MM_SHOULDER;

// //       Serial.println("Measurement stopped.");
// //       Serial.print("Arm total steps: "); Serial.print(stepsArm); Serial.print(" (" ); Serial.print(mmArm,2); Serial.println(" mm)");
// //       Serial.print("Bicep total steps: "); Serial.print(stepsBicep); Serial.print(" (" ); Serial.print(mmBicep,2); Serial.println(" mm)");
// //       Serial.print("Shoulder total steps: "); Serial.print(stepsShoulder); Serial.print(" (" ); Serial.print(mmShoulder,2); Serial.println(" mm)");
// //       Serial.println("--- Ready for next measurement ---");
// //       delay(300); // debounce
// //     }
// //   }
// // }

