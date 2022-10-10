// Global variables
bool state = true;
int x = 0;

// Prints the values from 0 to 255, then loops back to 0
// We do this instead of a for-loop, because we want to do
// something else at the same time, and the for-loop blocks
// other code from running while this is counting
void ramp_up() {
    x += 1;
    if (x > 255) {
        x = 0;
    }
    Serial.print("Ramp: ");
    Serial.println(x);
}

// Prints 1 and 0 every other loop
void toggle() {
    Serial.print("Toggle: ");
    Serial.println(state);
    // State = the oposite of the current state
    state = !state;
}

void setup() {
    Serial.begin(9600);
}

void loop() {
    ramp_up();
    toggle();
}
