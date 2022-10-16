#include <Arduino.h>

//Define the analog sensor pin
const int ldrPin = A1;

//Define the variable to be written in Serial
int val;

void setup() {
  // Starting Serial port
  Serial.begin(9600);
  pinMode(ldrPin, INPUT);
}

void loop() {
  val = analogRead(ldrPin);
  Serial.println("New measurement!");
  Serial.print("Raw light data: ");
  Serial.println(val);
  Serial.println();
  delay(1000);
}