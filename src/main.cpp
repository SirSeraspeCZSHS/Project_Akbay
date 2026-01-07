// #include <Arduino.h>
// #include <AccelStepper.h>

// const int LED_PIN = 2;

// // Set these pins to match your wiring

// #define stepperX_elbow_Step_pin 26 // pin for stepper X axis' driver
// #define stepperX_elbow_Dir_pin 16 // pin for stepper X axis' direction/dating 16
// #define XLeftbutton 14 // pin for X axis left button

// #define stepperY_shoulder_flexion_Step_pin 25 // pin for stepper Y axis' driver
// #define stepperY_shoulder_flexion_Dir_pin 27 // pin for stepper Y axis' direction
// #define YTopbutton13 13 //pin for Y axis  button

// #define stepperZ_shoulder_abduction_Step_pin 17 // pin for stepper Z axis' driver, dating 17
// #define stepperZ_shoulder_abduction_Dir_pin 14 // pin for stepper Z axis' direction


// AccelStepper stepperX(AccelStepper::DRIVER, stepperX_elbow_Step_pin, stepperX_elbow_Dir_pin); // create an object for stepper X axis
// AccelStepper stepperY(AccelStepper::DRIVER, stepperY_shoulder_flexion_Step_pin, stepperY_shoulder_flexion_Dir_pin); // create an object for stepper X axis
// AccelStepper stepperZ(AccelStepper::DRIVER, stepperZ_shoulder_abduction_Step_pin, stepperZ_shoulder_abduction_Dir_pin);

// long maxSpeed = 10000.0; // max speed possible for stepper motor
// long accel = 50000.0; // how fast does the stepper motor moves

// // NEW: back-and-forth params using AccelStepper acceleration (moveTo + run)
// long travelDistance = 8000;     // travel limit in steps
// long runSpeedValue = 10000;     // used as max speed
// int direction = 1;              // 1 = forward, -1 = backward

// // Start button config
// const int startButtonPin = YTopbutton13;
// bool running = false;
// unsigned long lastDebounceTime = 0;
// const unsigned long debounceDelay = 50;
// int lastButtonState = HIGH;


// void setup() {
//   Serial.begin(115200);
//   pinMode(LED_PIN, OUTPUT);
//   Serial.println("ESP32 PlatformIO AccelStepper example");

//   // Use run() so acceleration is applied; set max speed and acceleration
//   stepperX.setMaxSpeed(runSpeedValue);
//   stepperX.setAcceleration(accel);
  
//   stepperY.setMaxSpeed(runSpeedValue);
//   stepperY.setAcceleration(accel);


//   stepperZ.setMaxSpeed(runSpeedValue);
//   stepperZ.setAcceleration(accel);


//   // start moving to the initial target (will accelerate)
//   stepperX.moveTo(travelDistance);
//   stepperY.moveTo(travelDistance);
//   stepperZ.moveTo(travelDistance);

//   // button as input with pullup (press to GND)
//   pinMode(startButtonPin, INPUT_PULLUP);

//   // do NOT start moving until button pressed
//   // steppers remain idle until running == true
// }



// void loop() {
//   // run() respects acceleration/decelleration and is non-blocking
//   // Check start button (debounced) — latched start when button goes LOW
//   int reading = digitalRead(startButtonPin);
//   if (reading != lastButtonState) {
//     lastDebounceTime = millis();
//   }

//   if ((millis() - lastDebounceTime) > debounceDelay) {
//     if (!running && reading == LOW) {
//       // button pressed -> start motion
//       running = true;
//       direction = 1;
//       long target = direction * travelDistance;
//   stepperX.moveTo(target);
//   stepperY.moveTo(target);
//   stepperZ.moveTo(target);
//  Serial.println("Start button pressed: starting steppers");
//       digitalWrite(LED_PIN, HIGH);

//     }
//  }
//    lastButtonState = reading;

// if (!running) {
//     // not running yet — do nothing
//     return;
//   }

//   // run() respects acceleration/deceleration and is non-blocking
//   stepperX.run();
//   stepperY.run();
//   stepperZ.run();

   
//   // when primary axis (Y) reaches its target, toggle direction and set new targets
//   if (stepperY.distanceToGo() == 0) {
//     direction = -direction;
//     long target = direction * travelDistance;
//     stepperX.moveTo(target);
//     stepperY.moveTo(target);
//     stepperZ.moveTo(target);

//     if (direction > 0) Serial.println("Reversing direction -> PORT");
//     else Serial.println("Reversing direction -> BACK");
//   }
// }