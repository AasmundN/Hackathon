volatile byte motorState = 0;

unsigned long activeTime = 0;
unsigned long activeTimeStartTime = 0;

void printToDisplay(String message);

void printActiveTime()
{

    String message = "Active time: " + String(activeTime);
    printToDisplay(message);
}

void updateActiveTime()
{
    if (motorState == 0)
    {
        activeTimeStartTime = millis();
    }

    activeTime = millis() - activeTimeStartTime;
}

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    updateActiveTime();

    motorState = 0;
    // motorState = 1;

    Serial.println(activeTime);
}
