// https://www.tinkercad.com/things/kowNgb05nG5

enum TrafficLightState
{
    RED,
    RED_YELLOW,
    YELLOW,
    GREEN
};

struct TrafficLight
{
    int redPin;
    int yellowPin;
    int greenPin;

    unsigned long changeTimer = 0;
    TrafficLightState state = RED;

    void setup()
    {
        pinMode(redPin, OUTPUT);
        pinMode(yellowPin, OUTPUT);
        pinMode(greenPin, OUTPUT);

        writeLeds();
    }

    void writeLeds()
    {
        switch (state)
        {
        case RED:
            digitalWrite(redPin, HIGH);
            digitalWrite(yellowPin, LOW);
            digitalWrite(greenPin, LOW);
            break;
        case RED_YELLOW:
            digitalWrite(redPin, HIGH);
            digitalWrite(yellowPin, HIGH);
            digitalWrite(greenPin, LOW);
            break;
        case YELLOW:
            digitalWrite(redPin, LOW);
            digitalWrite(yellowPin, HIGH);
            digitalWrite(greenPin, LOW);
            break;
        case GREEN:
            digitalWrite(redPin, LOW);
            digitalWrite(yellowPin, LOW);
            digitalWrite(greenPin, HIGH);
            break;
        }
    }

    void nextState()
    {
        switch (state)
        {
        case RED:
            state = RED_YELLOW;
            break;
        case RED_YELLOW:
            state = GREEN;
            break;
        case GREEN:
            state = YELLOW;
            break;
        case YELLOW:
            state = RED;
            break;
        }

        writeLeds();
    }

    unsigned long getChangeInterval()
    {
        switch (state)
        {
        case RED:
            return 5000;
        case RED_YELLOW:
            return 500;
        case GREEN:
            return 2000;
        case YELLOW:
            return 500;
        }
    }

    void update()
    {
        if ((millis() - changeTimer) > getChangeInterval())
        {
            nextState();
            changeTimer = millis();
        }
    }
};

TrafficLight northLight;
TrafficLight southLight;

void setup()
{
    Serial.begin(9600);

    northLight.redPin = 2;
    northLight.yellowPin = 3;
    northLight.greenPin = 4;

    southLight.redPin = 5;
    southLight.yellowPin = 6;
    southLight.greenPin = 7;
    southLight.state = GREEN;

    northLight.setup();
    southLight.setup();
}

void loop()
{
    northLight.update();
    southLight.update();
}
