#include "ParkingProcess.h"

void setup() {
  // Ultrasonic sensors
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input

  // Shift register
  pinMode(clockPin, OUTPUT);  // Sets the cloPin as an Output
  pinMode(dataPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(latchPin, OUTPUT);  // Sets the trigPin as an Output

  delay(1000);
  Serial.begin(9600);  // Baud rate 9600 or 115200, depending on the firmware

  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("success!");
  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP());

  // Configure time to meet PDT Timezone
  configTime(-7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }

  // Firebase initialization
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // Check WiFi Status
  getTime();

  if (WiFi.status() == WL_CONNECTED) {
    sensorLedInterfaces();
  } else if (WiFi.status() == WL_CONNECTION_LOST || WiFi.status() == WL_DISCONNECTED) {
    Serial.println(" WiFi connection is lost, reconnecting");
    ESP.restart();
  }

  delay(100);  // Fetching data from firebase every 0.1 seconds
}
