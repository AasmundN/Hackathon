volatile byte motorState = 0;

bool speedUpButtonState = false;
bool speedDownButtonState = false;

unsigned long buttonHoldStartTime = 0;

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

void updateButtonHolds()
{
    if (!speedUpButtonState && !speedDownButtonState)
    {
        buttonHoldStartTime = millis();
    }

    unsigned long holdTime = millis() - buttonHoldStartTime;

    if (holdTime > 1000)
    {

        if (speedUpButtonState)
        {
            increaseMotorState();
        }
        
        if (speedDownButtonState)
        {
            decreaseMotorState();
        }

        buttonHoldStartTime = millis();
    }
}

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    updateButtonHolds();

    Serial.println(motorState);
}
