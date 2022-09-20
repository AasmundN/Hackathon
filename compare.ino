int number = 1;
float decimalNumber = 1.0;
String text = "Hello World";
char singleLetter = 'a';
int numbers[3] = {1, 2, 3};
float decimalNumbers[3] = {1.0, 2.0, 3.0};
String texts[3] = {"Hello", "World", "!"};
const float aVariableThatNeverChanges = 3.14159;
// Constants are written IN_ALL_UPPERCASE_LETTERS_AND_UNDERSCORES
const int ARRAY_SIZE = 3;
// An array size can never change in C++.
// If you want to use a variable to define the size of an array,
// you must use a constant integer (const int).
int numberArray[ARRAY_SIZE] = {1, 2, 3};

bool thisOrThat = true || false;
bool thisAndThat = true && false;

Serial.println("Print " + "multiple " + "values");

while (true) {
    Serial.println("While loop");
}

for (int i = 0; i < 10; i++) {
    Serial.println(i);
}

int addNumbers(int a, int b) {
  return a + b;
}

float addDecimalNumbers(float a, float b) {
  return a + b;
}

float compareStrings(String a, String b) {
  return a == b
}

void doSomethingWithoutReturning() {
  Serial.println("Hello World");
}

// When & is used between the type and the variable name, it means that the variable is a reference
// When you pass a variable as a reference, the value of the variable can be changed from within the function
// NB! If you do not use &, the value of the variable used as function arguments will not change.
// Like in python, Arrays are always passed by reference.
void returnMultipleValues(int &outNumber, String &outText) {
  outNumber = 1;
  outText = "2";
}
int outNumber = 0;
String outText = "";
returnMultipleValues(outNumber, outText);
Serial.println(outNumber); // Prints: 1
Serial.println(outText); // Prints: 2

void setup() {
    // Arduino serial methods will not work without Serial.begin() in setup
    Serial.begin(9600);
    Serial.println("Runs once");
}

void loop(){
    Serial.println("Runs forever");
}
