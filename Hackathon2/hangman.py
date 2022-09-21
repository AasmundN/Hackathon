def request_word_from_user():
    word = input("Enter a secret word: ")
    # eventually add file reading to get a random from file

    # convert the input to small letters so we only have to check for small letters
    word = word.lower()
    # remove spaces at the beginning and end of the string
    # " a " will be changed to "a"
    word = word.strip()
    return word


def request_letter_from_user():
    # returns (True if the input was valid, the letter if the input was valid)
    # Usage:
    # (input_was_valid, letter) = request_letter_from_user()
    # if input_was_valid:
    #     print(letter)

    user_input = input("Enter a letter: ")

    # behandle gjetningen slik at den er mindre sensitiv
    user_input = user_input.lower()
    user_input = user_input.strip()

    input_was_valid = len(user_input) == 1

    if input_was_valid:
        # now that we know the input is a letter, we can put
        # the value in a more descriptive variable
        letter = user_input
        return (input_was_valid, letter)
    else:
        print("Please enter a single letter")
        return (input_was_valid, "")


def find_letter_indexes_in_word(guess, word):
    # get a list of every index in the word containing the guessed character
    indexes = []
    for i in range(len(word)):
        if word[i] == guess:
            indexes.append(i)
    return indexes


def draw_figure(num_wrong_guesses):
    if num_wrong_guesses == 0:
        print("  +---+")
        print("  |   |")
        print("      |")
        print("      |")
        print("      |")
        print("      |")
        print("=========")
    elif num_wrong_guesses == 1:
        print("  +---+")
        print("  |   |")
        print("  O   |")
        print("      |")
        print("      |")
        print("      |")
        print("=========")
    elif num_wrong_guesses == 2:
        print("  +---+")
        print("  |   |")
        print("  O   |")
        print("  |   |")
        print("      |")
        print("      |")
        print("=========")
    elif num_wrong_guesses == 3:
        print("  +---+")
        print("  |   |")
        print("  O   |")
        print(" /|   |")
        print("      |")
        print("      |")
        print("=========")
    elif num_wrong_guesses == 4:
        print("  +---+")
        print("  |   |")
        print("  O   |")
        print(" /|\  |")
        print("      |")
        print("      |")
        print("=========")
    elif num_wrong_guesses == 5:
        print("  +---+")
        print("  |   |")
        print("  O   |")
        print(" /|\  |")
        print(" /    |")
        print("      |")
        print("=========")
    elif num_wrong_guesses == 6:
        print("  +---+")
        print("  |   |")
        print("  O   |")
        print(" /|\  |")
        print(" / \  |")
        print("      |")
        print("=========")


def evaluate_guess(guess, word, word_outline, list_of_wrong_guesses, max_guesses):
    # Matches the guess to the word and updates the lists

    letter_indexes = find_letter_indexes_in_word(guess, word)
    guess_is_not_in_word = len(letter_indexes) == 0

    if guess_is_not_in_word:
        list_of_wrong_guesses.append(guess)
        print("Wrong guess. You have", max_guesses -
              len(list_of_wrong_guesses), "guesses left")
    else:
        for index in letter_indexes:
            word_outline[index] = guess
        print("Correct guess")

def create_word_outline(word):
    # creates a list of underscores with the same length as the word
    word_outline = []
    for i in range(len(word)):
        if word[i] == " ":
            word_outline.append(" ")
        else:
            word_outline.append("_")
    return word_outline

def has_letter_alredy_been_guessed(guess, list_of_wrong_guesses, word_outline):
    # returns true if the letter has already been guessed
    return (guess in list_of_wrong_guesses) or (guess in word_outline)

def print_word_outline(word_outline_list):
    # https://www.programiz.com/python-programming/methods/string/join
    word_outline_string = "".join(word_outline_list)
    print("Correct guesses: ", word_outline_string)

def print_wrong_guesses(list_of_wrong_guesses):
    wrong_guesses_string = ", ".join(list_of_wrong_guesses)
    print("Wrong guesses: ", wrong_guesses_string)

def is_word_complete(word_outline):
    # returns true if the word is complete
    return "_" not in word_outline

def hangman():
    print("Welcome to Hangman!")
    max_guesses = 6
    word = request_word_from_user()
    wrong_guesses = []
    # correct guesses will contain an underscore _ for each letter
    # the game is won when all underscores are replaced with the correct letter
    word_outline = create_word_outline(word)

    while True:
        (input_was_valid, guess) = request_letter_from_user()

        if not input_was_valid:
            # the input was not valid and we
            # restart the loop to ask for new input
            continue

        if has_letter_alredy_been_guessed(guess, wrong_guesses, word_outline):
            print("You already guessed that letter")
            # restart the loop to ask for new input
            continue

        evaluate_guess(
            # For functions that take many arguments,
            # it is good to put each argument on a new line
            # and name the arguments, rather than just
            # using the correct order
            # name_of_function_argument=name_of_variable
            guess=guess,
            word=word,
            word_outline=word_outline,
            list_of_wrong_guesses=wrong_guesses,
            max_guesses=max_guesses,
        )

        draw_figure(len(wrong_guesses))
        
        print_word_outline(word_outline)
        print_wrong_guesses(wrong_guesses)

        if is_word_complete(word_outline):
            print("You won!")
            break

        exceeded_max_guesses = len(wrong_guesses) == max_guesses
        if exceeded_max_guesses:
            print("You lost!")
            print("The word was", word)
            break


hangman()
