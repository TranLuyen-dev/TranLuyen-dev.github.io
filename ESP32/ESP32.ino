#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>

WiFiMulti WiFiMulti;

const char *ssid     = "Tenda RD";
const char *password = "khongcopass";

#define FW_VERSION 1.0

String url_update  = "";
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
	new_version        = doc_obj["ver"].as<float>();

	Serial.print("url_update: ");
	Serial.println(url_update);
	Serial.print("new version: ");
	Serial.println(new_version);
	// serializeJsonPretty(obj, Serial);
}

void update_started() { Serial.println("CALLBACK:  HTTP update process started"); }

void update_finished() { Serial.println("CALLBACK:  HTTP update process finished"); }

void update_progress(int cur, int total) { Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total); }

void update_error(int err) { Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err); }

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

	WiFiMulti.addAP(ssid, password);
}

void loop() {
	if (Serial.available()) {
		String s = Serial.readString();
		if (s.indexOf("get") != -1) { get_version(); }
		else if (s.indexOf("update") != -1) {
			WiFiClient client;
			httpUpdate.onStart(update_started);
			httpUpdate.onEnd(update_finished);
			httpUpdate.onProgress(update_progress);
			httpUpdate.onError(update_error);
			t_httpUpdate_return ret = httpUpdate.update(client, url_update);
			switch (ret) {
				case HTTP_UPDATE_FAILED:
					Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
					break;

				case HTTP_UPDATE_NO_UPDATES: Serial.println("HTTP_UPDATE_NO_UPDATES"); break;

				case HTTP_UPDATE_OK: Serial.println("HTTP_UPDATE_OK"); break;
			}
		}
	}
}