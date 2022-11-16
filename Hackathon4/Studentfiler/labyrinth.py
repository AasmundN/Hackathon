from labyrinth_utils import turn_left, turn_right, move, detect, set_delay

set_delay(500)

# Example of a solution, that does not work:
# When the robot detects a wall, it turns right
while True:
    if detect():
        turn_right()
    else:
        move()
