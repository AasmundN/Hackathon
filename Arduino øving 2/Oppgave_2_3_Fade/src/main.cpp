#include <Arduino.h>
#define PWM_MAX_RAW_VAL 255
// Declare the PWM pin which will control the fade
const int ledPwmPin = 6;

void setup() {
  pinMode(ledPwmPin, OUTPUT);
}

void loop() {
  //Increasing fade. Increase delay value to make it last longer.
  for(int i = 0; i <= PWM_MAX_RAW_VAL; i++){
    analogWrite(ledPwmPin, i);
    delay(10);
  }
  //Decreasing fade. Increase delay value to make it last longer.
  for(int i = PWM_MAX_RAW_VAL; i >= 0; i--){
    analogWrite(ledPwmPin, i);
    delay(10);
  }

  // Fade becomes faster if you increment/decrement larger values from iterator (i += 5 instead of i++).
}