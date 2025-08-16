#include <SoftwareSerial.h>

SoftwareSerial BT(2, 3);  // RX, TX (Arduino pins)
// --- Pin Definitions ---
const int trigPin = 9;
const int echoPin = 10;
const int buzzer = 11;
const int ledPin = 4;          // LED for rain only
const int rainSensorPin = A3;  // Rain sensor analog pin

// --- Variables ---
long duration;
int distance;
int safetyDistance;
int rainValue;
int rainThreshold = 300;  // Rain sensor threshold


int buttonPin = A0;
int buttonState = 0;

void setup() {
  BT.begin(9600);                    
  pinMode(buttonPin, INPUT_PULLUP); 
  BT.println("Bluetooth ready. Press button to send message.");


  Serial.begin(9600);        // Serial monitor
  pinMode(trigPin, OUTPUT);  // Ultrasonic trigger
  pinMode(echoPin, INPUT);   // Ultrasonic echo
  pinMode(buzzer, OUTPUT);   // Buzzer
  pinMode(ledPin, OUTPUT);   // LED (only for rain)
}

void loop() {
  buttonState = digitalRead(buttonPin);

  
  if (buttonState == LOW) {
    BT.println("Help Me, Im In Danger!!!");
    delay(500);  // debounce & avoid spamming
  }
  
  rainValue = analogRead(rainSensorPin);
  Serial.print("Rain Value : ");
  Serial.println(rainValue);

  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  safetyDistance = distance;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (rainValue < rainThreshold) {
    digitalWrite(ledPin, HIGH);
    Serial.println("Heavy rain detected - LED ON");
  } else {
    digitalWrite(ledPin, LOW);
  }


  if (rainValue < rainThreshold || safetyDistance <= 30) {
    digitalWrite(buzzer, HIGH);
    if (safetyDistance <= 5) Serial.println("Obstacle ahead!");
  } else {
    digitalWrite(buzzer, LOW);
  }

  delay(200);
}
