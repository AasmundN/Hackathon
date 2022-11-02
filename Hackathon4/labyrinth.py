from labyrinth_utils import turn_left, turn_right, move, detect

while True:
    if detect():
        turn_right()
    else:
        move()
