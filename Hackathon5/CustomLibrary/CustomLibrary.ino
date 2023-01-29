#include "Button.h";
#include "Timer.h";
#include "Motor.h";
#include "Display.h";
#include "Thermometer.h";

Display display;

Thermometer thermometer;

Timer oledTimer;
int oledState = 0;

Motor motor1;

Button speedUpButton;
Button speedDownButton;
Timer motorSpeedButtonsTimer;

int speedInputSteps = 50;
int motorSpeedHoldTime = 1000;

void updateOledScreen()
{
    if (motor1.getSpeed() == 0 && motor1.getTargetSpeed() == 0)
    {
        display.oled.clearDisplay();
        display.oled.println("Press the button to start");
        display.oled.display();
    }

    if (motor1.getSpeed() > 0 && motor1.getTargetSpeed() == 0)
    {
        display.oled.clearDisplay();
        display.oled.println("Stopping ...");
        display.oled.display();
    }

    if (motor1.getTargetSpeed() != 0)
    {
        switch (oledState)
        {
        case 0:
            oledTimer.reset();
            oledState += 1;
        case 1:
            display.oled.clearDisplay();
            display.oled.println("Speed");
            display.oled.print(motor1.getSpeed());
            display.oled.print(" / ");
            display.oled.println(motor1.getTargetSpeed());
            display.oled.display();

            if (oledTimer.isFinished(1000))
            {
                oledTimer.reset();
                oledState += 1;
            }
            break;
        case 2:
            display.oled.clearDisplay();
            display.oled.println("Temp");
            display.oled.print(thermometer.value);
            display.oled.println(" *C");
            display.oled.display();

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

    thermometer.pin = A0;

    speedUpButton.pin = 2;
    speedUpButton.pulldown = true;
    pinMode(speedUpButton.pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(speedUpButton.pin), onSpeedUpButtonChange, CHANGE);

    speedDownButton.pin = 3;
    speedDownButton.pulldown = true;
    pinMode(speedDownButton.pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(speedDownButton.pin), onSpeedDownButtonChange, CHANGE);

    display.setup();

    motor1.setup();
}

void loop()
{
    speedUpButton.update();
    speedDownButton.update();

    thermometer.update();

    updateOledScreen();
    updateMotorSpeedControls();

    motor1.update();
}
