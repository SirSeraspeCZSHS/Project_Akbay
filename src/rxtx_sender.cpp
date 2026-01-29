// #include <Arduino.h>
// #include "rxtx.h"
// #include <WiFi.h>

// // Simple example sender: broadcasts a message every second.

// void setup() {
//   Serial.begin(115200);
//   delay(100);
//   rxtx_init(nullptr); // no receive callback on sender
//   Serial.println("rxtx_sender ready: broadcasting messages every second");
// }

// void loop() {
//   static uint32_t counter = 0;
//   char buf[64];
//   int n = snprintf(buf, sizeof(buf), "msg %u from sender", (unsigned)counter++);
//   esp_err_t res = rxtx_send(nullptr, (const uint8_t*)buf, (size_t)n);
//   Serial.print("send: "); Serial.print(res == ESP_OK ? "OK" : "ERR"); Serial.print(" -> "); Serial.println(buf);
//   delay(1000);
// }
