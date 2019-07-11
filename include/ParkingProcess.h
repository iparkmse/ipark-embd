#include "DataFetch.h"
#include "ExpirationCheck.h"


// defines pins numbers
// for ultrasonic sensors
const uint8_t echoPin = 12;  // D6

// for Led shift register
uint8_t clockPin = 16;  // D0
uint8_t dataPin = 4;   // D2
uint8_t latchPin = 5;  // D1

// defines variables
uint8_t minDistance = 0;  // minimum sensing distance
uint8_t maxDistance = 5;  // maximum sensing distance

String databaseLed[] = {"stallA1", "stallA2", "stallA3", "stallA4", "stallA5",
                        "stallB1", "stallB2", "stallB3", "stallB4", "stallB5"};
uint16_t distance[numberOfStall];  // sensing distance for each stall

uint32_t ledPattern = 0x00;  // occupied
uint32_t ledPattern2 = 0x00;  // violated
uint32_t ledPattern3 = 0x00;  // expiring
uint8_t count = 0;

bool checkReservation (String reserveStall) {

  // Return 1 if true, 0 if false
  String setName = setResvStall(reserveStall);
  Serial.println(setName);
  bool readResvData = false;

  if (setName == "nothing"){
      Serial.println("not checking firebase as the time is out of schedule...");
  }
  else {
      readResvData = resValidation(setName);
        }
  return readResvData;
}

void shiftOutData(uint32_t dataPattern) {
  // function to output data pattern from shift register
  uint16_t dataPatterna = dataPattern>>16;
  uint16_t dataPatternb = dataPattern;
  uint8_t dataPattern1 = dataPatterna>>8;
  uint8_t dataPattern2 = dataPatterna; 
  uint8_t dataPattern3 = dataPatternb>>8;
  uint8_t dataPattern4 = dataPatternb; 

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, dataPattern1);
  shiftOut(dataPin, clockPin, MSBFIRST, dataPattern2);
  shiftOut(dataPin, clockPin, MSBFIRST, dataPattern3);
  shiftOut(dataPin, clockPin, MSBFIRST, dataPattern4);
  digitalWrite(latchPin, HIGH);
}

int ultraSensing(uint16_t trigPin) {
  // function to calculate distance from ultrasonic sensor's reading
  // Clears the trigPin
  shiftOutData(ledPattern);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  shiftOutData(ledPattern + trigPin);
  delayMicroseconds(10);
  shiftOutData(ledPattern);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  int distance = duration * 0.034 / 2;
  return distance;
}

uint16_t turnLedRed(int distance, int n, String status) {
  // function to turn led colour to red and set status
  uint16_t ledRed = 0x00;
  if (distance > minDistance && distance < maxDistance) {
    ledRed = (long)(1)<<(n);  // turn led to red
    if (status == "vacant") {
      writingData(databaseLed[n], "occupied");
      setExpiration(n);
    }
    else if (status == "reserved") {
      if (checkReservation(databaseLed[n]) == true) {
        writingData(databaseLed[n], "occupied");
        setExpiration(n);
      }
      else if (checkReservation(databaseLed[n]) == false){
        writingData(databaseLed[n], "violated");
      }
    }
    else if (status == "occupied") {
      uint8_t expiration = checkExpiration(n);
      if (expiration == 1) {
        writingData(databaseLed[n], "violated");
      }
      else if (expiration == 2) {
        writingData(databaseLed[n], "expiring");
      }
      else if (n <= 3 && checkReservation(databaseLed[n]) == false) {
        writingData(databaseLed[n], "violated");
      }
    }
  } else if (distance > maxDistance && status == "occupied") {
    writingData(databaseLed[n], "vacant");
  }
  return ledRed;
}

uint16_t forViolation(int distance, int n, String status) {
  // function to blink led and set status
  uint16_t ledViolation = 0x00;
 if (distance > minDistance && distance < maxDistance) {
   if (status == "violated") {
      ledViolation = (long)(1)<<(n);  // blink led
      if (n <= 3 && checkReservation(databaseLed[n]) == true) {
        writingData(databaseLed[n], "occupied");
        setExpiration(n);
      }
   }
 } else if (distance > maxDistance && status == "violated") {
   writingData(databaseLed[n], "vacant");
 }
  return ledViolation;
}

uint16_t forExpiring(int distance, int n, String status) {
  // function to blink led and set status
  uint16_t ledExpiring = 0x00;
 if (distance > minDistance && distance < maxDistance) {
   if (status == "expiring") {
      ledExpiring = (long)(1)<<(n);  // blink led
      if (checkExpiration(n) == 1) {
        writingData(databaseLed[n], "violated");
      }
      else if (n <= 3 && checkReservation(databaseLed[n]) == false) {
          writingData(databaseLed[n], "violated");
      }
   }
 } else if (distance > maxDistance && status == "expiring") {
   writingData(databaseLed[n], "vacant");
 }
  return ledExpiring;
}

void sensorLedInterfaces() {
  // read data from ultrasonic sensors
  for (uint8_t i = 0; i < numberOfStall; i++) {
    // read data from ultrasonic sensors
    uint16_t trigger = (long)(1)<<(i);
    distance[i] = ultraSensing(trigger);

    // Prints the distance on the Serial Monitor
    Serial.print("Distance");
    Serial.print(i+1);
    Serial.print(": ");
    Serial.println(distance[i]);
  }
  
  // setup led pattern from led to change colour
  uint32_t red = 0x00;
  uint32_t green = 0x00;
  uint32_t violation = 0x00;
  uint32_t expiring = 0x00;
  for (uint8_t i = 0; i < numberOfStall; i++) {
    String status = readingData(databaseLed[i]);
    red = red + turnLedRed(distance[i], i, status);
    violation = violation + forViolation(distance[i], i, status);
    expiring = expiring + forExpiring(distance[i], i, status);
  }
  green = ~ red;
  violation = red & (red ^ (violation ^ green));
  expiring = red & (red ^ (expiring ^ green));
  ledPattern = (red << 10) + (green << 20);
  ledPattern2 = (violation << 10) + (green << 20);
  ledPattern3 = (expiring << 10) + (green << 20);

  // send led pattern to shift register
  count += 1;
  if (count == 3) {
    shiftOutData(ledPattern3);
    delay(50);
    count = 0;
  }

  // for (uint8_t i = 0; i < 5  ; i++) {
    shiftOutData(ledPattern2);
    delay(100);
    shiftOutData(ledPattern);
    delay(10);
  // }
}
