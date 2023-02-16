int motorCcwPin = 4;
int motorCwPin = 5;
int motorEnablePin = 6;

volatile byte motorState = 0;

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

void setup()
{
    Serial.begin(9600);

    pinMode(motorCcwPin, OUTPUT);
    pinMode(motorCwPin, OUTPUT);
    pinMode(motorEnablePin, OUTPUT);
}

void loop()
{
    // Up
    increaseMotorState();
    updateMotorSpeed();
    delay(1000);

    increaseMotorState();
    updateMotorSpeed();
    delay(1000);

    increaseMotorState();
    updateMotorSpeed();
    delay(1000);

    increaseMotorState();
    updateMotorSpeed();
    delay(1000);

    // Down
    decreaseMotorState();
    updateMotorSpeed();
    delay(1000);

    decreaseMotorState();
    updateMotorSpeed();
    delay(1000);

    decreaseMotorState();
    updateMotorSpeed();
    delay(1000);

    decreaseMotorState();
    updateMotorSpeed();
    delay(1000);
}
