# -*- coding: utf-8 -*-

import sys
import time

def player_char_from_direction():
    if player_direction == UP:
        return "⯅ "
    if player_direction == DOWN:
        return "⯆ "
    if player_direction == LEFT:
        return "⯇ "
    if player_direction == RIGHT:
        return "⯈ "

def print_canvas():
    output = ""
    for y, row in enumerate(LABYRINTH):
        for x, cell in enumerate(row):
            if (x, y) == GOAL_POSITION:
                output += "⛳" if not player_position == GOAL_POSITION else "✅"
            elif cell == 1:
                output += "⬜"
            elif (x, y) == player_position:
                output += player_char_from_direction()
            elif cell == 0:
                output += "  "
        output += "\n"
    
    print(output, )
    

# Constants
PLAYER_COLOR = "red"
WALL_COLOR = "lightgrey"
GOAL_COLOR = "green"

UP = "UP"
DOWN = "DOWN"
RIGHT = "RIGHT"
LEFT = "LEFT"

GOAL_POSITION = (1, 0)

LABYRINTH = [
    [1, 0, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 1, 1, 1, 1, 1, 1, 0, 1],
    [1, 0, 1, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 1, 0, 1, 1, 1, 1, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
]

move_delay = 0.5

player_position = (8, 1)
player_direction = DOWN
current_labyrinth = 0


def turn_left():
    global player_direction
    print("Turning LEFT")
    if player_direction == UP:
        player_direction = LEFT
    elif player_direction == DOWN:
        player_direction = RIGHT
    elif player_direction == RIGHT:
        player_direction = UP
    elif player_direction == LEFT:
        player_direction = DOWN
    print_canvas()
    time.sleep(move_delay)


def turn_right():
    global player_direction
    print("Turning RIGHT")
    if player_direction == UP:
        player_direction = RIGHT
    elif player_direction == DOWN:
        player_direction = LEFT
    elif player_direction == RIGHT:
        player_direction = DOWN
    elif player_direction == LEFT:
        player_direction = UP
    print_canvas()
    time.sleep(move_delay)


def get_next_position():
    global player_position
    global player_direction

    x, y = player_position

    if player_direction == UP:
        y -= 1
    elif player_direction == DOWN:
        y += 1
    elif player_direction == RIGHT:
        x += 1
    elif player_direction == LEFT:
        x -= 1

    return (x, y)


def move():
    global player_position
    global player_direction
    print("Moving", player_direction)
    if not check_path():
        player_position = get_next_position()
        print_canvas()
    else:
        print("Could not move because the path is blocked!")
    time.sleep(move_delay)
    if player_position == GOAL_POSITION:
        print("###########################")
        print("### You found the goal! ###")
        print("###########################")
        sys.exit("Game Closed")

def check_path():
    # Returns True if the path is blocked
    
    x, y = get_next_position()
    return LABYRINTH[y][x] == 1

def detect():
    # Returns True if the path is blocked
    
    path_is_blocked = check_path()
    print("Detected", "a wall" if path_is_blocked else "nothing")
    return path_is_blocked

print_canvas()
