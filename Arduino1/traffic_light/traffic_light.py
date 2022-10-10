# Constants
RED = 'r'
GREEN = 'g'
YELLOW = 'y'
RED_YELLOW = 's'

# Global variables
current_state = RED

# https://www.veygo.com/wp-content/uploads/2021/08/traffic_light_sequence.png
def next_state(current_state):
    if current_state == RED:
        return RED_YELLOW
    elif current_state == RED_YELLOW:
        return GREEN
    elif current_state == GREEN:
        return YELLOW
    elif current_state == YELLOW:
        return RED

# returns the human-readable name of the state as a string
def state_to_string(state):
    if state == RED:
        return "Red"
    elif state == RED_YELLOW:
        return "Red and Yellow"
    elif state == GREEN:
        return "Green"
    elif state == YELLOW:
        return "Yellow"

# The program should print
# r
# Red
# s
# Red and Yellow
# g
# Green
# y
# Yellow
# in a loop
def loop():
    global current_state
    print(current_state)
    print(state_to_string(current_state))
    current_state = next_state(current_state)

while True:
    loop()
