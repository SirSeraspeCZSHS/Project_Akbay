#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Button connected to GPIO19 (to GND), use internal pull-up
constexpr int BUTTON_PIN = 19;

Adafruit_MPU6050 mpu;

// debounce
const unsigned long DEBOUNCE_MS = 50UL;
int lastRawBtn = HIGH;
int stableBtn = HIGH;
unsigned long lastDebounceBtn = 0;

void setup() {
	Serial.begin(115200);
	while (!Serial) delay(10);

	pinMode(BUTTON_PIN, INPUT_PULLUP);

	Wire.begin();
	if (!mpu.begin()) {
		Serial.println("Failed to find MPU6050 chip");
		while (1) { delay(1000); }
	}

	mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
	mpu.setGyroRange(MPU6050_RANGE_500_DEG);
	mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

	Serial.println("MPU6050 found and configured");
}

void loop() {
	sensors_event_t a, g, temp;
	mpu.getEvent(&a, &g, &temp);

	// debounce button
	int raw = digitalRead(BUTTON_PIN);
	if (raw != lastRawBtn) lastDebounceBtn = millis();
	if (millis() - lastDebounceBtn > DEBOUNCE_MS) {
		if (raw != stableBtn) {
			stableBtn = raw;
			Serial.print("Button ");
			Serial.println(stableBtn == LOW ? "PRESSED" : "RELEASED");
		}
	}
	lastRawBtn = raw;

	// Print sensor values
	// Acceleration in m/s^2 (Adafruit returns in m/s^2), gyro in rad/s
	Serial.print("ACC: "); Serial.print(a.acceleration.x); Serial.print(","); Serial.print(a.acceleration.y); Serial.print(","); Serial.print(a.acceleration.z);
	Serial.print("  GYRO: "); Serial.print(g.gyro.x); Serial.print(","); Serial.print(g.gyro.y); Serial.print(","); Serial.print(g.gyro.z);
	Serial.print("  TEMP_C: "); Serial.print(temp.temperature);
	Serial.print("  BTN: "); Serial.println(stableBtn == LOW ? "DOWN" : "UP");

	delay(200);
}

