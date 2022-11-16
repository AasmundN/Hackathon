# -*- coding: utf-8 -*-
import sys
import time
import random

# Constants
USE_UNICODE = False

PLAYER_COLOR = "red"
WALL_COLOR = "lightgrey"
GOAL_COLOR = "green"

UP = "UP"
DOWN = "DOWN"
RIGHT = "RIGHT"
LEFT = "LEFT"

if USE_UNICODE:
    # UNICODE config
    PLAYER_UP_ICON = "⮝ "
    PLAYER_DOWN_ICON = "⮟ "
    PLAYER_LEFT_ICON = "⮜ "
    PLAYER_RIGHT_ICON = "⮞ "
    GOAL_ICON = "⛳"
    GOAL_REACHED_ICON = "✅"
    WALL_ICON = "⬜"
    EMPTY_ICON = "  "
else:
    # Ascii config
    PLAYER_UP_ICON = "^ "
    PLAYER_DOWN_ICON = "v "
    PLAYER_LEFT_ICON = "< "
    PLAYER_RIGHT_ICON = "> "
    GOAL_ICON = "G "
    GOAL_REACHED_ICON = "G "
    WALL_ICON = "||"
    EMPTY_ICON = "  "

# Indexed by the current labyrinth
GOAL_POSITIONS = [
    (1, 1),
    (4, 4),
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
        [1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
        [1, 0, 0, 1, 0, 0, 0, 1, 0, 1],
        [1, 1, 0, 1, 0, 1, 0, 1, 0, 1],
        [1, 1, 0, 1, 0, 1, 0, 1, 0, 1],
        [1, 1, 0, 1, 0, 1, 0, 1, 0, 1],
        [1, 1, 0, 1, 0, 1, 0, 1, 0, 1],
        [1, 1, 0, 0, 0, 1, 0, 0, 0, 1],
        [1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    ],
    [
        [1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
        [1, 0, 0, 0, 0, 0, 0, 0, 0, 1],
        [1, 0, 1, 1, 1, 1, 1, 1, 0, 1],
        [1, 0, 1, 0, 0, 0, 0, 1, 0, 1],
        [1, 0, 1, 0, 0, 0, 0, 1, 0, 1],
        [1, 0, 1, 1, 1, 0, 1, 1, 0, 1],
        [1, 0, 0, 0, 0, 0, 1, 0, 0, 1],
        [1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    ],
]


def generate_random_labyrinth():
    # example labyrinth with goal at (1, 0) and start at (8, 1)
    
    #create empty labyrinth
    labyrinth = []
    for y in range(8):
        labyrinth.append([1, 1, 1, 1, 1, 1, 1, 1, 1, 1])
    
    #pick a random start position in the middle of the labyrinth
    start = (random.randint(2, 7), random.randint(2, 7))
    #goal position on the edge of the labyrinth
    goal = (random.randint(2, 7), random.randint(2, 7))
    
    #clear the start and goal position
    labyrinth[start[1]][start[0]] = 0
    labyrinth[goal[1]][goal[0]] = 0
    
    #create a random path from start to goal
    current = [start[0], start[1]]
    while current[0] != goal[0] or current[1] != goal[1]:
        #pick a random direction
        direction = random.randint(0, 3)

        ##make it more likely to go in the direction of the goal
        go_to_goal = random.randint(0, 100)
        if go_to_goal > 91:
            if current[0] < goal[0]:
                direction = 0
            elif current[0] > goal[0]:
                direction = 1
            elif current[1] < goal[1]:
                direction = 2
            elif current[1] > goal[1]:
                direction = 3
        
        #move in that direction
        if direction == 0:
            if current[0] < 9:
                current[0] += 1
        elif direction == 1:
            if current[0] > 0:
                current[0] -= 1
        elif direction == 2:
            if current[1] < 7:
                current[1] += 1
        elif direction == 3:
            if current[1] > 0:
                current[1] -= 1
        #clear the path
        labyrinth[current[1]][current[0]] = 0
        
    
    # print the labyrinth
    # print("this levels random labyrinth:\n")
    # for row in labyrinth:
    #     print(str(row) + "\n")
        
    return (labyrinth, start, goal)

def add_random_labyrinth():
    #add random labyrinth to LABYRINTHS
    LABYRINTHS.append(generate_random_labyrinth()[0])
    START_POSITIONS_PLAYER.append(generate_random_labyrinth()[1])
    START_DIRECTIONS_PLAYER.append(DOWN)
    GOAL_POSITIONS.append(generate_random_labyrinth()[2])

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
        return PLAYER_UP_ICON
    if player_direction == DOWN:
        return PLAYER_DOWN_ICON
    if player_direction == LEFT:
        return PLAYER_LEFT_ICON
    if player_direction == RIGHT:
        return PLAYER_RIGHT_ICON

def print_canvas():
    global LABYRINTHS
    global GOAL_POSITIONS
    global player_position
    global current_labyrinth
    
    output = ""
    for y, row in enumerate(LABYRINTHS[current_labyrinth]):
        for x, cell in enumerate(row):
            if (x, y) == GOAL_POSITIONS[current_labyrinth]:
                output += GOAL_ICON if not player_position == GOAL_POSITIONS[current_labyrinth] else GOAL_REACHED_ICON
            elif cell == 1:
                output += WALL_ICON
            elif (x, y) == player_position:
                output += player_char_from_direction()
            elif cell == 0:
                output += EMPTY_ICON
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
    print("Setting delay to", delay, "ms")
    move_delay = delay / 1000
