from labyrinth_utils import turn_left, turn_right, move, detect

# Example of a solution to the first and second labyrinth, which will not work for the third
while True:
    if detect():
        turn_right()
    else:
        move()
