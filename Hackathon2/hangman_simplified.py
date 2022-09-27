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
    input_was_valid = user_input.isalpha() and len(user_input) == 1

    if input_was_valid:
        # now that we know the input is a letter, we can put
        # the value in a more descriptive variable
        letter = user_input
        return (input_was_valid, letter)
    else:
        return (input_was_valid, "")

def make_needed_letters_list(word):
    needed_letters = []
    for letter in word:
        if letter not in needed_letters:
            needed_letters.append(letter)
    return needed_letters

def get_word(word, guessed_letters):
    current_word = ""
    for letter in word:
        if letter in guessed_letters:
            current_word += letter
        else:
            current_word += "_"
    return current_word

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


def hangman():
    word = request_word_from_user()
    needed_letters = make_needed_letters_list(word)
    guessed_letters = []
    correct_guesses = []
    num_wrong_guesses = 0

    while True:
        print("You have guessed the following letters: " + str(guessed_letters))
        print("You have " + str(6 - num_wrong_guesses) + " guesses left")
        print(get_word(word, guessed_letters))
        draw_figure(num_wrong_guesses)
        (input_was_valid, letter) = request_letter_from_user()

        if input_was_valid:
            if letter in guessed_letters:
                print("You have already guessed that letter")
            else:
                guessed_letters.append(letter)
                if letter in needed_letters:
                    correct_guesses.append(letter)
                    print("Correct!")
                else:
                    num_wrong_guesses += 1
                    print("Wrong!")
        else:
            print("Please enter a single letter")
        
        if len(correct_guesses) == len(needed_letters):
            print("You won!")
            break
        elif num_wrong_guesses == 6:
            print("You lost!")
            break

hangman()