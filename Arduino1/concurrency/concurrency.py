# Global variables
state = True
x = 0

# Prints the values from 0 to 255, then loops back to 0
# We do this instead of a for-loop, because we want to do
# something else at the same time, and the for-loop blocks
# other code from running while this is counting
def ramp_up():
    global x
    x += 1
    if x > 255:
        x = 0
    print("Ramp: ", x)

# Prints True and False every other loop
def toggle():
    global state
    print("Toggle: ", state)
    # State = the oposite of the current state
    state = not state

def loop():
    ramp_up()
    toggle()

while True:
    loop()
