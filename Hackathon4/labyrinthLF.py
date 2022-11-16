from labyrinth_utils import turn_left, turn_right, move, detect, set_delay

set_delay(50)

# Truth table website for right hand rule
# https://andrewyong7338.medium.com/maze-escape-with-wall-following-algorithm-170c35b88e00
while True:
    # Check if there is a wall in front
    front = detect()

    # Check if there is a wall on the right
    turn_right()
    right = detect()

    # Go back to start orientation
    turn_left()

    # Move one step to the right if the right side is clear
    if not right:
        turn_right()
        move()

    # Move one step forward if the front is clear and right side is blocked
    if right and not front:
        move()

    # Turn left if the front and right sides are blocked
    if right and front:
        turn_left()