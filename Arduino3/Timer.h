#ifndef Timer_h
#define Timer_h

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

    bool isFinished(const unsigned long durationMs)
    {
        return getElapsedTime() >= durationMs;
    }
};

#endif

Timer timer1;
Timer timer2;

void setup() {
  Serial.begin(9600);
}

void loop()
{
    if (timer1.isFinished(1000))
    {
        Serial.println("1 second has passed");
        timer1.reset();
    }

    if (timer2.isFinished(2000))
    {
        Serial.println("2 seconds have passed");
        timer2.reset();
    }
}
