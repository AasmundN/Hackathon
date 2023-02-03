int speedUpButtonPin = 2;
int speedDownButtonPin = 3;

volatile byte motorState = 0;

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

void setup()
{
    Serial.begin(9600);

    pinMode(speedUpButtonPin, INPUT);
    pinMode(speedDownButtonPin, INPUT);

    attachInterrupt(digitalPinToInterrupt(speedUpButtonPin), onSpeedUpButtonPressed, FALLING);
    attachInterrupt(digitalPinToInterrupt(speedDownButtonPin), onSpeedDownButtonPressed, FALLING);
}

void loop()
{
    Serial.println(motorState);
    // Serial.println(speedUpButtonState);
    // Serial.println(speedDownButtonState);
}
