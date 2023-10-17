# 1 "C:\\Users\\Tran_Luyen\\Documents\\delete\\GitOTA\\TranLuyen-dev.github.io\\ESP32\\ESP32.ino"
# 2 "C:\\Users\\Tran_Luyen\\Documents\\delete\\GitOTA\\TranLuyen-dev.github.io\\ESP32\\ESP32.ino" 2
# 3 "C:\\Users\\Tran_Luyen\\Documents\\delete\\GitOTA\\TranLuyen-dev.github.io\\ESP32\\ESP32.ino" 2
# 4 "C:\\Users\\Tran_Luyen\\Documents\\delete\\GitOTA\\TranLuyen-dev.github.io\\ESP32\\ESP32.ino" 2
# 5 "C:\\Users\\Tran_Luyen\\Documents\\delete\\GitOTA\\TranLuyen-dev.github.io\\ESP32\\ESP32.ino" 2

const char *ssid = "Tenda RD";
const char *password = "khongcopass";



String url_update = "";
float new_version = 1.1;

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
 String url_ver = "https://tranluyen-dev.github.io/info.json";
 HTTPClient http;
 String response;

 http.begin(url_ver);
 http.GET();

 response = http.getString();
 Serial.println(response);

 StaticJsonDocument<1024> doc;
 deserializeJson(doc, response);
 JsonObject doc_obj = doc.as<JsonObject>();
 url_update = doc_obj["url"].as<String>();
 new_version = doc_obj["ver"].as<float>();

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
 WiFi.mode(WIFI_MODE_STA);
 WiFi.begin(ssid, password);

 if (WiFi.waitForConnectResult() != WL_CONNECTED) {
  Serial.println("WiFi Failed");
  while (1) {
   delay(1000);
  }
 }
 Serial.println("WiFi connected");

 Serial.print("FW_VERSION: ");
 Serial.println(1.0);
}

void loop() {
 if (Serial.available()) {
  String s = Serial.readString();
  if (s.indexOf("get") != -1) { get_version(); }
  else if (s.indexOf("update") != -1) {
   if (new_version > 1.0) {
    Serial.println("Update Available");
    if (updateOverHttp(url_update) == HTTP_UPDATE_OK) { Serial.println("Update Success"); }
    else { Serial.println("Update Failed"); }
    Serial.println("Update Success");
   }
   else { Serial.println("No Update Available"); }
  }
 }
}
