// #pragma once
// #include <Arduino.h>

// typedef void (*rxtx_recv_cb_t)(const uint8_t *mac, const uint8_t *data, int len);

// // Initialize ESP-NOW for RX/TX. Provide a receive callback (can be nullptr).
// void rxtx_init(rxtx_recv_cb_t cb);

// // Send data to a peer MAC (6 bytes). If peerMac is nullptr, broadcast will be used.
// // Returns ESP_OK on success.
// esp_err_t rxtx_send(const uint8_t *peerMac, const uint8_t *data, size_t len);
