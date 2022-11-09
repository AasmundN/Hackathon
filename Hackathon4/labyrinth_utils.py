# -*- coding: utf-8 -*-

import sys
import time

# Constants
PLAYER_COLOR = "red"
WALL_COLOR = "lightgrey"
GOAL_COLOR = "green"

UP = "UP"
DOWN = "DOWN"
RIGHT = "RIGHT"
LEFT = "LEFT"

# Indexed by the current labyrinth
GOAL_POSITIONS = [
    (1, 0),
    (1, 0),
]

# Indexed by the current labyrinth
START_POSITIONS_PLAYER = [
    (8, 1),
    (8, 1),
]

# Indexed by the current labyrinth
START_DIRECTIONS_PLAYER = [
    DOWN,
    DOWN,
]

LABYRINTHS = [
    [
        [1, 0, 1, 1, 1, 1, 1, 1, 1, 1],
        [1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
        [1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
        [1, 0, 1, 1, 1, 1, 1, 1, 0, 1],
        [1, 0, 1, 0, 0, 0, 0, 0, 0, 1],
        [1, 0, 1, 0, 1, 1, 1, 1, 0, 1],
        [1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
        [1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    ],
    [
        [1, 0, 1, 1, 1, 1, 1, 1, 1, 1],
        [1, 0, 1, 1, 1, 1, 1, 1, 0, 1],
        [1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
        [1, 0, 1, 1, 1, 1, 1, 1, 0, 1],
        [1, 0, 1, 0, 0, 0, 0, 0, 0, 1],
        [1, 0, 1, 0, 1, 1, 1, 1, 0, 1],
        [1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
        [1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    ],
]

# Global Variables

move_delay = 0.5

current_labyrinth = 0

player_position = START_POSITIONS_PLAYER[current_labyrinth]
player_direction = START_DIRECTIONS_PLAYER[current_labyrinth]

def silent_sleep(duration):
    try:
        time.sleep(duration)
    except KeyboardInterrupt:
        sys.exit("Game Closed")

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
    global LABYRINTHS
    global GOAL_POSITIONS
    global player_position
    global current_labyrinth
    
    output = ""
    for y, row in enumerate(LABYRINTHS[current_labyrinth]):
        for x, cell in enumerate(row):
            if (x, y) == GOAL_POSITIONS[current_labyrinth]:
                output += "⛳" if not player_position == GOAL_POSITIONS[current_labyrinth] else "✅"
            elif cell == 1:
                output += "⬜"
            elif (x, y) == player_position:
                output += player_char_from_direction()
            elif cell == 0:
                output += "  "
        output += "\n"
    
    print(output, )
    
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

def is_out_of_bounds(position):
    # Returns True if the position is out of the labyrinth
    
    global LABYRINTHS
    global current_labyrinth
    x, y = position
    
    if x < 0 or x >= len(LABYRINTHS[current_labyrinth][0]):
        return True
    
    if y < 0 or y >= len(LABYRINTHS[current_labyrinth]):
        return True
    
    return False


def check_path():
    # Returns True if the path is blocked
    global LABYRINTHS
    global current_labyrinth
    
    next_position = get_next_position()
    x, y = next_position
    
    if is_out_of_bounds(next_position):
        return True
    
    return LABYRINTHS[current_labyrinth][y][x] == 1

print_canvas()

###########################
### Available functions ###
###########################

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
    silent_sleep(move_delay)

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
    silent_sleep(move_delay)

def detect():
    # Returns True if the path is blocked
    
    path_is_blocked = check_path()
    print("Detected", "a wall" if path_is_blocked else "nothing")
    return path_is_blocked

def move():
    global LABYRINTHS
    global GOAL_POSITIONS
    global player_position
    global player_direction
    global current_labyrinth
    global START_POSITIONS_PLAYER    
    global START_DIRECTIONS_PLAYER

    print("Moving", player_direction)
    if not check_path():
        player_position = get_next_position()
        print_canvas()
    else:
        print("Could not move because the path is blocked!")
    silent_sleep(move_delay)
    if player_position == GOAL_POSITIONS[current_labyrinth]:
        current_labyrinth += 1
        if current_labyrinth == len(LABYRINTHS):
            print("###########################")
            print("###  You won the game!  ###")
            print("###########################")
            sys.exit("Game Closed")
        else:
            print("###########################")
            print("### You found the goal! ###")
            print("###########################")
            print("Moving to labyrinth", current_labyrinth + 1)
            player_position = START_POSITIONS_PLAYER[current_labyrinth]
            player_direction = START_DIRECTIONS_PLAYER[current_labyrinth]

def set_delay(delay):
    # Sets the delay in seconds after each move action

    global move_delay
    print("Setting delay to", delay)
    move_delay = delay
