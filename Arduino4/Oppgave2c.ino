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

Button button1;
Button button2;
Button button3;

void setup()
{
    Serial.begin(9600);

    button1.pin = 2;
    button2.pin = 3;
    button3.pin = 4;

    button1.pulldown = true;
    button2.pulldown = true;
    button3.pulldown = true;

    pinMode(button1.pin, INPUT);
    pinMode(button2.pin, INPUT);
    pinMode(button3.pin, INPUT);
}

void loop()
{
    button1.update();
    button2.update();
    button3.update();

    if (button1.pressed)
    {
        Serial.print("Button1, Pressed ");
        Serial.print(button1.timesPressed);
        Serial.println(" times");
    }

    if (button1.released)
    {
        Serial.println("Button1, Released");
    }

    if (button2.pressed)
    {
        Serial.print("Button2, Pressed ");
        Serial.print(button1.timesPressed);
        Serial.println(" times");
    }

    if (button2.released)
    {
        Serial.println("Button2, Released");
    }

    if (button3.pressed)
    {
        Serial.print("Button3, Pressed ");
        Serial.print(button1.timesPressed);
        Serial.println(" times");
    }

    if (button3.released)
    {
        Serial.println("Button3, Released");
    }
}
