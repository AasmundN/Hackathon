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

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if (isFinished(1000))
    {
        Serial.print("Timer1, Elapsed Time: ");
        Serial.println(getElapsedTime());
        reset();
    }
}
