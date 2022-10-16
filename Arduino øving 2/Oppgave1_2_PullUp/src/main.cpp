#include <Arduino.h>
#define BOUNCE_DELAY 10

// Declare the button pin and the LED pin
const int ledPin = 13;
const int swPin = 2;

// Declare variables which save the state of the button
bool buttonState = false;

void setup() {
  // Declare the button pin as input and the LED pin as output
  pinMode(ledPin, OUTPUT);
  pinMode(swPin, INPUT);
}

void loop() {
  // If the button is not pressed the LED is turned on.
  // Depending on the functionality, invert the logic for the code below.

  // First, read the state of the button
  buttonState = digitalRead(swPin);

  // Then comes the if statement
  // Bounce delay has been set in, in order to give the system a short pause so that we avoid the rise time of the response.

  // Set in the inverse statement for pull-down 
  // if(!buttonState){
  //  ....
  // }
  
  if(buttonState){
    digitalWrite(ledPin, LOW);
    delay(BOUNCE_DELAY);
  } else {
    digitalWrite(ledPin, HIGH);
    delay(BOUNCE_DELAY);
  }
}