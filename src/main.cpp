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

#define FIREBASE_HOST "ipark-2997b.firebaseio.com"
#define FIREBASE_AUTH "xxx" //contains secret, needs not to be hardcoded in the future

// Replace these with your WiFi network settings
const char* ssid = "xxx"; //replace this with WiFi network name
const char* password = "xxx"; //replace this with WiFi network password

void setup(){
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
    //Jack's code should reside inside here while using the function readingData or writingData.

  }
  // Delay
  delay(3000); //fetching data from firebase every 3 seconds
}

void readingData(String stallNumber){
  // Reading stall status
  String stall_status = Firebase.getString(stallNumber + "/status/");
  Serial.println("Stall status is " + stall_status);

}

void writingData(String stallNumber , String status){
  // Writing stall status
  if (status="occupied"){
    Serial.print("Available for reservation ");
    Firebase.setString("stall/"+stallNumber+"/status/",status);
  }
  else if(status="vacant"){
    Serial.print("Stall number "+stallNumber);
    Firebase.setString("stall/"+stallNumber+"/status/",status);
  }

}
