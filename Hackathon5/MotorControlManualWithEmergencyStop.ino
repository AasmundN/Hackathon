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
bool emergencyStopActive = false;

int motorPin = 4;
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

void updateOledScreen()
{
    if (oledTimer.isFinished(2000))
    {
        oledState += 1;
        oledTimer.reset();
    }

    switch (oledState)
    {
    case 0:
        Serial.println("State 0");
        oledState += 1;
        break;
    case 1:
        // Do nothing until the timer is finished
        break;
    case 2:
        Serial.println("State 1");
        oledState += 1;
        break;
    case 3:
        // Do nothing until the timer is finished
        break;
    case 4:
        Serial.println("State 2");
        oledState += 1;
        break;
    case 5:
        // Do nothing until the timer is finished
        break;
    case 6:
        Serial.println("State 3");
        oledState += 1;
        break;
    case 7:
        // Do nothing until the timer is finished
        break;
    case 8:
        Serial.println("State 4");
        oledState += 1;
        break;
    case 9:
        // Do nothing until the timer is finished
        break;
    default:
        // Go back to start if other states are reached
        oledState = 0;
        break;
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
    if (emergencyStopActive)
    {
        setTargetMotorSpeed(0);
        return;
    }

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
    if (emergencyStopActive)
    {
        setCurrentMotorSpeed(0);
        return;
    }

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

void applyCurrentMotorSpeed()
{
    if (emergencyStopActive)
    {
        analogWrite(motorPin, 0);
        return;
    }

    analogWrite(motorPin, currentMotorSpeed);
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

    pinMode(motorPin, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(emergencyStopPin), onEmergencyStopPressed, FALLING);
    attachInterrupt(digitalPinToInterrupt(emergencyStopPin), onEmergencyStopReleased, RISING);
}

void loop()
{
    speedUpButton.update();
    speedDownButton.update();

    updateOledScreen();
    updateMotorSpeedControls();
    updateCurrentMotorSpeed();
    applyCurrentMotorSpeed();
}
