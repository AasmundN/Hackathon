const int SPEED_UP_BUTTON_PIN = 2;
const int SPEED_DOWN_BUTTON_PIN = 3;

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

    pinMode(SPEED_UP_BUTTON_PIN, INPUT);
    pinMode(SPEED_DOWN_BUTTON_PIN, INPUT);

    attachInterrupt(digitalPinToInterrupt(SPEED_UP_BUTTON_PIN), onSpeedUpButtonPressed, RISING);
    attachInterrupt(digitalPinToInterrupt(SPEED_DOWN_BUTTON_PIN), onSpeedDownButtonPressed, RISING);
}

void loop()
{
    Serial.println(motorState);
    // Serial.println(speedUpButtonState);
    // Serial.println(speedDownButtonState);
}