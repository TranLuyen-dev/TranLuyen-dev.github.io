#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
const char *ssid     = "Tenda RD";
const char *password = "khongcopass";

#define FW_VERSION 1.0

String url_update      = "";
float  new_version = 1.1;

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
	url_update         = doc_obj["url"].as<String>();
	new_version    = doc_obj["ver"].as<float>();

	Serial.print("url_update: ");
	Serial.println(url_update);
	Serial.print("new version: ");
	Serial.println(new_version);
	// serializeJsonPretty(obj, Serial);
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
}

void loop() {
	if (Serial.available()) {
		String s = Serial.readString();
		if (s.indexOf("get") != -1) { get_version(); }
	}
}