///////////////////////////////////////////////////////
//////////////////// Exercise 1 ///////////////////////
///////////////////////////////////////////////////////

#include<avr/wdt.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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
        while (true) {
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

//////////////////// The actual exercise ///////////////////////

volatile byte motorState = 0;

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

///////////////////////////////////////////////////////
//////////////////// Exercise 2b //////////////////////
///////////////////////////////////////////////////////

const int MOTOR_CCW_PIN = 4;
const int MOTOR_CW_PIN = 5;
const int MOTOR_ENABLE_PIN = 6;

// volatile byte motorState = 0;

void writeMotorSpeed(int speed)
{
    // Set the direction
    digitalWrite(MOTOR_CW_PIN, LOW);
    digitalWrite(MOTOR_CCW_PIN, HIGH);

    // Set the speed
    analogWrite(MOTOR_ENABLE_PIN, speed);
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

///////////////////////////////////////////////////////
//////////////////// Exercise 3 ///////////////////////
///////////////////////////////////////////////////////

const int SPEED_UP_BUTTON_PIN = 2;
const int SPEED_DOWN_BUTTON_PIN = 3;

// volatile byte motorState = 0;
//
// void increaseMotorState()
// {
//     if (motorState < 3)
//     {
//         motorState += 1;
//     }
// }
// 
// void decreaseMotorState()
// {
//     if (motorState > 0)
//     {
//         motorState -= 1;
//     }
// }

void onSpeedUpButtonPressed()
{
    increaseMotorState();
}

void onSpeedDownButtonPressed()
{
    decreaseMotorState();
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

void setup()
{
    Serial.begin(9600);
    
    setupOled();
    
    pinMode(MOTOR_CCW_PIN, OUTPUT);
    pinMode(MOTOR_CW_PIN, OUTPUT);
    pinMode(MOTOR_ENABLE_PIN, OUTPUT);
    
    pinMode(SPEED_UP_BUTTON_PIN, INPUT);
    pinMode(SPEED_DOWN_BUTTON_PIN, INPUT);

    attachInterrupt(digitalPinToInterrupt(SPEED_UP_BUTTON_PIN), onSpeedUpButtonPressed, FALLING);
    attachInterrupt(digitalPinToInterrupt(SPEED_DOWN_BUTTON_PIN), onSpeedDownButtonPressed, FALLING);
}

void loop()
{
    updateMotorSpeed();

    printMotorSpeed();
}