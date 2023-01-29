#include <Wire.h>
#include <Ramp.h>
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

    // Inspired by EasyButton library
    // https://github.com/evert-arias/EasyButton/blob/4e818410252e9518564fc55f8d4a976fac70a9b2/examples/InterruptsOnPressedFor/InterruptsOnPressedFor.ino
    volatile bool detectedInterrupt = false;

    void updateInterrupt()
    {
        detectedInterrupt = true;
    }

    void update()
    {
        if (!detectedInterrupt)
        {
            return;
        }

        if (pulldown)
            state = digitalRead(pin);
        else
            state = !digitalRead(pin);

        pressed = state && !prevState;
        released = !state && prevState;
        prevState = state;

        detectedInterrupt = false;
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

struct Motor
{
    // Motor library
    // https://github.com/bjoernboeckle/L293D
    // Motor tutorial
    // https://lastminuteengineers.com/l293d-dc-motor-arduino-tutorial/

    // TODO: What pins go where?
    int enablePin = 4;
    int in1Pin = 4;
    int in2Pin = 4;
    L293D motor = L293D(9, 8, 7);

    int minSpeed = 0;
    int maxSpeed = 255;
    int rampDuration = 1000;
    ramp speedController;

    void setup()
    {
        motor.begin();
    }

    int getSpeed() {
        return speedController.getValue();
    }

    int getTargetSpeed() {
        return speedController.getTarget();
    }

    void setTargetSpeed(int speed)
    {
        int targetSpeed = constrain(speed, minSpeed, maxSpeed);
        speedController.go(targetSpeed, rampDuration);
    }

    void update()
    {
        int speed = speedController.update();
        motor.SetMotorSpeed(speed);
    }
};

int temperaturePin = A0;
int temperature = 0;

Timer oledTimer;
int oledState = 0;

Motor motor1;

Button speedUpButton;
Button speedDownButton;
Timer motorSpeedButtonsTimer;

int speedInputSteps = 50;
int motorSpeedHoldTime = 1000;

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
    if (motor1.getSpeed() == 0 && motor1.getTargetSpeed() == 0)
    {
        oled.clearDisplay();
        oled.println("Press the button to start");
        oled.display();
    }

    if (motor1.getSpeed() > 0 && motor1.getTargetSpeed() == 0)
    {
        oled.clearDisplay();
        oled.println("Stopping ...");
        oled.display();
    }

    if (motor1.getTargetSpeed() != 0)
    {
        switch (oledState)
        {
        case 0:
            oledTimer.reset();
            oledState += 1;
        case 1:
            oled.clearDisplay();
            oled.println("Speed");
            oled.print(motor1.getSpeed());
            oled.print(" / ");
            oled.println(motor1.getTargetSpeed());
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

void updateMotorSpeedControls()
{
    int target = motor1.getTargetSpeed();
    int step = speedInputSteps;

    if (speedUpButton.pressed)
    {
        motor1.setTargetSpeed(target + step);
        motorSpeedButtonsTimer.reset();
    }

    if (speedDownButton.pressed)
    {
        motor1.setTargetSpeed(target - step);
        motorSpeedButtonsTimer.reset();
    }

    // When holding down buttons the speed will slowly increase in intervals of x milliseconds
    if (motorSpeedButtonsTimer.isFinished(motorSpeedHoldTime))
    {
        if (speedUpButton.state)
        {
            motor1.setTargetSpeed(target + step);
            motorSpeedButtonsTimer.reset();
        }

        if (speedDownButton.state)
        {
            motor1.setTargetSpeed(target - step);
            motorSpeedButtonsTimer.reset();
        }
    }
}

// Formula from this tutorial
// https://www.makerguides.com/tmp36-arduino-tutorial/
void updateTemperature()
{
    // TODO: Temperature math
    temperature = (analogRead(temperaturePin) - 500) / 10;
}

void onSpeedUpButtonChange()
{
    speedUpButton.updateInterrupt();
}

void onSpeedDownButtonChange()
{
    speedDownButton.updateInterrupt();
}

void setup()
{
    Serial.begin(9600);

    speedUpButton.pin = 2;
    speedUpButton.pulldown = true;
    pinMode(speedUpButton.pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(speedUpButton.pin), onSpeedUpButtonChange, CHANGE);

    speedDownButton.pin = 3;
    speedDownButton.pulldown = true;
    pinMode(speedDownButton.pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(speedDownButton.pin), onSpeedDownButtonChange, CHANGE);

    setupOled();

    motor1.setup();
}

void loop()
{
    speedUpButton.update();
    speedDownButton.update();

    updateTemperature();
    updateOledScreen();
    updateMotorSpeedControls();

    motor1.update();
}
