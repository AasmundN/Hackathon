#include <Arduino.h>
#define DELAY 450

// IS MORE ACCURATE WITH MILLIS

// Declare number of LEDs, LED pins and button pins.
const int numLeds = 3;
const int yellowLed = 11;
const int blueLed = 12;
const int greenLed = 13;
const int buttonPinRight = 2;
const int buttonPinLeft = 4;

// Declaring variables for saving the state of the button presses
bool buttonRight = false;
bool buttonLeft = false;

// Declaring a variable which shows which LED is on
unsigned int currentLed = 0;

// Declaring blink function for the different leds
void ledBlink(const int &led, const int &delayTimer){
  digitalWrite(led, HIGH);
  delay(delayTimer);
  digitalWrite(led, LOW);
  delay(delayTimer);
};

// Declaring a switch function which switches the controlled LED
void ledSwitch(const int &ledCount){
  switch (ledCount) {
    case 0:
      ledBlink(yellowLed, DELAY);
      break;
    case 1:
      ledBlink(blueLed, DELAY);
      break;
    case 2:
      ledBlink(greenLed, DELAY);
      break;
    default:
      break;
  }
}

void setup() {
  // Declare the buttons as inputs and the pins as outputs
  pinMode(buttonPinLeft, INPUT);
  pinMode(buttonPinRight, INPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
}

void loop() {
  // First, read the button values
  buttonRight = digitalRead(buttonPinRight);
  buttonLeft = digitalRead(buttonPinLeft);

  if(buttonRight && !buttonLeft){
    currentLed--;
    if(currentLed < 0){
      currentLed = 2;
    }
  } 
  else if(!buttonRight && !buttonLeft){
    currentLed++;
    if(currentLed > 2){
      currentLed = 0;
    }
  }
  else{
    currentLed = currentLed;
  }

  ledSwitch(currentLed);
}