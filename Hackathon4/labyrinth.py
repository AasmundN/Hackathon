from labyrinth_utils import turn_left, turn_right, move, detect, set_delay

set_delay(0.5)

# Example of a solution to the first and second labyrinth, which will not work for the third
while True:
    if detect():
        turn_right()
    else:
        move()
