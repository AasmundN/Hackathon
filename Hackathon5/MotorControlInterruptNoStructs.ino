int motorPin = 4;
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

int speedUpButtonPin = 2;
int speedDownButtonPin = 3;

// true for pulldown, false for pullup
bool speedUpButtonPulldown = true;
bool speedDownButtonPulldown = true;

bool speedUpButtonState = false;
bool speedDownButtonState = false;
unsigned long motorSpeedButtonsTimerStartTime;

void updateOledScreen()
{
    if ((millis() - oledTimerStartTime) >= 2000)
    {
        oledState += 1;
        oledTimerStartTime = millis();
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

void applyCurrentMotorSpeed()
{
    // If an emergency stop is required, it should set the motor speed to 0 in this function.
    analogWrite(motorPin, currentMotorSpeed);
}

void onSpeedUpButtonPressed()
{
    setTargetMotorSpeed(targetMotorSpeed + speedInputSteps);
    motorSpeedButtonsTimerStartTime = millis();
}

void onSpeedDownButtonPressed()
{
    setTargetMotorSpeed(targetMotorSpeed - speedInputSteps);
    motorSpeedButtonsTimerStartTime = millis();
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

    pinMode(motorPin, OUTPUT);
}

void loop()
{
    updateButtons();

    updateOledScreen();
    updateMotorSpeedControls();
    updateCurrentMotorSpeed();
    applyCurrentMotorSpeed();
}
