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

struct ButtonPressCounter
{
    bool state = false;
    bool prevState = false;
    bool pressed = false;
    bool released = false;

    int timesPressed = 0;
    int timesReleased = 0;

    void reset()
    {
        timesPressed = 0;
        timesReleased = 0;
    }

    bool wasPressedTimes(int times)
    {
        return timesPressed == times;
    }

    bool wasReleasedTimes(int times)
    {
        return timesReleased == times;
    }

    void update(bool newState)
    {
        state = newState;
        pressed = state && !prevState;
        released = !state && prevState;
        prevState = state;

        if (pressed)
            timesPressed++;

        if (released)
            timesReleased++;
    }
};

Button button1;
ButtonPressCounter pressCounter1;
ButtonPressCounter pressCounter2;
ButtonPressCounter pressCounter3;

void setup()
{
    Serial.begin(9600);

    button1.pin = 2;
    button1.pulldown = true;
    pinMode(button1.pin, INPUT);
}

void loop()
{
    button1.update();
    pressCounter1.update(button1.state);
    pressCounter2.update(button1.state);
    pressCounter3.update(button1.state);

    if (pressCounter1.wasPressedTimes(1))
    {
        Serial.print("Button1, Pressed ");
        Serial.print(pressCounter1.timesPressed);
        Serial.println(" times");
        pressCounter1.reset();
    }

    if (pressCounter2.wasPressedTimes(3))
    {
        Serial.print("Button1, Pressed ");
        Serial.print(pressCounter2.timesPressed);
        Serial.println(" times");
        pressCounter2.reset();
    }

    if (pressCounter3.wasPressedTimes(10))
    {
        Serial.print("Button1, Pressed ");
        Serial.print(pressCounter3.timesPressed);
        Serial.println(" times");
        pressCounter3.reset();
    }
}
