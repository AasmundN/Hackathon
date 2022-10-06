# Inspired by https://github.com/Ripeey
# https://github.com/Ripeey/Snake/blob/master/snake.py

import tkinter

# decrease / increase game speed based on difficulty choice (higher valueswd are slower)
difficulty = input("Select your difficulty [easy/medium/hard]: ")

if(difficulty == "easy"):
    GAME_SPEED = 100
elif(difficulty == "medium"):
    GAME_SPEED = 75
elif(difficulty == "hard"):
    GAME_SPEED = 50


WINDOW_WIDTH = 800
WINDOW_HEIGHT = 500
PIXELS_PER_GRID_CELL = 20

GRID_WIDTH = WINDOW_WIDTH // PIXELS_PER_GRID_CELL
GRID_HEIGHT = WINDOW_HEIGHT // PIXELS_PER_GRID_CELL

engine = tkinter.Tk()
engine.title("Snake")
window = tkinter.Canvas(engine, bg="black", bd=0, width=WINDOW_WIDTH,
                        height=WINDOW_HEIGHT, highlightthickness=0)

status_text = window.create_text(
    WINDOW_WIDTH/2, 20, text="Score : 0", fill="white", font="Times 15 bold")


def start_game(on_key_press, setup, loop):
    engine.bind('<Key>', on_key_press)
    window.pack()
    window.update()
    setup()

    def update():
        if loop():
            window.update()
            engine.after(GAME_SPEED, update)
        else:
            engine.destroy()

    engine.after(GAME_SPEED, update)
    engine.mainloop()


pixels = dict()


def remove_pixel(position):
    # Position MUST be integer
    global pixels
    if position in pixels:
        window.delete(pixels[position])
        del pixels[position]


def draw_pixel(position, color):
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


def update_status_text(text):
    global window
    global status_text
    window.itemconfig(status_text, text=text)
