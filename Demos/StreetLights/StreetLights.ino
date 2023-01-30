
int button = 2;
int redLed = 3;
int yellowLed = 4;
int greenLed = 5;

enum class LightState
{
    RED = 0,
    WAIT_FOR_YELLOW = 1,
    YELLOW = 2,
    WAIT_FOR_GREEN = 3,
    GREEN = 4,
    WAIT_FOR_RED = 5
};

LightState lightState = LightState::RED;

void updateLight()
{
    switch (lightState)
    {
    // RED
    case LightState::RED:
        digitalWrite(redLed, HIGH);
        digitalWrite(yellowLed, LOW);
        digitalWrite(greenLed, LOW);

        if (digitalRead(button) == HIGH)
        {
            lightState = LightState::WAIT_FOR_YELLOW;
        }
        break;
    case LightState::WAIT_FOR_YELLOW:
        if (digitalRead(button) == LOW)
        {
            lightState = LightState::YELLOW;
        }
    // YELLOW
    case LightState::YELLOW:
        digitalWrite(redLed, LOW);
        digitalWrite(yellowLed, HIGH);
        digitalWrite(greenLed, LOW);

        if (digitalRead(button) == HIGH)
        {
            lightState = LightState::WAIT_FOR_GREEN;
        }
        break;
    case LightState::WAIT_FOR_GREEN:
        if (digitalRead(button) == LOW)
        {
            lightState = LightState::GREEN;
        }
    // GREEN
    case LightState::GREEN:
        digitalWrite(redLed, LOW);
        digitalWrite(yellowLed, LOW);
        digitalWrite(greenLed, HIGH);

        if (digitalRead(button) == HIGH)
        {
            lightState = LightState::WAIT_FOR_RED;
        }
        break;
    case LightState::WAIT_FOR_RED:
        if (digitalRead(button) == LOW)
        {
            lightState = LightState::RED;
        }
    }
}

void setup()
{
    pinMode(button, INPUT);
    pinMode(redLed, OUTPUT);
    pinMode(yellowLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
}

void loop()
{
    updateLight();
}