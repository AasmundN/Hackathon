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

void setup()
{
    Serial.begin(9600);

    pin = 2;
    pulldown = true;

    pinMode(pin, INPUT);
}

void loop()
{
    update();

    if (pressed)
    {
        Serial.print("Button1, Pressed ");
        Serial.print(timesPressed);
        Serial.println(" times");
    }

    if (released)
    {
        Serial.println("Button1, Released");
    }
}
