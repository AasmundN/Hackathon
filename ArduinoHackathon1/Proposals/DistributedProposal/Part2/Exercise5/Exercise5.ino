int speedUpButtonPin = 2;
int speedDownButtonPin = 3;

volatile byte motorState = 0;

bool speedUpButtonState = false;
bool speedDownButtonState = false;

unsigned long buttonHoldStartTime = 0;

void increaseMotorState();
void decreaseMotorState();

void updateButtonStates()
{
    // Since buttons are on a pulldown circuit, we need to invert the inputs
    speedUpButtonState = !digitalRead(speedUpButtonPin);
    speedDownButtonState = !digitalRead(speedDownButtonPin);
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
    updateButtonStates();
    updateButtonHolds();

    Serial.println(motorState);
}
