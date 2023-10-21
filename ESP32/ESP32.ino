#include <WiFi.h>

#include "ESP_OTA.h"

const char *ssid     = "Tenda RD";
const char *password = "khongcopass";

void setup() {
	Serial.begin(115200);
	Serial.println("wifi connecting...");
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.println("Connection Failed! Rebooting...");
		// delay(5000);
		// ESP.restart();
	}
	esp_ota_init();
}

void loop() {
	esp_ota_handle();
}