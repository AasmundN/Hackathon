from labyrinth_utils import turn_left, turn_right, move, detect, set_delay

set_delay(50)

# Truth table website for right hand rule https://andrewyong7338.medium.com/maze-escape-with-wall-following-algorithm-170c35b88e00
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