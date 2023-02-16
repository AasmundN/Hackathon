struct Thermometer
{
    int pin;
    int value = 0;

    // Formula from this tutorial
    // https://www.makerguides.com/tmp36-arduino-tutorial/
    void update()
    {
        // TODO: Temperature math
        value = (analogRead(pin) - 500) / 10;
    }
};
