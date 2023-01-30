#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Motor library
// https://github.com/bjoernboeckle/L293D
// Motor tutorial
// https://lastminuteengineers.com/l293d-dc-motor-arduino-tutorial/

int motorEnablePin = 4;
int motorIn1Pin = 4;
int motorIn2Pin = 4;

volatile byte motorState = 0;

int speedUpButtonPin = 2;
int speedDownButtonPin = 3;

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

void printToDisplay(String message) {
    oled.clearDisplay();
    oled.println(message);
    oled.display();
}

void updateOledScreen()
{
    String message = "Motor speed: " + String(motorState);
    printToDisplay(message);
}

void updateMotorSpeed() {
    switch (motorState) {
        // Stopped
        case 0:
            analogWrite(motorEnablePin, 0);
            break;
        // Slow
        case 1:
            analogWrite(motorEnablePin, 100);
            digitalWrite(motorIn1Pin, HIGH);
            digitalWrite(motorIn2Pin, LOW);
            break;
        // Medium
        case 2:
            analogWrite(motorEnablePin, 200);
            digitalWrite(motorIn1Pin, HIGH);
            digitalWrite(motorIn2Pin, LOW);
            break;
        // Fast
        case 3:
            analogWrite(motorEnablePin, 255);
            digitalWrite(motorIn1Pin, HIGH);
            digitalWrite(motorIn2Pin, LOW);
            break;
        default:
            motorState = 0;
            break;
    }
}

void onSpeedUpButtonPressed()
{
    if (motorState < 3) {
        motorState += 1;
    }
}

void onSpeedDownButtonPressed()
{
    if(motorState > 1) {
        motorState -= 1;
    }
}

void setup()
{
    Serial.begin(9600);

    pinMode(speedUpButtonPin, INPUT);
    pinMode(speedDownButtonPin, INPUT);
    pinMode(motorIn1Pin, INPUT);
    pinMode(motorIn2Pin, INPUT);
    pinMode(motorEnablePin, INPUT);

    attachInterrupt(digitalPinToInterrupt(speedUpButtonPin), onSpeedUpButtonPressed, FALLING);
    attachInterrupt(digitalPinToInterrupt(speedDownButtonPin), onSpeedDownButtonPressed, FALLING);

    setupOled();
}

void loop()
{
    updateOledScreen();
    updateMotorSpeed();
}
