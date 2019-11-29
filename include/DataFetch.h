#include <Arduino.h>
// Include firebase library
#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>
#include "FirebaseCred.h"

String readingData(String stallNumber) {
  // Reading stall status
  String stallStatus = Firebase.getString("stalls/" + stallNumber + "/status/");
  Serial.println("The status of " + stallNumber + " is " + stallStatus);
  return stallStatus;
}

void writingData(String stallNumber, String status) {
  // Writing stall status
  Firebase.setString("stalls/" + stallNumber + "/status/", status);
  Serial.println("Updating " + stallNumber + " to " + status);
}
