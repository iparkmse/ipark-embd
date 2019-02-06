#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// Replace these with your WiFi network settings
const char* ssid = "Tom_Asus"; //replace this with your WiFi network name
const char* password = "12345678"; //replace this with your WiFi network password

void setup()
{
  delay(1000);
  Serial.begin(9600); // baud rate 9600 or 115200, depending on the firmware
 
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("success!");
  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check WiFi Status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;  //Object of class HTTPClient
    http.begin("http://jsonplaceholder.typicode.com/users/1");
    int httpCode = http.GET();
    //Check the returning code                                                                  
    if (httpCode > 0) {
      // Parsing
      const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
      // Parameters
      int id = root["id"]; // 1
      const char* name = root["name"]; // "Leanne Graham"
      const char* username = root["username"]; // "Bret"
      const char* email = root["email"]; // "Sincere@april.biz"
      // Output to serial monitor
      Serial.print("Name:");
      Serial.println(name);
      Serial.print("Username:");
      Serial.println(username);
      Serial.print("Email:"); 
      Serial.println(email);
    }
    http.end();   //Close connection
  }
  // Delay
  delay(60000);
}
