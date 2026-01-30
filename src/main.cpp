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

AccelStepper stepperArm(AccelStepper::DRIVER, STEP_ARM, DIR_ARM);
AccelStepper stepperBicep(AccelStepper::DRIVER, STEP_BICEP, DIR_BICEP);
AccelStepper stepperShoulder(AccelStepper::DRIVER, STEP_SHOULDER, DIR_SHOULDER);

MFRC522DriverPinSimple ss_pin(5);
MFRC522DriverSPI driver{ss_pin};
MFRC522 mfrc522{driver};

// Buzzer and authorized UIDs
const int BUZZER_PIN = 13;
// Replace the example below with your authorized UID(s) (format: AA:BB:CC:DD)
const char* allowedUIDs[] = { "F9:6D:FC:14" };
const int ALLOWED_UIDS_COUNT = sizeof(allowedUIDs) / sizeof(allowedUIDs[0]);

// Authentication state
bool authenticated = false;

bool isAllowedUID(const String &uid);
void buzzSuccess();
void buzzFail();

// runtime parameters
const float RUN_SPEED = 1200.0f;   // steps per second (magnitude)
const unsigned long DEBOUNCE_MS = 50UL;
const unsigned long MAX_STEPS = 20000UL; // per-motor maximum steps from power-up

// step tracking and limits
long lastPosArm = 0, lastPosBicep = 0, lastPosShoulder = 0;
unsigned long stepsArm = 0, stepsBicep = 0, stepsShoulder = 0;

bool armAtLimit = false, bicepAtLimit = false, shoulderAtLimit = false;

// debounce state for each button
int lastRawArmUp = HIGH, lastRawArmDown = HIGH;
int stableArmUp = HIGH, stableArmDown = HIGH;
unsigned long lastDebounceArmUp = 0, lastDebounceArmDown = 0;

int lastRawBicepUp = HIGH, lastRawBicepDown = HIGH;
int stableBicepUp = HIGH, stableBicepDown = HIGH;
unsigned long lastDebounceBicepUp = 0, lastDebounceBicepDown = 0;

int lastRawShoulderUp = HIGH, lastRawShoulderDown = HIGH;
int stableShoulderUp = HIGH, stableShoulderDown = HIGH;
unsigned long lastDebounceShoulderUp = 0, lastDebounceShoulderDown = 0;

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

  // Initialize buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // --- Blocking RFID authentication at startup ---
  Serial.println("Scan authorized RFID to start the program...");
  while (!authenticated) {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      String uidStr = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        char buf[3];
        sprintf(buf, "%02X", mfrc522.uid.uidByte[i]);
        uidStr += String(buf);
        if (i + 1 < mfrc522.uid.size) uidStr += ":";
      }
      Serial.print("Scanned UID: ");
      Serial.println(uidStr);

      if (isAllowedUID(uidStr)) {
        Serial.println("Authorized UID detected. Starting program.");
        buzzSuccess();
        authenticated = true;
      } else {
        Serial.println("Unauthorized UID. Try again.");
        buzzFail();
      }
      mfrc522.PICC_HaltA();
    }
    delay(50);
  }

  // configure steppers (using runSpeed() => setSpeed() controls direction/velocity)
  stepperArm.setMaxSpeed(RUN_SPEED);
  stepperArm.setCurrentPosition(0);

  stepperBicep.setMaxSpeed(RUN_SPEED);
  stepperBicep.setCurrentPosition(0);

  stepperShoulder.setMaxSpeed(RUN_SPEED);
  stepperShoulder.setCurrentPosition(0);

  // initialize tracking from the current (zero) positions
  lastPosArm = stepperArm.currentPosition();
  lastPosBicep = stepperBicep.currentPosition();
  lastPosShoulder = stepperShoulder.currentPosition();
  stepsArm = (unsigned long)abs(lastPosArm);
  stepsBicep = (unsigned long)abs(lastPosBicep);
  stepsShoulder = (unsigned long)abs(lastPosShoulder);

  Serial.println("Button control ready. ARM/BICEP/SHOULDER up/down control enabled");
  Serial.println("Each motor has a max of 20000 steps from power-up to avoid jams.");
}

// Debounce helper function
void debounceButton(int pin, int &lastRaw, int &stable, unsigned long &lastDebounce, const char* name) {
  int raw = digitalRead(pin);
  
  if (raw != lastRaw) lastDebounce = millis();
  if (millis() - lastDebounce > DEBOUNCE_MS) {
    if (raw != stable) {
      stable = raw;
      Serial.print(name);
      Serial.println(stable == HIGH ? " pressed" : " released");
    }
  }
  lastRaw = raw;
}

// Check UID against allowed list
bool isAllowedUID(const String &uid) {
  for (int i = 0; i < ALLOWED_UIDS_COUNT; i++) {
    if (uid.equalsIgnoreCase(String(allowedUIDs[i]))) return true;
  }
  return false;
}

// Simple buzzer patterns
void buzzSuccess() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);
  delay(100);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);
}

void buzzFail() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(300);
  digitalWrite(BUZZER_PIN, LOW);
}



void loop() {
  // Debounce all buttons
  debounceButton(ARM_UP, lastRawArmUp, stableArmUp, lastDebounceArmUp, "ARM_UP");
  debounceButton(ARM_DOWN, lastRawArmDown, stableArmDown, lastDebounceArmDown, "ARM_DOWN");
  
  debounceButton(BICEP_UP, lastRawBicepUp, stableBicepUp, lastDebounceBicepUp, "BICEP_UP");
  debounceButton(BICEP_DOWN, lastRawBicepDown, stableBicepDown, lastDebounceBicepDown, "BICEP_DOWN");
  
  debounceButton(SHOULDER_UP, lastRawShoulderUp, stableShoulderUp, lastDebounceShoulderUp, "SHOULDER_UP");
  debounceButton(SHOULDER_DOWN, lastRawShoulderDown, stableShoulderDown, lastDebounceShoulderDown, "SHOULDER_DOWN");

  // ARM control (prevent moving past 0..MAX_STEPS)
  if (stableArmUp == HIGH && stableArmDown == LOW) {
    if (!armAtLimit && stepperArm.currentPosition() < (long)MAX_STEPS) {
      stepperArm.setSpeed(RUN_SPEED);  // ARM UP
    } else {
      stepperArm.setSpeed(0);  // at limit, don't move
      if (!armAtLimit) {
        armAtLimit = true;
        Serial.println("Arm reached step limit and is disabled (pre-check).");
      }
    }
  } else if (stableArmDown == HIGH && stableArmUp == LOW) {
    if (stepperArm.currentPosition() > 0) {
      stepperArm.setSpeed(-RUN_SPEED);  // ARM DOWN
    } else {
      stepperArm.setSpeed(0);  // at zero, don't move further
    }
  } else {
    stepperArm.setSpeed(0);  // STOP
  }

  // BICEP control (prevent moving past 0..MAX_STEPS)
  if (stableBicepUp == HIGH && stableBicepDown == LOW) {
    if (!bicepAtLimit && stepperBicep.currentPosition() < (long)MAX_STEPS) {
      stepperBicep.setSpeed(RUN_SPEED);  // BICEP UP
    } else {
      stepperBicep.setSpeed(0);
      if (!bicepAtLimit) {
        bicepAtLimit = true;
        Serial.println("Bicep reached step limit and is disabled (pre-check).");
      }
    }
  } else if (stableBicepDown == HIGH && stableBicepUp == LOW) {
    if (stepperBicep.currentPosition() > 0) {
      stepperBicep.setSpeed(-RUN_SPEED);  // BICEP DOWN
    } else {
      stepperBicep.setSpeed(0);
    }
  } else {
    stepperBicep.setSpeed(0);  // STOP
  }

  // SHOULDER control (prevent moving past 0..MAX_STEPS)
  if (stableShoulderUp == HIGH && stableShoulderDown == LOW) {
    if (!shoulderAtLimit && stepperShoulder.currentPosition() < (long)MAX_STEPS) {
      stepperShoulder.setSpeed(RUN_SPEED);  // SHOULDER UP
    } else {
      stepperShoulder.setSpeed(0);
      if (!shoulderAtLimit) {
        shoulderAtLimit = true;
        Serial.println("Shoulder reached step limit and is disabled (pre-check).");
      }
    }
  } else if (stableShoulderDown == HIGH && stableShoulderUp == LOW) {
    if (stepperShoulder.currentPosition() > 0) {
      stepperShoulder.setSpeed(-RUN_SPEED);  // SHOULDER DOWN
    } else {
      stepperShoulder.setSpeed(0);
    }
  } else {
    stepperShoulder.setSpeed(0);  // STOP
  }



  // Run steppers at the setSpeed (non-blocking). If speed==0 they won't step.
  stepperArm.runSpeed();
  stepperBicep.runSpeed();
  stepperShoulder.runSpeed();

  // update positions and enforce hard limits (clamp and disable when reached)
  long p;

  p = stepperArm.currentPosition();
  if (p != lastPosArm) {
    lastPosArm = p;
    stepsArm = (unsigned long)abs(p);
    if (stepsArm >= MAX_STEPS) {
      if (!armAtLimit) {
        Serial.println("Arm reached step limit (20000) and is now disabled.");
        armAtLimit = true;
      }
      stepperArm.setCurrentPosition((long)MAX_STEPS);
      stepsArm = MAX_STEPS;
      stepperArm.setSpeed(0);
    } else if (p <= 0) {
      stepperArm.setCurrentPosition(0);
      stepsArm = 0;
    }
  }

  p = stepperBicep.currentPosition();
  if (p != lastPosBicep) {
    lastPosBicep = p;
    stepsBicep = (unsigned long)abs(p);
    if (stepsBicep >= MAX_STEPS) {
      if (!bicepAtLimit) {
        Serial.println("Bicep reached step limit (20000) and is now disabled.");
        bicepAtLimit = true;
      }
      stepperBicep.setCurrentPosition((long)MAX_STEPS);
      stepsBicep = MAX_STEPS;
      stepperBicep.setSpeed(0);
    } else if (p <= 0) {
      stepperBicep.setCurrentPosition(0);
      stepsBicep = 0;
    }
  }

  p = stepperShoulder.currentPosition();
  if (p != lastPosShoulder) {
    lastPosShoulder = p;
    stepsShoulder = (unsigned long)abs(p);
    if (stepsShoulder >= MAX_STEPS) {
      if (!shoulderAtLimit) {
        Serial.println("Shoulder reached step limit (20000) and is now disabled.");
        shoulderAtLimit = true;
      }
      stepperShoulder.setCurrentPosition((long)MAX_STEPS);
      stepsShoulder = MAX_STEPS;
      stepperShoulder.setSpeed(0);
    } else if (p <= 0) {
      stepperShoulder.setCurrentPosition(0);
      stepsShoulder = 0;
    }
  }
}