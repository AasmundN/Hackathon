# Inspired by https://github.com/Ripeey
# https://github.com/Ripeey/Snake/blob/master/snake.py

import time
import tkinter
from typing import Callable

Position2D = tuple[int, int]

game_speed = 100

WINDOW_WIDTH = 200
WINDOW_HEIGHT = 160
PIXELS_PER_GRID_CELL = 20

GRID_WIDTH = WINDOW_WIDTH // PIXELS_PER_GRID_CELL
GRID_HEIGHT = WINDOW_HEIGHT // PIXELS_PER_GRID_CELL

engine = tkinter.Tk()
engine.title("Snake")
engine.configure(bg="black")

window = tkinter.Canvas(engine, bg="black", bd=0, width=WINDOW_WIDTH,
                        height=WINDOW_HEIGHT, highlightthickness=0)

# status_text = window.create_text(
#     WINDOW_WIDTH/2, 20, text="Score : 0", fill="white", font="Times 15 bold")


def make_speed_setter(speed: int) -> Callable[[], None]:
    def set_speed():
        global game_speed
        game_speed = speed
    return set_speed


easy_button = tkinter.Button(
    engine, text="Easy", bg="green", borderwidth=0, command=make_speed_setter(100))
medium_button = tkinter.Button(
    engine, text="Medium", bg="orange", borderwidth=0, command=make_speed_setter(75))
hard_button = tkinter.Button(
    engine, text="Hard", bg="red", borderwidth=0, command=make_speed_setter(50))


def create_labyrinth(on_key_press: Callable[[str], None], setup: Callable, loop: Callable[[], bool]):
    engine.bind('<Key>', on_key_press)

    # easy_button.grid(row=1, column=0, columnspan=2)
    # medium_button.grid(row=1, column=1, columnspan=2)
    # hard_button.grid(row=1, column=2, columnspan=2)
    window.grid(row=1, column=0, columnspan=15, rowspan=10)

    window.update()
    setup()

    def update():
        if loop():
            window.update()
            engine.after(game_speed, update)
        # Uncomment this to auto-close the game when the loop returns False
        else:
            window.update()
            # time.sleep(2)
            # update_status_text("Game closing in 3")
            # window.update()
            # time.sleep(1)
            # update_status_text("Game closing in 2")
            # window.update()
            # time.sleep(1)
            # update_status_text("Game closing in 1")
            # window.update()
            # time.sleep(1)
            engine.destroy()

    engine.after(game_speed, update)


pixels = dict()


def remove_pixel(position: Position2D):
    # Position MUST be integer
    global pixels
    if position in pixels:
        window.delete(pixels[position])
        del pixels[position]


def draw_pixel(position: Position2D, color: str):
    # Position MUST be integer
    # This function handles conversion from window coordinates to grid coordinates
    # and handles logic for creating and removing objects from the grid
    global pixels
    remove_pixel(position)

    width_of_pixel = PIXELS_PER_GRID_CELL

    x, y = position
    # box corner coordinates converted from grid to window coordinates using PIXELS_PER_GRID_CELL
    x1 = x * PIXELS_PER_GRID_CELL
    y1 = y * PIXELS_PER_GRID_CELL
    x2 = x1 + width_of_pixel
    y2 = y1 + width_of_pixel

    pixel = window.create_rectangle(x1, y1, x2, y2, fill=color)

    pixels[position] = pixel


def update_status_text(text: str):
    global window
    global status_text
    window.itemconfig(status_text, text=text)

########################################################
#################### Labyrinth code ####################
########################################################


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
    if player_direction == UP:
        player_direction = LEFT
    elif player_direction == DOWN:
        player_direction = RIGHT
    elif player_direction == RIGHT:
        player_direction = UP
    elif player_direction == LEFT:
        player_direction = DOWN
    window.update()
    time.sleep(move_delay)


def turn_right():
    global player_direction
    if player_direction == UP:
        player_direction = RIGHT
    elif player_direction == DOWN:
        player_direction = LEFT
    elif player_direction == RIGHT:
        player_direction = DOWN
    elif player_direction == LEFT:
        player_direction = UP
    window.update()
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
    if not detect():
        remove_pixel(player_position)
        player_position = get_next_position()
        draw_pixel(player_position, PLAYER_COLOR)
        window.update()
    else:
        print("Could not move because the path is blocked!")
    time.sleep(move_delay)


def detect():
    x, y = get_next_position()
    return LABYRINTH[y][x] == 1


def on_key_press(key: str) -> None:
    # key comes from the following documentation:
    # https://tkdocs.com/shipman/key-names.html

    key_symbol = key.keysym
    # implement keyboard listeners here


def setup() -> None:
    # this function runs once at the start of the program

    for (y, row) in enumerate(LABYRINTH):
        for (x, cell) in enumerate(row):
            if cell == 1:
                draw_pixel((x, y), WALL_COLOR)

    draw_pixel(GOAL_POSITION, GOAL_COLOR)
    draw_pixel(player_position, PLAYER_COLOR)


def loop() -> bool:
    return True


create_labyrinth(on_key_press, setup, loop)
