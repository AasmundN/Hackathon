#include <Arduino.h>

// Declare number of LEDs, LED pins and button pins.
const int numLeds = 3;
const int led0 = 11;
const int led1 = 12;
const int led2 = 13;
const int buttonPinRight = 2;
const int buttonPinLeft = 4;

// Declaring variables for saving the state of the button presses
bool buttonRight = false;
bool buttonLeft = false;

// Activation function for the LEDs
void ledActivate(const int &led){
  digitalWrite(led, HIGH);
}

// Deactivation function for the LEDs
void ledDeactivate(const int &led){
  digitalWrite(led, LOW);
}

// Decoder model for a 3-bit decoder
void decoder(bool &button0, bool &button1){
  if(!button0 && !button1){
    ledDeactivate(led0);
    ledDeactivate(led1);
    ledDeactivate(led2);
  }
  else if(button0 && !button1){
    ledActivate(led0);
    ledDeactivate(led1);
    ledDeactivate(led2);
  }
  else if(!button0 && button1){
    ledDeactivate(led0);
    ledActivate(led1);
    ledDeactivate(led2);
  }
  else{
    ledDeactivate(led0);
    ledDeactivate(led1);
    ledActivate(led2);
  }
}

void setup() {
  // Declare the buttons as inputs and the pins as outputs
  pinMode(buttonPinLeft, INPUT);
  pinMode(buttonPinRight, INPUT);
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop() {
  // First, read the button values
  buttonRight = digitalRead(buttonPinRight);
  buttonLeft = digitalRead(buttonPinLeft);

  decoder(buttonRight, buttonLeft);
}