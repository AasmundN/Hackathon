#include <avr/wdt.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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
    bool prevState = false;
    bool pressed = false;
    bool released = false;

    int debounceTime = 50;
    Timer debounceTimer;

    volatile bool detectedInterrupt = false;

    void updateInterrupt()
    {
        detectedInterrupt = true;
        debounceTimer.reset();
    }

    void update()
    {
        if (!debounceTimer.isFinished(debounceTime))
            return;

        if (!detectedInterrupt)
            return;

        if (pulldown)
            state = digitalRead(pin);
        else
            state = !digitalRead(pin);

        pressed = state && !prevState;
        released = !state && prevState;
        prevState = state;

        detectedInterrupt = false;
    }
};

volatile byte motorState = 0;

int motorCcwPin = 4;
int motorCwPin = 5;
int motorEnablePin = 6;

Button speedUpButton;
Button speedDownButton;

//////////////////// OLED Setup from tutorial ///////////////////////
// https://arduinogetstarted.com/tutorials/arduino-oled

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setupOled()
{
    // initialize OLED display with address 0x3C for 128x64
    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));

        // Enable the watchdog timer so the microcontroller resets after 2 seconds
        wdt_enable(WDTO_2S);
        while (true)
        {
            // Since the conenction failed,
            // wait for the watchdog timer to restart the microcontroller
        }
    }

    delay(2000);         // wait for initializing
    oled.clearDisplay(); // clear display

    oled.setTextSize(1);          // text size
    oled.setTextColor(WHITE);     // text color
    oled.setCursor(0, 10);        // position to display
    oled.println("Starting ..."); // text to display
    oled.display();               // show on OLED
    delay(2000);
}

void printToDisplay(String message)
{
    oled.clearDisplay();
    oled.println(message);
    oled.display();
}

void printMotorSpeed()
{
    String message = "Motor speed: " + String(motorState);
    printToDisplay(message);
}

void writeMotorSpeed(int speed)
{
    // Set the direction
    digitalWrite(motorCwPin, LOW);
    digitalWrite(motorCcwPin, HIGH);

    // Set the speed
    analogWrite(motorEnablePin, speed);
}

void updateMotorSpeed()
{
    switch (motorState)
    {
    // Stopped
    case 0:
        writeMotorSpeed(0);
        break;
    // Slow
    case 1:
        writeMotorSpeed(100);
        break;
    // Medium
    case 2:
        writeMotorSpeed(200);
        break;
    // Fast
    case 3:
        writeMotorSpeed(255);
        break;
    }
}

void increaseMotorState()
{
    if (motorState < 3)
    {
        motorState += 1;
    }
}

void decreaseMotorState()
{
    if (motorState > 0)
    {
        motorState -= 1;
    }
}

void updateMotorSpeedControls()
{
    if (speedUpButton.pressed)
    {
        increaseMotorState();
    }

    if (speedDownButton.pressed)
    {
        decreaseMotorState();
    }
}

void onSpeedUpButtonPressed()
{
    speedUpButton.updateInterrupt();
}

void onSpeedDownButtonPressed()
{
    speedDownButton.updateInterrupt();
}

///////////////////////////////////////////////////////
///////////////////// Extra Logic /////////////////////
///////////////////////////////////////////////////////

Timer buttonHoldTimer;

void updateButtonHolds()
{
    if (!speedUpButton.state && !speedDownButton.state)
    {
        buttonHoldTimer.reset();
    }

    if (buttonHoldTimer.isFinished(1000))
    {
        if (speedUpButton.state)
        {
            increaseMotorState();
        }

        if (speedDownButton.state)
        {
            decreaseMotorState();
        }

        buttonHoldTimer.reset();
    }
}

///////////////////////////////////////////////////////

Timer activeTime;

void printActiveTime()
{

    String message = "Active time: " + String(activeTime.getElapsedTime());
    printToDisplay(message);
}

void updateActiveTime()
{
    if (motorState == 0)
    {
        activeTime.reset();
    }
}

///////////////////////////////////////////////////////

int oledSequenceStep = 0;
Timer oledSequenceTimer;

void updateOledTimer()
{
    if (oledSequenceTimer.isFinished(1000))
    {
        oledSequenceStep += 1;
        oledSequenceTimer.reset();
    }
}

void updateOledScreen()
{

    switch (oledSequenceStep)
    {
    case 0:
        printMotorSpeed();
        break;
    case 1:
        printActiveTime();
        break;
    default:
        oledSequenceStep = 0;
        break;
    }
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

void setup()
{
    Serial.begin(9600);

    setupOled();

    pinMode(motorCcwPin, OUTPUT);
    pinMode(motorCwPin, OUTPUT);
    pinMode(motorEnablePin, OUTPUT);

    speedUpButton.pin = 3;
    speedUpButton.pulldown = true;
    pinMode(speedUpButton.pin, INPUT);

    speedDownButton.pin = 2;
    speedDownButton.pulldown = true;
    pinMode(speedDownButton.pin, INPUT);

    attachInterrupt(digitalPinToInterrupt(speedUpButton.pin), onSpeedUpButtonPressed, FALLING);
    attachInterrupt(digitalPinToInterrupt(speedDownButton.pin), onSpeedDownButtonPressed, FALLING);
}

void loop()
{
    speedUpButton.update();
    speedDownButton.update();

    updateMotorSpeed();
    updateButtonHolds();
    updateActiveTime();
    updateOledTimer();
    updateOledScreen();

    printMotorSpeed();
}
