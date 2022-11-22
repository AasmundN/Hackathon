// TinkerCad: https://www.tinkercad.com/things/4ehw5aqq2LC

// Define structs
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

struct Led
{
    int pin;
    bool state = false;
    int value = 0;

    void toggle()
    {
        state = !state;
        digitalWrite(pin, state);
    }

    void setDigital(bool newState)
    {
        state = newState;
        digitalWrite(pin, state);
    }

    void setAnalog(int newValue)
    {
        value = newValue;
        analogWrite(pin, newValue);
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

    bool isFinished(const unsigned long durationMs)
    {
        return getElapsedTime() >= durationMs;
    }
};

// Instantiate structs
Led led1;
Button button1;
Timer blinkTimer;
Timer fadeTimer;
Timer mainTimer;

// Globals
int blinkSequenceStep = 0;
int fadeSequenceStep = 0;
int mainSequenceStep = 0;

int timesBlinked = 0;

/**
 * Will blink the led on and off once.
 *
 * returns true when the sequence finishes
 */
bool updateBlinkSequence()
{

    switch (blinkSequenceStep)
    {
    case 0:
        led1.setDigital(true);
        blinkSequenceStep++;
        break;
    case 1:
        blinkTimer.reset();
        blinkSequenceStep++;
        break;
    case 2:
        if (blinkTimer.isFinished(1000))
            blinkSequenceStep++;
        break;
    case 3:
        led1.setDigital(false);
        blinkSequenceStep++;
        break;
    case 4:
        blinkTimer.reset();
        blinkSequenceStep++;
        break;
    case 5:
        if (blinkTimer.isFinished(1000))
            blinkSequenceStep++;
        break;
    case 6:
        blinkSequenceStep = 0;
        // Return true to indicate that the sequence
        // has finished and will restart from step 0
        return true;
    }

    // Return false to indicate that the sequence is still running
    return false;
}

bool updateRunBlinkSequenceTimes(int times)
{
    if (updateBlinkSequence())
    {
        timesBlinked++;
        if (timesBlinked >= times)
        {
            timesBlinked = 0;
            return true;
        }
    }
    return false;
}

/**
 * Will fade the led on and off once.
 *
 * returns true when the sequence finishes
 */
bool updateFadeSequence()
{
    switch (fadeSequenceStep)
    {
    case 0:
        led1.setAnalog(0);
        fadeSequenceStep++;
        break;
    case 1:
        if (fadeTimer.isFinished(20))
        {
            fadeTimer.reset();
            led1.setAnalog(led1.value + 5);
            if (led1.value >= 255)
                fadeSequenceStep++;
        }
        break;
    case 2:
        if (fadeTimer.isFinished(20))
        {
            fadeTimer.reset();
            led1.setAnalog(led1.value - 5);
            if (led1.value <= 0)
                fadeSequenceStep++;
        }
        break;
    case 3:
        led1.setAnalog(0);
        fadeSequenceStep++;
        break;
    case 4:
        fadeSequenceStep = 0;
        return true;
    }

    return false;
}

/**
 * When a button is pressed the led will flash twice,
 * fade in and out twice, then flash twice again.
 * After the last flash, the led will light up for 5 seconds.
 * When it turns off, the sequence will respond to
 * button presses again. If the button is pressed
 * during the sequence the led will turn off, and the
 * sequence will stop and reset.
 *
 * returns true when the sequence finishes
 */
bool updateMainSequence()
{
    // Reset the sequence if the button is pressed during its execution
    if (button1.pressed && mainSequenceStep != 0)
    {
        Serial.println("Resetting all sequences");
        mainSequenceStep = 0;
        fadeSequenceStep = 0;
        blinkSequenceStep = 0;
        led1.setDigital(false);
        return true;
    }

    switch (mainSequenceStep)
    {
    case 0:
        if (button1.pressed)
            mainSequenceStep++;
        break;
    case 1:
        if (updateBlinkSequence())
            mainSequenceStep++;
        break;
    case 2:
        if (updateBlinkSequence())
            mainSequenceStep++;
        break;
    case 3:
        if (updateFadeSequence())
            mainSequenceStep++;
        break;
    case 4:
        if (updateFadeSequence())
            mainSequenceStep++;
        break;
    case 5:
        if (updateRunBlinkSequenceTimes(2))
            mainSequenceStep++;
        break;
    case 6:
        led1.setDigital(true);
        mainSequenceStep++;
        break;
    case 7:
        mainTimer.reset();
        mainSequenceStep++;
        break;
    case 8:
        if (mainTimer.isFinished(5000))
            mainSequenceStep++;
        break;
    case 9:
        led1.setDigital(false);
        mainSequenceStep++;
        break;
    case 10:
        mainSequenceStep = 0;
        return true;
    }
    return false;
}

void setup()
{
    // Set pin values
    led1.pin = 5;

    button1.pin = 9;
    button1.pulldown = true;

    // Set pin modes
    pinMode(led1.pin, OUTPUT);
    pinMode(button1.pin, INPUT);

    Serial.begin(9600);
}

void loop()
{
    button1.update();

    updateMainSequence();
}
