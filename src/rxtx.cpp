#include "rxtx.h"
#include <WiFi.h>
#include <esp_now.h>

static rxtx_recv_cb_t g_recv_cb = nullptr;

static void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
	Serial.print("Received from: ");
	for (int i = 0; i < 6; i++) {
		if (i) Serial.print(":");
		if (mac[i] < 16) Serial.print('0');
		Serial.print(mac[i], HEX);
	}
	Serial.print("  len="); Serial.println(len);

	// Print payload as string if printable
	String s;
	for (int i = 0; i < len; ++i) s += (char)incomingData[i];
	Serial.print("Payload: "); Serial.println(s);

	if (g_recv_cb) g_recv_cb(mac, incomingData, len);
}

static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
	Serial.print("Send status to ");
	for (int i = 0; i < 6; i++) {
		if (i) Serial.print(":");
		if (mac_addr[i] < 16) Serial.print('0');
		Serial.print(mac_addr[i], HEX);
	}
	Serial.print(" -> ");
	Serial.println(status == ESP_NOW_SEND_SUCCESS ? "OK" : "FAIL");
}

void rxtx_init(rxtx_recv_cb_t cb) {
	g_recv_cb = cb;

	WiFi.mode(WIFI_STA);
	WiFi.disconnect();

	if (esp_now_init() != ESP_OK) {
		Serial.println("Error initializing ESP-NOW");
		return;
	}

	esp_now_register_recv_cb(onDataRecv);
	esp_now_register_send_cb(onDataSent);

	Serial.println("ESP-NOW init done (rxtx)");
}

esp_err_t rxtx_send(const uint8_t *peerMac, const uint8_t *data, size_t len) {
	// If user passes nullptr, use broadcast address
	uint8_t broadcast_mac[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	const uint8_t *target = peerMac ? peerMac : broadcast_mac;

	return esp_now_send(target, data, (size_t)len);
}

