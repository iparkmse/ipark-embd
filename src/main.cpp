// defines pins numbers
// for ultrasonic sensors
const int echoPin1 = 0;  //D3
const int echoPin2 = 5;  //D1
const int echoPin3 = 2;  //D4
const int echoPin4 = 4;  //D2

// for Led shift register
int clockPin = 14; //D5
int dataPin = 13;  //D7
int latchPin = 12; //D6
 

// defines variables
int min_distance = 0; // minimum sensing distance
int max_distance = 5; // dmaximum sensing distance

int distance[4];

uint8_t led_pattern = 0x00;

uint8_t trigPin1 = 0x10; // pin 4 on shift register
uint8_t trigPin2 = 0x20; // pin 5 on shift register
uint8_t trigPin3 = 0x40; // pin 6 on shift register
uint8_t trigPin4 = 0x80; // pin 7 on shift register

void setup() {
  // Ultrasonic sensors
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin3, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin4, INPUT); // Sets the echoPin as an Input

  // Led Shift register
  pinMode(clockPin, OUTPUT); // Sets the cloPin as an Output
  pinMode(dataPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(latchPin, OUTPUT); // Sets the trigPin as an Output
  
  Serial.begin(9600); // Starts the serial communication
}

void loop() {
  distance[0] = ultra_sensing(trigPin1, echoPin1);
  distance[1] = ultra_sensing(trigPin2, echoPin2);
  distance[2] = ultra_sensing(trigPin3, echoPin3);
  distance[3] = ultra_sensing(trigPin4, echoPin4);

  // Prints the distance on the Serial Monitor
  Serial.print("Distance1: ");
  Serial.println(distance[0]);
  Serial.print("Distance2: ");
  Serial.println(distance[1]);
  Serial.print("Distance3: ");
  Serial.println(distance[2]);
  Serial.print("Distance4: ");
  Serial.println(distance[3]);

  int i;
  led_pattern = 0x00;
  for (i = 0; i < 4; i = i + 1){
    int led = turn_led_red(distance[i], i);
    led_pattern = led_pattern + led;
  }

  shift_out_data(led_pattern);
  
  delay(500);
}

int ultra_sensing(int trigPin, int echoPin)
{
  // Clears the trigPin
//  digitalWrite(trigPin, LOW);
  shift_out_data(led_pattern);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
//  digitalWrite(trigPin, HIGH);
  shift_out_data(led_pattern+trigPin);
  delayMicroseconds(10);
//  digitalWrite(trigPin, LOW);
  shift_out_data(led_pattern);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  int distance = duration * 0.034 / 2;
  return distance;
}

void shift_out_data(int data_pattern)
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, data_pattern);
  digitalWrite(latchPin, HIGH); 
}

uint8_t turn_led_red(int distance, int n)
{
  uint8_t led_red = 0x00;
  if (distance > min_distance and distance < max_distance) {
    led_red = pow(2,n); // turn led to red 
  }
  return led_red;
}

