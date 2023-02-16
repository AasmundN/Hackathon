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

        if (!pressed && !released)
        	detectedInterrupt = false;
    }
};