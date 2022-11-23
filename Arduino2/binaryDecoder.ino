// Constants
const int LED_1_PIN = 11;
const int LED_2_PIN = 12;
const int LED_3_PIN = 13;

const int SWITCH_1_PIN = 4;
const int SWITCH_2_PIN = 2;

// A 2-bit decoder that displays the decimal value of the binary input using LEDs.
void decodeButtonsToLeds(bool switch1State, bool switch2State)
{
    if (!switch1State && !switch2State)
    {
        digitalWrite(LED_1_PIN, LOW);
        digitalWrite(LED_2_PIN, LOW);
        digitalWrite(LED_3_PIN, LOW);
    }
    else if (!switch1State && switch2State)
    {
        digitalWrite(LED_1_PIN, HIGH);
        digitalWrite(LED_2_PIN, LOW);
        digitalWrite(LED_3_PIN, LOW);
    }
    else if (switch1State && !switch2State)
    {
        digitalWrite(LED_1_PIN, LOW);
        digitalWrite(LED_2_PIN, HIGH);
        digitalWrite(LED_3_PIN, LOW);
    }
    else if (switch1State && switch2State)
    {
        digitalWrite(LED_1_PIN, LOW);
        digitalWrite(LED_2_PIN, LOW);
        digitalWrite(LED_3_PIN, HIGH);
    }
}

void setup()
{
    // Declare the buttons as inputs and the pins as outputs
    pinMode(SWITCH_1_PIN, INPUT);
    pinMode(SWITCH_2_PIN, INPUT);

    pinMode(LED_1_PIN, OUTPUT);
    pinMode(LED_2_PIN, OUTPUT);
    pinMode(LED_3_PIN, OUTPUT);
}

void loop()
{
    // First, read the button states
    bool switch1State = digitalRead(SWITCH_1_PIN);
    bool switch2State = digitalRead(SWITCH_2_PIN);

    decodeButtonsToLeds(switch1State, switch2State);
}
