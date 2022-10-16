#include <Arduino.h>
#define PWM_MAX_RAW_VAL 255
#define ADC_MAX_VAL 1023

// Define the potentiometer measurement pin and the LED pin.
const int potPin = A1;
const int ledPin = 6;

// Define a read value from the potentiometer (0-1023) and a converted value for the PWM (0 - 255)
int potVal;
int ledVal;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  potVal = analogRead(potPin);
  Serial.println(potVal);
  ledVal = map(potVal, 0, ADC_MAX_VAL, 0, PWM_MAX_RAW_VAL);
  analogWrite(ledPin, ledVal);
  delay(10);
}