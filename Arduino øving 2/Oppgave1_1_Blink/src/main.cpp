#include <Arduino.h>
// Define delay time
#define DELAY 1000

// Declare LED pin.
const int LED_pin = 13;

void setup() {
  // Define LED as an output
  pinMode(LED_pin, OUTPUT);
}

void loop() {
  digitalWrite(LED_pin, HIGH);
  delay(DELAY);
  digitalWrite(LED_pin, LOW);
  delay(DELAY);
}