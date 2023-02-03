#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int speedUpButtonPin = 2;
int speedDownButtonPin = 3;

int motorCcwPin = 4;
int motorCwPin = 5;
int motorEnablePin = 6;

volatile byte motorState = 0;

bool speedUpButtonState = false;
bool speedDownButtonState = false;

unsigned long buttonHoldStartTime = 0;

int oledSequenceStep = 0;
unsigned long oledSequenceStartTime = 0;

unsigned long activeTime = 0;
unsigned long activeTimeStartTime = 0;

/////////////////////////////////////////////////////////////////////
//////////////////// OLED Setup from tutorial ///////////////////////
/////////////////////////////////////////////////////////////////////
// https://arduinogetstarted.com/tutorials/arduino-oled

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setupOled()
{
    // initialize OLED display with address 0x3C for 128x64
    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        while (true)
            ;
    }

    delay(2000);         // wait for initializing
    oled.clearDisplay(); // clear display

    oled.setTextSize(1);          // text size
    oled.setTextColor(WHITE);     // text color
    oled.setCursor(0, 10);        // position to display
    oled.println("Starting ..."); // text to display
    oled.display();               // show on OLED
    delay(2000);
}

void printToDisplay(String message)
{
    oled.clearDisplay();
    oled.println(message);
    oled.display();
}

void printMotorSpeed()
{
    String message = "Motor speed: " + String(motorState);
    printToDisplay(message);
}

void printActiveTime()
{

    String message = "Active time: " + String(activeTime);
    printToDisplay(message);
}

void updateOledScreen()
{
    unsigned long sequenceTime = millis() - oledSequenceStartTime;

    if (sequenceTime > 1000)
    {
        oledSequenceStep += 1;
        oledSequenceStartTime = millis();
    }

    switch (oledSequenceStep)
    {
    case 0:
        printMotorSpeed();
        break;
    case 1:
        printActiveTime();
        break;
    default:
        oledSequenceStep = 0;
        break;
    }
}

void updateActiveTime()
{
    if (motorState == 0)
    {
        unsigned long now = millis();
        activeTime = now - activeTimeStartTime;
        activeTimeStartTime = now;
    }
}

void updateMotorSpeed()
{
    switch (motorState)
    {
    // Stopped
    case 0:
        analogWrite(motorEnablePin, 0);
        digitalWrite(motorCwPin, LOW);
        digitalWrite(motorCcwPin, LOW);
        break;
    // Slow
    case 1:
        analogWrite(motorEnablePin, 100);
        digitalWrite(motorCwPin, LOW);
        digitalWrite(motorCcwPin, HIGH);
        break;
    // Medium
    case 2:
        analogWrite(motorEnablePin, 200);
        digitalWrite(motorCwPin, LOW);
        digitalWrite(motorCcwPin, HIGH);
        break;
    // Fast
    case 3:
        analogWrite(motorEnablePin, 255);
        digitalWrite(motorCwPin, LOW);
        digitalWrite(motorCcwPin, HIGH);
        break;
    default:
        motorState = 0;
        break;
    }
}

void increaseMotorState()
{
    if (motorState < 3)
    {
        motorState += 1;
    }
}

void decreaseMotorState()
{
    if (motorState > 1)
    {
        motorState -= 1;
    }
}

void updateButtonStates()
{
    // Since buttons are on a pulldown circuit, we need to invert the inputs
    speedUpButtonState = !digitalRead(speedUpButtonPin);
    speedDownButtonState = !digitalRead(speedDownButtonPin);
}

void updateButtonHolds()
{

    unsigned long holdTime = millis() - buttonHoldStartTime;

    if (holdTime > 1000)
    {

        if (speedUpButtonState)
        {
            increaseMotorState();
        }

        if (speedDownButtonState)
        {
            decreaseMotorState();
        }

        buttonHoldStartTime = millis();
    }
}

void onSpeedUpButtonPressed()
{
    increaseMotorState();
}

void onSpeedDownButtonPressed()
{
    decreaseMotorState();
}

void setup()
{
    Serial.begin(9600);

    pinMode(speedUpButtonPin, INPUT);
    pinMode(speedDownButtonPin, INPUT);
    pinMode(motorCwPin, OUTPUT);
    pinMode(motorCcwPin, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(speedUpButtonPin), onSpeedUpButtonPressed, FALLING);
    attachInterrupt(digitalPinToInterrupt(speedDownButtonPin), onSpeedDownButtonPressed, FALLING);

    setupOled();
}

void loop()
{
    updateButtonStates();
    updateButtonHolds();
    updateActiveTime();

    updateOledScreen();
    updateMotorSpeed();
}
