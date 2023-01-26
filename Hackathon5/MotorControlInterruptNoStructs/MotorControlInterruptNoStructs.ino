#include <Wire.h>
#include <L293D.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


int temperaturePin = A0;
int temperature = 0;

// Motor library
// https://github.com/bjoernboeckle/L293D
// Motor tutorial
// https://lastminuteengineers.com/l293d-dc-motor-arduino-tutorial/

// TODO: What pins go where?
int motorEnableAPin = 4;
int motorEnableBPin = 4;
int motorIn1Pin = 4;
int motorIn2Pin = 4;
int motorIn3Pin = 4;
int motorIn4Pin = 4;
L293D motor(9,8,7);

int minMotorSpeed = 0;
int maxMotorSpeed = 255;

unsigned long oledTimerStartTime;
int oledState = 0;

unsigned long motorSpeedRampTimerStartTime;
int speedInputSteps = 50;
int motorSpeedRampStep = 5;
int motorSpeedRampDelay = 20;
int motorSpeedHoldTime = 1000;
int targetMotorSpeed = 0;
int currentMotorSpeed = 0;

volatile bool speedUpButtonFlankPressed = false;
volatile bool speedDownButtonFlankPressed = false;
int speedUpButtonPin = 2;
int speedDownButtonPin = 3;

// true for pulldown, false for pullup
bool speedUpButtonPulldown = true;
bool speedDownButtonPulldown = true;

bool speedUpButtonState = false;
bool speedDownButtonState = false;
unsigned long motorSpeedButtonsTimerStartTime;

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

void updateOledScreen()
{
    if (currentMotorSpeed == 0 && targetMotorSpeed == 0)
    {
        oled.clearDisplay();
        oled.println("Press the button to start");
        oled.display();
    }

    if (currentMotorSpeed > 0 && targetMotorSpeed == 0)
    {
        oled.clearDisplay();
        oled.println("Stopping ...");
        oled.display();
    }

    if (targetMotorSpeed != 0)
    {
        switch (oledState)
        {
        case 0:
            oledTimerStartTime = millis();
            oledState += 1;
        case 1:
            oled.clearDisplay();
            oled.println("Speed");
            oled.print(currentMotorSpeed);
            oled.print(" / ");
            oled.println(targetMotorSpeed);
            oled.display();

            if ((millis() - oledTimerStartTime) >= 2000) {
                oledTimerStartTime = millis();
                oledState += 1;
            }
            break;
        case 2:
            oled.clearDisplay();
            oled.println("Temp");
            oled.print(temperature);
            oled.println(" *C");
            oled.display();

            if ((millis() - oledTimerStartTime) >= 2000) {
                oledTimerStartTime = millis();
                oledState += 1;
            }
            break;
        default:
            // Go back to start if other states are reached
            oledState = 0;
            break;
        }
    }
}

void setTargetMotorSpeed(int speed)
{
    targetMotorSpeed = constrain(speed, minMotorSpeed, maxMotorSpeed);
}

void setCurrentMotorSpeed(int speed)
{
    currentMotorSpeed = constrain(speed, minMotorSpeed, maxMotorSpeed);
}

void updateMotorSpeedControls()
{
    // When holding down buttons the speed will slowly increase in intervals of x milliseconds
    if ((millis() - motorSpeedButtonsTimerStartTime) >= motorSpeedHoldTime)
    {
        if (speedUpButtonState)
        {
            setTargetMotorSpeed(targetMotorSpeed + speedInputSteps);
            motorSpeedButtonsTimerStartTime = millis();
        }

        if (speedDownButtonState)
        {
            setTargetMotorSpeed(targetMotorSpeed - speedInputSteps);
            motorSpeedButtonsTimerStartTime = millis();
        }
    }
}

void updateCurrentMotorSpeed()
{
    if ((millis() - motorSpeedRampTimerStartTime) >= motorSpeedRampDelay)
    {
        if (currentMotorSpeed < targetMotorSpeed)
        {
            setCurrentMotorSpeed(currentMotorSpeed + motorSpeedRampStep);
        }
        else if (currentMotorSpeed > targetMotorSpeed)
        {
            setCurrentMotorSpeed(currentMotorSpeed - motorSpeedRampStep);
        }

        motorSpeedRampTimerStartTime = millis();
    }
}

// Formula from this tutorial
// https://www.makerguides.com/tmp36-arduino-tutorial/
void updateTemperature()
{
    // TODO: Temperature math
    temperature = (analogRead(temperaturePin) - 500) / 10;
}


void applyCurrentMotorSpeed()
{
    // If an emergency stop is required, it should set the motor speed to 0 in this function.
    motor.SetMotorSpeed(currentMotorSpeed);
}

// These variables can only be used in this function
void updateFlankDetection()
{
    if (speedUpButtonFlankPressed)
    {
        setTargetMotorSpeed(targetMotorSpeed + speedInputSteps);
        motorSpeedButtonsTimerStartTime = millis();
        speedDownButtonFlankPressed = false;
    }

    if (speedDownButtonFlankPressed)
    {
        setTargetMotorSpeed(targetMotorSpeed - speedInputSteps);
        motorSpeedButtonsTimerStartTime = millis();
        speedDownButtonFlankPressed = false;
    }
}

void onSpeedUpButtonPressed()
{
    speedUpButtonFlankPressed = true;
}

void onSpeedDownButtonPressed()
{
    speedDownButtonFlankPressed = true;
}

void updateButtons()
{
    if (speedUpButtonPulldown)
        speedUpButtonState = digitalRead(speedUpButtonPin);
    else
        speedUpButtonState = !digitalRead(speedUpButtonPin);

    if (speedDownButtonPulldown)
        speedDownButtonState = digitalRead(speedDownButtonPin);
    else
        speedDownButtonState = !digitalRead(speedDownButtonPin);
}

void setup()
{
    Serial.begin(9600);

    pinMode(speedUpButtonPin, INPUT);
    pinMode(speedDownButtonPin, INPUT);

    if (speedUpButtonPulldown)
    {
        attachInterrupt(digitalPinToInterrupt(speedUpButtonPin), onSpeedUpButtonPressed, RISING);
    }

    if (speedDownButtonPulldown)
    {
        attachInterrupt(digitalPinToInterrupt(speedDownButtonPin), onSpeedDownButtonPressed, RISING);
    }

    setupOled();

    motor.begin();
}

void loop()
{
    updateFlankDetection();

    updateButtons();

    updateTemperature();
    updateOledScreen();
    updateMotorSpeedControls();
    updateCurrentMotorSpeed();
    applyCurrentMotorSpeed();
}
