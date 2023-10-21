#ifndef __ESP_OTA_H__
#define __ESP_OTA_H__

#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define ota_debug_enabled

#ifdef ota_debug_enabled
#define ota_debug(...) Serial.printf(__VA_ARGS__)
uint8_t ota_progress = 0;
#else
#define ota_debug(...)                                                                                                                               \
	do {                                                                                                                                             \
	} while (0)
#endif

bool esp_ota_init(void);
void esp_ota_handle(void);

#endif