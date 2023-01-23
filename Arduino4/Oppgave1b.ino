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

Timer timer1;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if (timer1.isFinished(1000))
    {
        Serial.print("Timer1, Elapsed Time: ");
        Serial.println(timer1.getElapsedTime());
        timer1.reset();
    }
}
