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
Timer timer2;
Timer timer3;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if (timer1.isFinished(1000))
    {
        Serial.print("Timer 1, Elapsed Time: ");
        Serial.println(timer1.getElapsedTime());
        timer1.reset();
    }

    if (timer2.isFinished(2000))
    {
        Serial.print("Timer 2, Elapsed Time: ");
        Serial.println(timer2.getElapsedTime());
        timer2.reset();
    }

    if (timer3.isFinished(3000))
    {
        Serial.print("Timer 3, Elapsed Time: ");
        Serial.println(timer3.getElapsedTime());
        timer3.reset();
    }
}
