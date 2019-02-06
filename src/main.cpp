#include <Arduino.h>

#include <ESP8266WiFi.h>

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

}
