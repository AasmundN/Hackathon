import os
import json
import random
from snake_utils import draw_pixel, remove_pixel, update_status_text, GRID_WIDTH, GRID_HEIGHT, start_game

# Constants
SNAKE_COLOR = "lightgrey"
FOOD_COLOR = "orange"

SNAKE_COLORS = ["pink", "purple", "lightblue", "lightgreen", "yellow", "white"]

UP = "UP"
DOWN = "DOWN"
RIGHT = "RIGHT"
LEFT = "LEFT"

NUM_STARTING_SNAKE_SEGMENTS = 3

# Custom datatypes
Position2D = tuple[int, int]  # example: (x, y)

# Global variables

# example [(1, 2), (3, 4)]
snake_segments: list[Position2D] = []

# example [(1, 2), (3, 4)]
foods: list[Position2D] = []

snake_movement_direction = RIGHT
next_snake_movement_direction = RIGHT
highscore = 0
score = 0


def create_snake(position, num_snake_segments, snake_segments):

    print("Creating snake")

    # --------- Oppgave 1.1 ----------

    x, y = position
    for i in range(num_snake_segments):
        segment_position = (x + i, y)
        snake_segments.append(segment_position)
        draw_pixel(segment_position, SNAKE_COLOR)

    # --------------------------------


def remove_snake_tail(snake_segments):

    print("Removing snake tail")

    # --------- Oppgave 1.2 ----------

    tail_position = snake_segments.pop(0)
    remove_pixel(tail_position)

    # --------------------------------


def add_new_snake_head(position, snake_segments):

    print("Adding new snake tail")

    # --------- Oppgave 1.3 ----------

    snake_segments.append(position)
    draw_pixel(position, SNAKE_COLOR)

    # --------------------------------


def spawn_food(snake_segments, foods):

    print("Spawning new food")

    # --------- Oppgave 2.1 ----------

    while True:
        # Subtract 1 from the edges to prevent spawning food on the border
        x = random.randint(1, GRID_WIDTH - 1)
        y = random.randint(1, GRID_HEIGHT - 1)
        food_position = (x, y)

        # prevent new food from spawning on top of snake
        if food_position in snake_segments:
            continue

        # prevent new food from spawning on top of existing food
        if food_position in foods:
            continue

        draw_pixel(food_position, FOOD_COLOR)
        foods.append(food_position)
        break

    # --------------------------------


def remove_food(food_position, foods):

    print("Removing food")

    # --------- Oppgave 2.2 ----------

    if food_position in foods:
        foods.remove(food_position)
        remove_pixel(food_position)

    # --------------------------------


def get_next_snake_head_position() -> Position2D:
    # The head is last in the list.
    # -1 is the index of the last element in a python array
    head_x, head_y = snake_segments[-1]

    if snake_movement_direction == UP:
        head_y -= 1
    elif snake_movement_direction == DOWN:
        head_y += 1
    elif snake_movement_direction == LEFT:
        head_x -= 1
    elif snake_movement_direction == RIGHT:
        head_x += 1

    # Move the head to the oposit end of the screen if it goes off screen
    if head_x < 0:
        head_x = GRID_WIDTH - 1

    if head_x > GRID_WIDTH - 1:
        head_x = 0

    if head_y < 0:
        head_y = GRID_HEIGHT - 1

    if head_y > GRID_HEIGHT - 1:
        head_y = 0

    next_snake_head_position = (head_x, head_y)
    return next_snake_head_position


def move_snake(snake_segments: list[Position2D], next_snake_head_position: Position2D) -> None:
    add_new_snake_head(next_snake_head_position, snake_segments)
    remove_snake_tail(snake_segments)


def check_death(snake_segments: list[Position2D], next_snake_head_position: Position2D) -> bool:

    # All the snake segments except the head
    snake_body = snake_segments[:-1]
    if next_snake_head_position in snake_body:
        print("Snake hit itself and died")
        show_game_over()
        return True

    return False


def check_fruit(snake_segments: list[Position2D], foods: list[Position2D], next_snake_head_position: Position2D) -> bool:
    if next_snake_head_position in foods:
        print("Snake found some food")
        update_score(1)

        # Remove the old food and spawn a new random food
        remove_food(next_snake_head_position, foods)
        spawn_food(snake_segments, foods)
        return True

    return False


def on_key_press(key: str) -> None:
    global snake_movement_direction
    global next_snake_movement_direction
    key_symbol = key.keysym
    # The keys can either be wasd or arrow keys
    if key_symbol in ("w", "Up"):
        # Prevent the user from instantly killing the snake
        # by traveling into itself
        # This does not work for very fast input, but it's good enough
        if snake_movement_direction != DOWN:
            next_snake_movement_direction = UP
    elif key_symbol in ("s", "Down"):
        if snake_movement_direction != UP:
            next_snake_movement_direction = DOWN
    elif key_symbol in ("a", "Left"):
        if snake_movement_direction != RIGHT:
            next_snake_movement_direction = LEFT
    elif key_symbol in ("d", "Right"):
        if snake_movement_direction != LEFT:
            next_snake_movement_direction = RIGHT


def read_dict_from_json_file(path: str) -> dict:
    with open(path) as file:
        return json.load(file)


def write_dict_to_json_file(path: str, data: dict) -> None:
    with open(path, "w+") as file:
        json.dump(data, file)


def get_game_file_path() -> str:
    # Get the path of the current python script file and find the folder
    # it belongs to
    current_folder = os.path.dirname(os.path.abspath(__file__))

    # Create a path to the game file within the folder
    game_file_path = os.path.join(current_folder, "game.json")
    return game_file_path


def create_game_file() -> None:
    if not os.path.exists(get_game_file_path()):
        data = {
            "highscore": 0
        }
        write_dict_to_json_file(get_game_file_path(), data)


def save_highscore(highscore: int) -> None:
    write_dict_to_json_file(get_game_file_path(), {"highscore": highscore})


def load_highscore() -> int:
    game_stats = read_dict_from_json_file(get_game_file_path())
    return game_stats["highscore"]


def show_score() -> None:
    global score
    global highscore

    print("Showing score")

    # --------- Oppgave 3.1 ----------

    update_status_text("Score : {}/{}".format(score, highscore))

    # --------------------------------


def show_game_over() -> None:
    global score
    global highscore

    print("Game over")

    # --------- Oppgave 3.2 ----------

    update_status_text("Game Over, Score : {}/{}".format(score, highscore))

    # --------------------------------


def update_score(bonus):
    global score
    global highscore

    print("Updating score")

    # --------- Oppgave 3.3 ----------

    score += bonus
    if score > highscore:
        highscore = score
        save_highscore(highscore)
    show_score()

    # --------------------------------


def setup() -> None:
    # this function runs once at the start of the program

    global snake_segments
    global foods
    global highscore

    create_game_file()
    highscore = load_highscore()
    show_score()

    starting_position = (GRID_WIDTH//2, GRID_HEIGHT//2)

    create_snake(starting_position,
                 NUM_STARTING_SNAKE_SEGMENTS, snake_segments)

    spawn_food(snake_segments, foods)
    spawn_food(snake_segments, foods)
    spawn_food(snake_segments, foods)


def load_next_movement_direction() -> None:
    global snake_movement_direction
    global next_snake_movement_direction
    snake_movement_direction = next_snake_movement_direction


def loop() -> None:
    # This function will be run over and over again as long as it returns True
    # The function should return True if the game should continue, False if the game should end

    global snake_segments
    global foods

    load_next_movement_direction()
    next_snake_head_position = get_next_snake_head_position()

    if check_death(snake_segments, next_snake_head_position):
        print("Game over")
        return False

    if check_fruit(snake_segments, foods, next_snake_head_position):
        # save the position of the last tail segment before moving the snake
        tail_position = snake_segments[0]
        move_snake(snake_segments, next_snake_head_position)
        # Add a new tail segment where the old tail segment was
        snake_segments.insert(0, tail_position)
    else:
        move_snake(snake_segments, next_snake_head_position)

    return True


start_game(on_key_press, setup, loop)
