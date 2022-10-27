// https://www.tinkercad.com/things/3W2M7DQEK1V

void printLine(int width, int maxWidth)
{
    int numSpaces = maxWidth - width;
    String output = "";
    for (int x = 0; x < numSpaces; x++)
    {
        output += ' ';
    }
    for (int x = 0; x < width; x++)
    {
        output += String(width);
        output += ' ';
    }
    Serial.println(output);
}

void printPyramid()
{

    int maxWidth = 6;
    for (int x = 0; x < maxWidth; x++)
    {
        printLine(x, maxWidth);
    }
}

void setup()
{
    Serial.begin(9600);
    printPyramid();
}

void loop()
{
}