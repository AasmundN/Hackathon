#include <Servo.h>

Servo min_servo;

int position = 0;

struct ServoStruct
{
    int position = 0;
    int servopin;
    int start = 0;
    int slutt = 180;
    int target = 0;
    unsigned long timerPrevTime = 0;
    bool run = false;
};

ServoStruct servo1;

// & bruker den faktiske globale variabelen i stedet for å kopiere den
void pan(ServoStruct &servo)
{

    if (servo.run)
    {
        if (servo.position == servo.start)
        {
            servo.target = servo.slutt;
        }
        servo.run = false;
    }

    if (servo.position == servo.slutt)
    {
        servo.target = servo.start;
    }

    if ((millis() - servo.timerPrevTime) > 15)
    {
        if (servo.position < servo.target)
        {
            servo.position += 1;
            min_servo.write(servo.position);
        }

        if (servo.position > servo.target)
        {
            servo.position -= 1;
            min_servo.write(servo.position);
        }

        servo.timerPrevTime = millis();
    }
}

void test(ServoStruct servo)
{
    for (int position = servo.start; position <= servo.slutt; position += 1)
    {
        min_servo.write(position);
        delay(15);
    }

    for (int position = servo.slutt; position >= servo.start; position -= 1)
    {
        min_servo.write(position);
        delay(15);
    }
}

void setup()
{
    min_servo.attach(9);

    servo1.servopin = 9;
    pinMode(2, INPUT_PULLUP);

    Serial.begin(9600);
}

void loop()
{
    pan(servo1);
    if (!digitalRead(2))
    {
        servo1.run = true;
    }
}