# 1 "C:\\Users\\Tran_Luyen\\Documents\\delete\\GitOTA\\TranLuyen-dev.github.io\\ESP32\\ESP32.ino"
# 2 "C:\\Users\\Tran_Luyen\\Documents\\delete\\GitOTA\\TranLuyen-dev.github.io\\ESP32\\ESP32.ino" 2

# 4 "C:\\Users\\Tran_Luyen\\Documents\\delete\\GitOTA\\TranLuyen-dev.github.io\\ESP32\\ESP32.ino" 2

const char *ssid = "Tenda RD";
const char *password = "khongcopass";

void setup() {
 Serial.begin(115200);
 Serial.println("wifi connecting...");
 WiFi.mode(WIFI_MODE_STA);
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
# 1 "C:\\Users\\Tran_Luyen\\Documents\\delete\\GitOTA\\TranLuyen-dev.github.io\\ESP32\\ESP_OTA.ino"
# 2 "C:\\Users\\Tran_Luyen\\Documents\\delete\\GitOTA\\TranLuyen-dev.github.io\\ESP32\\ESP_OTA.ino" 2

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
      if (ArduinoOTA.getCommand() == 0) type = "sketch";
      else // U_SPIFFS
       type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.printf("Start updating %s \r\n", type.c_str());
     })
     .onEnd([]() { Serial.printf("\nEnd\r\n"); })
     .onProgress([](unsigned int progress, unsigned int total) {

      uint8_t percent = progress / (total / 100);
      if (percent != ota_progress) {
       ota_progress = percent;
       Serial.printf("Progress: %u%%\r\n", (progress / (total / 100)));
      }

     })
     .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.printf("Auth Failed\r\n");
      else if (error == OTA_BEGIN_ERROR)
       Serial.printf("Begin Failed\r\n");
      else if (error == OTA_CONNECT_ERROR)
       Serial.printf("Connect Failed\r\n");
      else if (error == OTA_RECEIVE_ERROR)
       Serial.printf("Receive Failed\r\n");
      else if (error == OTA_END_ERROR)
       Serial.printf("End Failed\r\n");
     });

 ArduinoOTA.begin();

 Serial.printf("Ready\r\n");
 Serial.printf("IP address: %s", WiFi.localIP().toString().c_str());
 return true;
}

void esp_ota_handle(void) { ArduinoOTA.handle(); }
