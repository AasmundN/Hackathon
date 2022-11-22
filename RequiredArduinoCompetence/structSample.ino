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

struct Fader {
    Timer timer;
    int timesFaded;
    int duration = 20;
    // True when fading up, false when fading down
    bool fadingUp = true;

    void reset() {
        timesFaded = 0;
        fadingUp = true;
        timer.reset();
    }

    bool fadeTimes(Led &led, int timesToFade) {
        if (timer.isFinished(duration)) {
            timer.reset();

            if (fadingUp) {
                led.setAnalog(led.value + 10);
                if (led.value >= 255) {
                    fadingUp = false;
                }
            } else {
                led.setAnalog(led.value - 10);
                if (led.value <= 0) {
                    timesFaded++;
                    fadingUp = true;
                }
            }
        }

        if (timesFaded >= timesToFade) {
            reset();
            return true;
        } else {
            return false;
        }
    }
};

struct Blinker {
    Timer timer;
    int timesBlinked;
    int duration = 1000;

    void reset() {
        timesBlinked = 0;
        timer.reset();
    }

    bool blinkTimes(Led &led, int timesToBlink) {
        if (timer.isFinished(duration)) {
            timer.reset();
            led.toggle();
            timesBlinked++;
        }

        if (timesBlinked >= timesToBlink) {
            reset();
            return true;
        } else {
            return false;
        }
    }
};

// Instantiate structs
Led led1;
Button button1;
Timer mainTimer;
Fader fader1;
Blinker blinker1;

// Globals
int mainSequenceStep = 0;

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
        fader1.reset();
        blinker1.reset();
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
        if (blinker1.blinkTimes(led1, 2))
            mainSequenceStep++;
        break;
    case 3:
        if (fader1.fadeTimes(led1, 2))
            mainSequenceStep++;
        break;
    case 5:
        if (blinker1.blinkTimes(led1, 2))
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
