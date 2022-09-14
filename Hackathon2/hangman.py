def getWord():
    word = input("Enter a secret word: ")
    #eventually add file reading to get a random from file

    #behandle ordet slik at det er mindre sensitivt
    word = word.lower()
    word = word.strip()
    # word = word.replace(" ", "")
    return word

def getGuess():
    while True:
        guess = input("Enter a letter: ")
        if len(guess) == 1:
            return guess
        else:
            print("Please enter a single letter")

def checkGuess(guess, word):
    #get a list of indexes where the guess is in the word
    indexes = []
    for i in range(len(word)):
        if word[i] == guess:
            indexes.append(i)
    return indexes

def hangmann():
    print("Welcome to Hangman!")
    word = getWord()
    wrongGuesses = []
    correctGuesses = []
    for i in range(len(word)):
        correctGuesses.append("_")
    
    while True:
        guess = getGuess()

        if guess in wrongGuesses or guess in correctGuesses:
            print("You already guessed that letter")
            continue

        indexes = checkGuess(guess, word)
        if indexes == []:
            wrongGuesses.append(guess)
            print("Wrong guess. You have", 6 - len(wrongGuesses), "guesses left")
        else:
            for i in indexes:
                correctGuesses[i] = guess
            print("Correct guess")
            
        print("Correct guesses: ", correctGuesses)
        print("Wrong guesses: ", wrongGuesses)
        if "_" not in correctGuesses:
            print("You won!")
            break
        if len(wrongGuesses) == 6:
            print("You lost!")
            break

hangmann()