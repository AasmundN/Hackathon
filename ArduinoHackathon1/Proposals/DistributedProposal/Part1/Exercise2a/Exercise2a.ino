int motorCcwPin = 4;
int motorCwPin = 5;
int motorEnablePin = 6;

void writeMotorSpeed(int speed)
{
    // Set the direction
    digitalWrite(motorCwPin, LOW);
    digitalWrite(motorCcwPin, HIGH);

    // Set the speed
    analogWrite(motorEnablePin, speed);
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
    writeMotorSpeed(0);
    delay(1000);

    writeMotorSpeed(100);
    delay(1000);

    writeMotorSpeed(200);
    delay(1000);
    
    writeMotorSpeed(255);
    delay(1000);
}
