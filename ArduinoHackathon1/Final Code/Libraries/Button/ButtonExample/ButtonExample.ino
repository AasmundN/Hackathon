struct Button
{
    int pin;
    // true for pulldown, false for pullup
    bool pulldown;

    bool state = false;
    bool prevState = false;
    bool pressed = false;
    bool released = false;

    volatile bool detectedInterrupt = false;

    void updateInterrupt()
    {
        detectedInterrupt = true;
    }

    void update()
    {
        if (!detectedInterrupt)
            return;

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

int motorState = 0;

Button speedUpButton;
Button speedDownButton;

void updateMotorSpeedControls()
{
    if (speedUpButton.pressed)
    {
        motorState++;
    }

    if (speedDownButton.pressed)
    {
        motorState--;
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

    speedUpButton.pin = 2;
    speedUpButton.pulldown = true;
    pinMode(speedUpButton.pin, INPUT);

    speedDownButton.pin = 3;
    speedDownButton.pulldown = true;
    pinMode(speedDownButton.pin, INPUT);

    attachInterrupt(digitalPinToInterrupt(speedUpButton.pin), onSpeedUpButtonChange, CHANGE);
    attachInterrupt(digitalPinToInterrupt(speedDownButton.pin), onSpeedDownButtonChange, CHANGE);
}

void loop()
{
    speedUpButton.update();
    speedDownButton.update();

    updateMotorSpeedControls();

    Serial.print("Speed: ");
    Serial.println(motorState);
}
