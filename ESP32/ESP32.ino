#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>

const char *ssid     = "Tenda RD";
const char *password = "khongcopass";

#define FW_VERSION 1.5

String new_url     = "";
float  new_version = FW_VERSION;

TaskHandle_t task0;

t_httpUpdate_return updateOverHttp(String url_update) {
	t_httpUpdate_return ret;

	if ((WiFi.status() == WL_CONNECTED)) {

		ret = ESPhttpUpdate.update(url_update);

		switch (ret) {
			case HTTP_UPDATE_FAILED:
				Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
				return ret;
				break;

			case HTTP_UPDATE_NO_UPDATES:
				Serial.println("HTTP_UPDATE_NO_UPDATES");
				return ret;
				break;

			case HTTP_UPDATE_OK:
				Serial.println("HTTP_UPDATE_OK");
				return ret;
				break;
		}
	}
}

void get_version(void) {
	String     url_ver = "https://tranluyen-dev.github.io/info.json";
	HTTPClient http;
	String     response;

	http.begin(url_ver);
	http.GET();

	response = http.getString();
	Serial.println(response);

	StaticJsonDocument<1024> doc;
	deserializeJson(doc, response);
	JsonObject doc_obj = doc.as<JsonObject>();
	new_url            = doc_obj["url"].as<String>();
	new_version        = doc_obj["ver"].as<float>();

	Serial.print("new_url: ");
	Serial.println(new_url);
	Serial.print("new version: ");
	Serial.println(new_version);
	// serializeJsonPretty(obj, Serial);
}

void checkUpdate() {
	Serial.println("Checking update");
	HTTPClient http;
	String     response;
	String     url = "https://tranluyen-dev.github.io/info.json";
	http.begin(url);
	http.GET();
	response = http.getString();
	Serial.println(response);
	StaticJsonDocument<1024> doc;
	deserializeJson(doc, response);
	JsonObject doc_obj = doc.as<JsonObject>();
	new_url            = doc_obj["url"].as<String>();
	new_version        = doc_obj["ver"].as<float>();
	Serial.print("new_url: ");
	Serial.println(new_url);
	Serial.print("new version: ");
	Serial.println(new_version);

	Serial.println("Update Available");
	if (updateOverHttp(new_url) == HTTP_UPDATE_OK) { Serial.println("Update Success"); }
	else { Serial.println("Update Failed"); }
	Serial.println("Update Success");
}

void update_started() { Serial.println("CALLBACK:  HTTP update process started"); }

void update_finished() { Serial.println("CALLBACK:  HTTP update process finished"); }

void update_progress(int cur, int total) { Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total); }

void update_error(int err) { Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err); }

void ledBlink(void *pvParameter) {
	for (;;) {
		// Your Code
		digitalWrite(LED_BUILTIN, HIGH);
		delay(500);
		digitalWrite(LED_BUILTIN, LOW);
		delay(500);
	}
}

void setup() {
	Serial.begin(115200);
	Serial.setTimeout(10);
	delay(10);
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	if (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.println("WiFi Failed");
		while (1) {
			delay(1000);
		}
	}
	Serial.println("WiFi connected");
	Serial.print("current version: ");
	Serial.println(FW_VERSION);
	xTaskCreatePinnedToCore(ledBlink, "ledBlink", 10000, NULL, 1, &task0, 0);
}

void loop() {
	if (Serial.available()) {
		String s = Serial.readString();
		if (s.indexOf("get") != -1) { get_version(); }
		else if (s.indexOf("update") != -1) { checkUpdate(); }
	}
	vTaskDelay(5000);
}