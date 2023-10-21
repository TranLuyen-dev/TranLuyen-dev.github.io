#include "ESP_OTA.h"

bool esp_ota_init() {

	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		return false;
	}

	// Port defaults to 3232
	ArduinoOTA.setPort(3232);

	// Hostname defaults to esp3232-[MAC]
	// ArduinoOTA.setHostname("myesp32");

	// No authentication by default
	// ArduinoOTA.setPassword("admin");

	// Password can be set with it's md5 value as well
	// MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
	// ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

	ArduinoOTA
	    .onStart([]() {
		    String type;
		    if (ArduinoOTA.getCommand() == U_FLASH) type = "sketch";
		    else // U_SPIFFS
			    type = "filesystem";

		    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
		    ota_debug("Start updating %s \r\n", type.c_str());
	    })
	    .onEnd([]() { ota_debug("\nEnd\r\n"); })
	    .onProgress([](unsigned int progress, unsigned int total) {
#ifdef ota_debug_enabled
		    uint8_t percent = progress / (total / 100);
		    if (percent != ota_progress) {
			    ota_progress = percent;
			    ota_debug("Progress: %u%%\r\n", (progress / (total / 100)));
		    }
#endif
	    })
	    .onError([](ota_error_t error) {
		    ota_debug("Error[%u]: ", error);
		    if (error == OTA_AUTH_ERROR) ota_debug("Auth Failed\r\n");
		    else if (error == OTA_BEGIN_ERROR)
			    ota_debug("Begin Failed\r\n");
		    else if (error == OTA_CONNECT_ERROR)
			    ota_debug("Connect Failed\r\n");
		    else if (error == OTA_RECEIVE_ERROR)
			    ota_debug("Receive Failed\r\n");
		    else if (error == OTA_END_ERROR)
			    ota_debug("End Failed\r\n");
	    });

	ArduinoOTA.begin();

	ota_debug("Ready\r\n");
	ota_debug("IP address: %s", WiFi.localIP().toString().c_str());
	return true;
}

void esp_ota_handle(void) { ArduinoOTA.handle(); }