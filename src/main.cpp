#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// Include firebase library
#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>
#include "FirebaseCred.h"

void setup() {
  delay(1000);
  Serial.begin(9600);  // Baud rate 9600 or 115200, depending on the firmware

  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("success!");
  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP());

  // Firebase initialization
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

String readingData(String stallNumber) {
  // Reading stall status
  String stall_status = Firebase.getString("stalls/"+ stallNumber + "/status/");
  return stall_status;
}

void writingData(String stallNumber, String status) {
  // Writing stall status
  if (status == "occupied") {
    Serial.print("Available for reservation ");
    Firebase.setString("stalls/"+stallNumber+"/status/", status);
  } else if (status == "vacant") {
    Serial.print("Stall number "+stallNumber);
    Firebase.setString("stalls/"+stallNumber+"/status/", status);
  }
}

void loop() {
  // Check WiFi Status
  if (WiFi.status() == WL_CONNECTED) {
    String status = readingData("stallA1");
    Serial.println("Stall 1 status: " + status);
    // Jack's code should reside inside here while using the function.
  }
  // Delay
  delay(3000);  // Fetching data from firebase every 3 seconds
}
