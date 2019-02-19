#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

//Include firebase library
#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

#define FIREBASE_HOST "my-test1-0123.firebaseio.com"
#define FIREBASE_AUTH "Lzzd3o6bTR0o59CtImDzm4MAULRJsnWwNlpzGDqu" //contains secret, needs not to be hardcoded in the future

// Replace these with your WiFi network settings
const char* ssid = "Firda's iPhone"; //replace this with WiFi network name
const char* password = "firda0303"; //replace this with WiFi network password

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

  //Firebase initialization
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
}

void loop() {
  // Check WiFi Status
  if (WiFi.status() == WL_CONNECTED) {
    // get value
    Serial.print("Stall 1 status:");
    String stall_status1 = Firebase.getString("stall1/status/");
    Serial.println(stall_status1);

    if (stall_status1="available"){
      Serial.println("Available for reservation");
      Firebase.setString("stall1/reservation/","yes");
    }
    else{
      Serial.println("Stall is busy");
      Firebase.setString("stall1/reservation/","no");
    }

    Serial.print("Stall 2 status:");
    String stall_status2 = Firebase.getString("stall2/status/");
    Serial.println(stall_status2);

    if (stall_status2="available"){
      Serial.print("Available for reservation");
      Firebase.setString("stall2/reservation/","yes");
    }
    else{
      Serial.print("Stall is busy");
      Firebase.setString("stall2/reservation/","no");
    }

  }
  // Delay
  delay(3000); //fetching data from firebase every 3 seconds
}
