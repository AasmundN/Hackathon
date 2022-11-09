from labyrinth_utils import turn_left, turn_right, move, detect, set_delay

set_delay(0.1)

# Example of a solution to the first and second labyrinth, which will not work for the third
while True:
    front = detect()
    turn_right()
    right = detect()
    turn_left()

    if not right:
        turn_right()
        move()

    if right and not front:
        move()

    if right and front:
        turn_left()