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


def find_letter_positions_in_word(guess, word):
    # get a list of every position (indexes) in the word containing the guessed character
    indexes = []
    for i in range(len(word)):
        if word[i] == guess:
            indexes.append(i)
    return indexes


def draw_figure(num_remaining_guesses):
    if num_remaining_guesses == 6:
        print("  +---+")
        print("  |   |")
        print("      |")
        print("      |")
        print("      |")
        print("      |")
        print("=========")
    elif num_remaining_guesses == 5:
        print("  +---+")
        print("  |   |")
        print("  O   |")
        print("      |")
        print("      |")
        print("      |")
        print("=========")
    elif num_remaining_guesses == 4:
        print("  +---+")
        print("  |   |")
        print("  O   |")
        print("  |   |")
        print("      |")
        print("      |")
        print("=========")
    elif num_remaining_guesses == 3:
        print("  +---+")
        print("  |   |")
        print("  O   |")
        print(" /|   |")
        print("      |")
        print("      |")
        print("=========")
    elif num_remaining_guesses == 2:
        print("  +---+")
        print("  |   |")
        print("  O   |")
        print(" /|\  |")
        print("      |")
        print("      |")
        print("=========")
    elif num_remaining_guesses == 1:
        print("  +---+")
        print("  |   |")
        print("  O   |")
        print(" /|\  |")
        print(" /    |")
        print("      |")
        print("=========")
    elif num_remaining_guesses == 0:
        print("  +---+")
        print("  |   |")
        print("  O   |")
        print(" /|\  |")
        print(" / \  |")
        print("      |")
        print("=========")


def evaluate_guess(guess, word, list_of_correct_guesses, list_of_wrong_guesses, max_guesses):
    # Matches the guess to the word and updates the lists

    letter_positions = find_letter_positions_in_word(guess, word)
    guess_is_not_in_word = len(letter_positions) == 0

    if guess_is_not_in_word:
        list_of_wrong_guesses.append(guess)
        print("Wrong guess. You have", max_guesses -
              len(list_of_wrong_guesses), "guesses left")
    else:
        for i in letter_positions:
            list_of_correct_guesses[i] = guess
        print("Correct guess")

def hangman():
    print("Welcome to Hangman!")
    max_guesses = 6
    word = request_word_from_user()
    wrong_guesses = []
    # correct guesses will contain an underscore _ for each letter
    # the game is won when all underscores are replaced with the correct letter
    correct_guesses = []
    for i in range(len(word)):
        if word[i] == " ":
            correct_guesses.append(" ")
        else:
            correct_guesses.append("_")

    while True:
        (input_was_valid, guess) = request_letter_from_user()

        if not input_was_valid:
            # the input was not valid and we
            # restart the loop to ask for new input
            continue

        if (guess in wrong_guesses) or (guess in correct_guesses):
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
            list_of_correct_guesses=correct_guesses,
            list_of_wrong_guesses=wrong_guesses,
            max_guesses=max_guesses,
        )

        draw_figure(max_guesses - len(wrong_guesses))
        
        # https://www.programiz.com/python-programming/methods/string/join
        correct_guesses_string = "".join(correct_guesses)
        wrong_guesses_string = ", ".join(wrong_guesses)
        
        print("Correct guesses: ", correct_guesses_string)
        print("Wrong guesses: ", wrong_guesses_string)

        no_more_empty_letters = "_" not in correct_guesses
        if no_more_empty_letters:
            print("You won!")
            break

        exceeded_max_guesses = len(wrong_guesses) == max_guesses
        if exceeded_max_guesses:
            print("You lost!")
            print("The word was", word)
            break


hangman()
