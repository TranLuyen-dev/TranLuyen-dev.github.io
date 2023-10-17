#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <HttpsOTAUpdate.h>

const char *ssid     = "Tenda RD";
const char *password = "khongcopass";

#define FW_VERSION 1.5

String new_url     = "";
float  new_version = FW_VERSION;

// t_httpUpdate_return updateOverHttp(String url_update) {
// 	t_httpUpdate_return ret;

// 	if ((WiFi.status() == WL_CONNECTED)) {

// 		ret = ESPhttpUpdate.update(url_update);

// 		switch (ret) {
// 			case HTTP_UPDATE_FAILED:
// 				Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
// 				return ret;
// 				break;

// 			case HTTP_UPDATE_NO_UPDATES:
// 				Serial.println("HTTP_UPDATE_NO_UPDATES");
// 				return ret;
// 				break;

// 			case HTTP_UPDATE_OK:
// 				Serial.println("HTTP_UPDATE_OK");
// 				return ret;
// 				break;
// 		}
// 	}
// }

static const char *server_certificate = "-----BEGIN CERTIFICATE-----\n"
                                        "MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/\n"
                                        "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n"
                                        "DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow\n"
                                        "SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT\n"
                                        "GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC\n"
                                        "AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF\n"
                                        "q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8\n"
                                        "SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0\n"
                                        "Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA\n"
                                        "a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj\n"
                                        "/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T\n"
                                        "AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG\n"
                                        "CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv\n"
                                        "bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k\n"
                                        "c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw\n"
                                        "VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC\n"
                                        "ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz\n"
                                        "MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu\n"
                                        "Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF\n"
                                        "AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo\n"
                                        "uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/\n"
                                        "wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu\n"
                                        "X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG\n"
                                        "PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6\n"
                                        "KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==\n"
                                        "-----END CERTIFICATE-----";

static HttpsOTAStatus_t otastatus;

void HttpEvent(HttpEvent_t *event) {
	switch (event->event_id) {
		case HTTP_EVENT_ERROR: Serial.println("Http Event Error"); break;
		case HTTP_EVENT_ON_CONNECTED: Serial.println("Http Event On Connected"); break;
		case HTTP_EVENT_HEADER_SENT: Serial.println("Http Event Header Sent"); break;
		case HTTP_EVENT_ON_HEADER: Serial.printf("Http Event On Header, key=%s, value=%s\n", event->header_key, event->header_value); break;
		case HTTP_EVENT_ON_DATA: break;
		case HTTP_EVENT_ON_FINISH: Serial.println("Http Event On Finish"); break;
		case HTTP_EVENT_DISCONNECTED: Serial.println("Http Event Disconnected"); break;
		// case HTTP_EVENT_REDIRECT: Serial.println("Http Event Redirect"); break;
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

	HttpsOTA.onHttpEvent(HttpEvent);
	Serial.println("Starting OTA");

	Serial.print("FW_VERSION: ");
	Serial.println(FW_VERSION);
}

void loop() {
	if (Serial.available()) {
		String s = Serial.readString();
		if (s.indexOf("get") != -1) { get_version(); }
		else if (s.indexOf("update") != -1) {
			if (new_version > FW_VERSION) {
				Serial.println("Update Available");
				// if (updateOverHttp(new_url) == HTTP_UPDATE_OK) { Serial.println("Update Success"); }
				// else { Serial.println("Update Failed"); }
				// Serial.println("Update Success");
				HttpsOTA.begin(new_url.c_str(), server_certificate);
				if (otastatus == HTTPS_OTA_SUCCESS) {
					Serial.println("Firmware written successfully. To reboot device, call API ESP.restart() or PUSH restart button on device");
				}
				else if (otastatus == HTTPS_OTA_FAIL) { Serial.println("Firmware Upgrade Fail"); }
			}
			else { Serial.println("No Update Available"); }
		}
		else {
			int idx = s.indexOf("url=");
			if (idx != -1) {
				new_url = s.substring(idx + 4, s.length());
				Serial.print("new url: ");
				Serial.println(new_url);
			}
		}
	}
}