int oledSequenceStep = 0;
unsigned long oledSequenceStartTime = 0;

void printMotorSpeed()
{
    Serial.println("Motor speed");
}

void printActiveTime()
{
    Serial.println("Active time");
}

void updateOledTimer()
{
    unsigned long sequenceStepTime = millis() - oledSequenceStartTime;

    if (sequenceStepTime > 1000)
    {
        oledSequenceStep += 1;
        oledSequenceStartTime = millis();
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

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    updateOledTimer();
    updateOledScreen();
}
