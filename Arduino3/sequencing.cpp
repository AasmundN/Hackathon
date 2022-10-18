int step = 0;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    switch (step)
    {
    case 0:
        Serial.println("Step 0");
        step = 1;
        break;
    case 1:
        Serial.println("Step 1");
        step = 2;
        break;
    case 2:
        Serial.println("Step 2");
        step = 0;
        break;
    }
}