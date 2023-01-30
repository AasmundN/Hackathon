#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int speedUpButtonPin = 2;
int speedDownButtonPin = 3;

int motorCcwPin = 4;
int motorCwPin = 5;
int motorEnablePin = 6;

volatile byte motorState = 0;

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

void updateOledScreen()
{
    String message = "Motor speed: " + String(motorState);
    printToDisplay(message);
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

void onSpeedUpButtonPressed()
{
    if (motorState < 3)
    {
        motorState += 1;
    }
}

void onSpeedDownButtonPressed()
{
    if (motorState > 1)
    {
        motorState -= 1;
    }
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
    updateOledScreen();
    updateMotorSpeed();
}
