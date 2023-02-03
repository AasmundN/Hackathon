#include<avr/wdt.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

volatile byte motorState = 0;

int motorCcwPin = 4;
int motorCwPin = 5;
int motorEnablePin = 6;

int speedUpButtonPin = 2;
int speedDownButtonPin = 3;

//////////////////// OLED Setup from tutorial ///////////////////////
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
        
        // Enable the watchdog timer so the microcontroller resets after 2 seconds
        wdt_enable(WDTO_2S);
        while (true) {
            // Since the conenction failed,
            // wait for the watchdog timer to restart the microcontroller
        }
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

void writeMotorSpeed(int speed)
{
    // Set the direction
    digitalWrite(motorCwPin, LOW);
    digitalWrite(motorCcwPin, HIGH);

    // Set the speed
    analogWrite(motorEnablePin, speed);
}

void updateMotorSpeed()
{
    switch (motorState)
    {
    // Stopped
    case 0:
        writeMotorSpeed(0);
        break;
    // Slow
    case 1:
        writeMotorSpeed(100);
        break;
    // Medium
    case 2:
        writeMotorSpeed(200);
        break;
    // Fast
    case 3:
        writeMotorSpeed(255);
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
    if (motorState > 0)
    {
        motorState -= 1;
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

///////////////////////////////////////////////////////
///////////////////// Extra Logic /////////////////////
///////////////////////////////////////////////////////

bool speedUpButtonState = false;
bool speedDownButtonState = false;

unsigned long buttonHoldStartTime = 0;

void updateButtonStates()
{
    // Since buttons are on a pulldown circuit, we need to invert the inputs
    speedUpButtonState = !digitalRead(speedUpButtonPin);
    speedDownButtonState = !digitalRead(speedDownButtonPin);
}

void updateButtonHolds()
{
    if (!speedUpButtonState && !speedDownButtonState)
    {
        buttonHoldStartTime = millis();
    }

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

///////////////////////////////////////////////////////

unsigned long activeTime = 0;
unsigned long activeTimeStartTime = 0;

void printActiveTime()
{

    String message = "Active time: " + String(activeTime);
    printToDisplay(message);
}

void updateActiveTime()
{
    if (motorState == 0)
    {
        activeTimeStartTime = millis();
    }

    activeTime = millis() - activeTimeStartTime;
}

///////////////////////////////////////////////////////

int oledSequenceStep = 0;
unsigned long oledSequenceStartTime = 0;

void updateOledTimer()
{
    unsigned long sequenceStepTime = millis() - oledSequenceStartTime;

    if (sequenceStepTime > 1000)
    {
        oledSequenceStep += 1;
        oledSequenceStartTime = millis();
    }
}

void updateOledScreen()
{

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

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

void setup()
{
    Serial.begin(9600);
    
    setupOled();
    
    pinMode(motorCcwPin, OUTPUT);
    pinMode(motorCwPin, OUTPUT);
    pinMode(motorEnablePin, OUTPUT);
    
    pinMode(speedUpButtonPin, INPUT);
    pinMode(speedDownButtonPin, INPUT);

    attachInterrupt(digitalPinToInterrupt(speedUpButtonPin), onSpeedUpButtonPressed, FALLING);
    attachInterrupt(digitalPinToInterrupt(speedDownButtonPin), onSpeedDownButtonPressed, FALLING);
}

void loop()
{
    updateButtonStates();
    updateMotorSpeed();
    updateButtonHolds();
    updateActiveTime();
    updateOledTimer();
    updateOledScreen();

    printMotorSpeed();
}
