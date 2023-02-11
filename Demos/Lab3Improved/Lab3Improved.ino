#include <Servo.h>

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

struct Button
{
    int pin;
    // true for pulldown, false for pullup
    bool pulldown;

    bool state = false;
    bool pressed = false;
    bool released = false;

    int debounceTime = 50;
    Timer debounceTimer;

    void setup()
    {
        pinMode(pin, INPUT);
        debounceTimer.reset();
    }

    // Used inside the struct.
    // If you need to know the state, call button.update(); in void loop()
    // and use the variable button.state
    bool readState()
    {
        if (pulldown)
            return digitalRead(pin);
        else
            return !digitalRead(pin);
    }

    void update()
    {
        bool newState = readState();

        if (newState != state)
            debounceTimer.reset();

        if (debounceTimer.isFinished(debounceTime))
        {
            pressed = newState && !state;
            released = !newState && state;
            state = newState;
        }
    }
};

struct ServoStruct
{
    int pin;

    // Delay in ms between position updates
    int speed = 15;
    int start = 0;
    int end = 180;

    Timer timer;
    Servo servo;

    int target = 0;
    int position = 0;
    bool run = false;

    void setup()
    {
        servo.attach(pin);
    }

    void update()
    {
        if (!run)
            return;

        // Sweep towards the target every [speed] ms
        if (timer.isFinished(speed))
        {
            if (position < target)
            {
                position += 1;
                servo.write(position);
            }

            if (position > target)
            {
                position -= 1;
                servo.write(position);
            }

            timer.reset();
        }
    }
};

Button button1;

ServoStruct servo1;
ServoStruct servo2;
ServoStruct servo3;
ServoStruct servo4;
ServoStruct servo5;

void updateServoControl(ServoStruct &servo, Button &button)
{
    // Start the servo when the button is pressed the first time
    if (button.pressed)
    {
        servo.run = true;
    }

    // When the servo is done with a back and forth sweep, wait for the next button press
    // Change button.pressed to button.state to allow for holding the button down to keep the servo moving
    if (button.pressed)
    {
        // Makre servo move to end position when at the start position
        if (servo.position == servo.start)
        {
            servo.target = servo.end;
        }
    }

    // Make servo return to start position when reaching the end position
    if (servo.position == servo.end)
    {
        servo.target = servo.start;
    }
}

void setup()
{
    button1.pin = 2;
    button1.pulldown = true;
    button1.setup();

    servo1.pin = 9;
    servo2.pin = 10;
    servo3.pin = 11;
    servo4.pin = 12;
    servo5.pin = 13;

    servo1.setup();
    servo2.setup();
    servo3.setup();
    servo4.setup();
    servo5.setup();

    Serial.begin(9600);
}

void loop()
{
    updateServoControl(servo1, button1);
    updateServoControl(servo2, button1);
    updateServoControl(servo3, button1);
    updateServoControl(servo4, button1);
    updateServoControl(servo5, button1);

    servo1.update();
    servo2.update();
    servo3.update();
    servo4.update();
    servo5.update();
}
