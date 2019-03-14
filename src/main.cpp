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

// defines pins numbers
// for ultrasonic sensors
const uint8_t echoPin1 = 0;  // D3
const uint8_t echoPin2 = 5;  // D1
const uint8_t echoPin3 = 2;  // D4
const uint8_t echoPin4 = 4;  // D2

// for Led shift register
uint8_t clockPin = 14;  // D5
uint8_t dataPin = 13;   // D7
uint8_t latchPin = 12;  // D6

// defines variables
uint8_t minDistance = 0;  // minimum sensing distance
uint8_t maxDistance = 5;  // maximum sensing distance

int distance[4];
String databaseLed[] = {"stallA1", "stallA2", "stallB1", "stallB2"};

uint8_t ledPattern = 0x00;

uint8_t trigPin1 = 0x10;  // pin 4 on shift register
uint8_t trigPin2 = 0x20;  // pin 5 on shift register
uint8_t trigPin3 = 0x40;  // pin 6 on shift register
uint8_t trigPin4 = 0x80;  // pin 7 on shift register

void setup() {
  // Ultrasonic sensors
  pinMode(echoPin1, INPUT);  // Sets the echoPin as an Input
  pinMode(echoPin2, INPUT);  // Sets the echoPin as an Input
  pinMode(echoPin3, INPUT);  // Sets the echoPin as an Input
  pinMode(echoPin4, INPUT);  // Sets the echoPin as an Input

  // Shift register
  pinMode(clockPin, OUTPUT);  // Sets the cloPin as an Output
  pinMode(dataPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(latchPin, OUTPUT);  // Sets the trigPin as an Output

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
  String stallStatus = Firebase.getString("stalls/" + stallNumber + "/status/");
  Serial.println("The status of " + stallNumber + "is " + stallStatus);
  return stallStatus;
}

void writingData(String stallNumber, String status) {
  // Writing stall status
  Firebase.setString("stalls/" + stallNumber + "/status/", status);
  Serial.print("Updating " + stallNumber + " to " + status);
}

void shiftOutData(int dataPattern) {
  // function to output data pattern from shift register
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, dataPattern);
  digitalWrite(latchPin, HIGH);
}

int ultraSensing(int trigPin, int echoPin) {
  // function to calculate distance from ultrasonic sensor's reading
  // Clears the trigPin
  shiftOutData(ledPattern);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  shiftOutData(ledPattern + trigPin);
  delayMicroseconds(10);
  shiftOutData(ledPattern);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  uint32_t duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  int distance = duration * 0.034 / 2;
  return distance;
}

uint8_t turnLedRed(int distance, int n) {
  // function to turn led colour to red
  uint8_t ledRed = 0x00;
  String status = readingData(databaseLed[n]);
  if (distance > minDistance && distance < maxDistance) {
    ledRed = pow(2, n);  // turn led to red
    if (status == "vacant") {
      writingData(databaseLed[n], "occupied");
    }
  } else if (distance > maxDistance && status == "occupied") {
    writingData(databaseLed[n], "vacant");
  }
  return ledRed;
}

void sensorLedInterfaces() {
  // read data from ultrasonic sensors
  distance[0] = ultraSensing(trigPin1, echoPin1);
  distance[1] = ultraSensing(trigPin2, echoPin2);
  distance[2] = ultraSensing(trigPin3, echoPin3);
  distance[3] = ultraSensing(trigPin4, echoPin4);

  // Prints the distance on the Serial Monitor
  Serial.print("Distance1: ");
  Serial.println(distance[0]);
  Serial.print("Distance2: ");
  Serial.println(distance[1]);
  Serial.print("Distance3: ");
  Serial.println(distance[2]);
  Serial.print("Distance4: ");
  Serial.println(distance[3]);

  // setup led pattern from led to change colour
  ledPattern = 0x00;
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t led = turnLedRed(distance[i], i);
    ledPattern = ledPattern + led;
  }

  // send led pattern to shift register
  shiftOutData(ledPattern);
}

void loop() {
  // Check WiFi Status
  if (WiFi.status() == WL_CONNECTED) {
    sensorLedInterfaces();
  }

  delay(100);  // Fetching data from firebase every 0.1 seconds
}
