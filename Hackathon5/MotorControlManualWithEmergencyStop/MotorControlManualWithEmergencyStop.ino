/*
    Possible compromise:
    Add interrupts to the exercise under the pretence of "safety" while testing the program and circuit? ....
*/

#include <Wire.h>
#include <L293D.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

struct Button
{
    int pin;
    // true for pulldown, false for pullup
    bool pulldown;

    bool state = false;
    bool prevState = false;
    bool pressed = false;
    bool released = false;
    int timesPressed = 0;

    void update()
    {
        if (pulldown)
            state = digitalRead(pin);
        else
            state = !digitalRead(pin);

        pressed = state && !prevState;
        released = !state && prevState;
        prevState = state;

        if (pressed)
        {
            timesPressed++;
        }
    }
};

struct Timer
{
    unsigned long startTime;

    void reset()
    {
        startTime = millis();
    }

    unsigned long getElapsedTime()
    {
        return millis() - startTime;
    }

    bool isFinished(unsigned long duration)
    {
        return getElapsedTime() >= duration;
    }
};

int emergencyStopPin = 5;
volatile bool emergencyStopActive = false;

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
L293D motor(9, 8, 7);

int minMotorSpeed = 0;
int maxMotorSpeed = 255;

Timer oledTimer;
int oledState = 0;

Timer motorSpeedRampTimer;
int speedInputSteps = 50;
int motorSpeedRampStep = 5;
int motorSpeedRampDelay = 20;
int motorSpeedHoldTime = 1000;
int targetMotorSpeed = 0;
int currentMotorSpeed = 0;

Button speedUpButton;
Button speedDownButton;
Timer motorSpeedButtonsTimer;

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

// returns true if the emergency stop button is pressed
// if this function returns true, all following code
// should stop executing
// The guard clause should be at the top of every function,
// and inside any loop.
bool checkEmergencyStop()
{
    if (!emergencyStopActive)
        return false;

    // Critical tasks
    setTargetMotorSpeed(0);
    setCurrentMotorSpeed(0);
    motor.SetMotorSpeed(0);

    // Non-critical tasks
    oled.clearDisplay();
    oled.println("Emergency stop active!");
    oled.display();

    return true;
}

void updateOledScreen()
{
    if (checkEmergencyStop())
        return;

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
            oledTimer.reset();
            oledState += 1;
        case 1:
            oled.clearDisplay();
            oled.println("Speed");
            oled.print(currentMotorSpeed);
            oled.print(" / ");
            oled.println(targetMotorSpeed);
            oled.display();

            if (oledTimer.isFinished(1000))
            {
                oledTimer.reset();
                oledState += 1;
            }
            break;
        case 2:
            oled.clearDisplay();
            oled.println("Temp");
            oled.print(temperature);
            oled.println(" *C");
            oled.display();

            if (oledTimer.isFinished(1000))
            {
                oledTimer.reset();
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
    if (checkEmergencyStop())
        return;

    if (speedUpButton.pressed)
    {
        setTargetMotorSpeed(targetMotorSpeed + speedInputSteps);
        motorSpeedButtonsTimer.reset();
    }

    if (speedDownButton.pressed)
    {
        setTargetMotorSpeed(targetMotorSpeed - speedInputSteps);
        motorSpeedButtonsTimer.reset();
    }

    // When holding down buttons the speed will slowly increase in intervals of x milliseconds
    if (motorSpeedButtonsTimer.isFinished(motorSpeedHoldTime))
    {
        if (speedUpButton.state)
        {
            setTargetMotorSpeed(targetMotorSpeed + speedInputSteps);
            motorSpeedButtonsTimer.reset();
        }

        if (speedDownButton.state)
        {
            setTargetMotorSpeed(targetMotorSpeed - speedInputSteps);
            motorSpeedButtonsTimer.reset();
        }
    }
}

void updateCurrentMotorSpeed()
{
    if (checkEmergencyStop())
        return;

    if (motorSpeedRampTimer.isFinished(motorSpeedRampDelay))
    {
        if (currentMotorSpeed < targetMotorSpeed)
        {
            setCurrentMotorSpeed(currentMotorSpeed + motorSpeedRampStep);
        }
        else if (currentMotorSpeed > targetMotorSpeed)
        {
            setCurrentMotorSpeed(currentMotorSpeed - motorSpeedRampStep);
        }

        motorSpeedRampTimer.reset();
    }
}

// Formula from this tutorial
// https://www.makerguides.com/tmp36-arduino-tutorial/
void updateTemperature()
{
    if (checkEmergencyStop())
        return;

    // TODO: Temperature math
    temperature = (analogRead(temperaturePin) - 500) / 10;
}

void applyCurrentMotorSpeed()
{
    if (checkEmergencyStop())
        return;

    motor.SetMotorSpeed(currentMotorSpeed);
}

void onEmergencyStopPressed()
{
    emergencyStopActive = true;
}

void onEmergencyStopReleased()
{
    emergencyStopActive = false;
}

void setup()
{
    Serial.begin(9600);

    speedUpButton.pin = 2;
    speedUpButton.pulldown = true;
    pinMode(speedUpButton.pin, INPUT);

    speedDownButton.pin = 3;
    speedDownButton.pulldown = true;
    pinMode(speedDownButton.pin, INPUT);

    // Emergency stop buttons MUST ALWAYS be pulldown in case of something breaking
    attachInterrupt(digitalPinToInterrupt(emergencyStopPin), onEmergencyStopPressed, FALLING);
    attachInterrupt(digitalPinToInterrupt(emergencyStopPin), onEmergencyStopReleased, RISING);

    setupOled();

    motor.begin();
}

void loop()
{
    speedUpButton.update();
    speedDownButton.update();

    updateTemperature();
    updateOledScreen();
    updateMotorSpeedControls();
    updateCurrentMotorSpeed();
    applyCurrentMotorSpeed();
}
