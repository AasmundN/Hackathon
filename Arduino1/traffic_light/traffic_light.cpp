// Constants
const char RED = 'r';
const char GREEN = 'g';
const char YELLOW = 'y';
const char RED_YELLOW = 's';

// Global variables
char currentState = RED;

// https://www.veygo.com/wp-content/uploads/2021/08/traffic_light_sequence.png
char nextState(char currentState) {
    // Strings can't be used in switch statements
    // therefore we use char or int for the state
    switch(currentState) {
        case RED:
            return RED_YELLOW;
        case RED_YELLOW:
            return GREEN;
        case GREEN:
            return YELLOW;
        case YELLOW:
            return RED;
    }
}

// returns the human-readable name of the state as a string
String stateToString(char state) {
    switch(state) {
        case RED:
            return "Red";
        case RED_YELLOW:
            return "Red and Yellow";
        case GREEN:
            return "Green";
        case YELLOW:
            return "Yellow";
    }
}

void setup() {
    Serial.begin(9600);
}

// The program should print
// r
// Red
// s
// Red and Yellow
// g
// Green
// y
// Yellow
// in a loop
void loop() {
    Serial.println(currentState);
    Serial.println(stateToString(currentState));
    currentState = nextState(currentState);
}
