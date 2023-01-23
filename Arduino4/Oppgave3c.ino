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

int step = 0;

Timer timer1;
Button button1;

void setup()
{
    Serial.begin(9600);

    button1.pin = 2;
    button1.pulldown = true;
    pinMode(button1.pin, INPUT);
}

void updateSwitch()
{
    if (button1.pressed)
    {
        step++;
    }

    switch (step)
    {
    case 0:
        Serial.println("Step 0");
        timer1.reset();
        step += 1;
        break;
    case 1:
        if (timer1.isFinished(1000))
        {
            step += 1;
        }
        break;
    case 2:
        Serial.println("Step 1");
        step += 1;
        break;
    case 4:
        Serial.println("Step 2");
        step += 1;
        break;
    case 6:
        Serial.println("Step 3");
        step += 1;
        break;
    default:
        step = 0;
        break;
    }
}

void loop()
{
    button1.update();

    updateSwitch();
}
