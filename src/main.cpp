#include <Arduino.h>

#define LED D0

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);    // LED pin as output
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED, HIGH);// turn the LED off.(Note that LOW is the voltage level but actually 
                        //the LED is on; this is because it is acive low on the ESP8266.
  delay(1000);            // wait for 1 second.
  digitalWrite(LED, LOW); // turn the LED on.
  delay(1000); // wait for 1 second.
}