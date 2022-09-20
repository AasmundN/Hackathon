#include <Arduino.h>

const int MAX_GUESSES = 6;
const int MAX_WORD_LENGTH = 50;

String input(String message) {
  Serial.println(message);
  while (Serial.available() == 0) {
    // Do nothing while waiting for input
  }
  String text = Serial.readString();
  return text;
}

String requestWordFromUser()
{
  String word = input("Enter a secret word: ");
  // eventually add file reading to get a random from file

  // convert the input to small letters so we only have to check for small letters
  word.toLowerCase();
  // remove spaces at the beginning and end of the string
  // " a " will be changed to "a"
  word.trim();
  return word;
}

char requestLetterFromUser(bool &outInputWasValid)
{
  // returns (True if the input was valid, the letter if the input was valid)
  // Usage:
  // (input_was_valid, letter) = request_letter_from_user()
  // if input_was_valid:
  //     print(letter)

  String userInput = input("Enter a letter: ");

  // behandle gjetningen slik at den er mindre sensitiv
  userInput.toLowerCase();
  userInput.trim();

  outInputWasValid = userInput.length() == 1;

  if (outInputWasValid)
  {
    // now that we know the input is a letter, we can put
    // the value in a more descriptive variable
    char letter = userInput[0];
    return letter;
  }
  else
  {
    Serial.println("Please enter a single letter");
    return ' ';
  }
}

void findLetterPositionsInWord(char guess, String word, int &outNumPositionsFound, int outPositions[])
{
  // get a list of every position (indexes) in the word containing the guessed character
  // for each in range word length
  for (int i = 0; i < word.length(); i++)
  {
    if (word[i] == guess)
    {
      outPositions[outNumPositionsFound] = i;
    }
  }
}

String drawFigure(int numRemainingGuesses)
{
  if (numRemainingGuesses == 6)
  {
    Serial.println("  +---+");
    Serial.println("  |   |");
    Serial.println("      |");
    Serial.println("      |");
    Serial.println("      |");
    Serial.println("      |");
    Serial.println("=========");
  }
  else if (numRemainingGuesses == 5)
  {
    Serial.println("  +---+");
    Serial.println("  |   |");
    Serial.println("  O   |");
    Serial.println("      |");
    Serial.println("      |");
    Serial.println("      |");
    Serial.println("=========");
  }
  else if (numRemainingGuesses == 4)
  {
    Serial.println("  +---+");
    Serial.println("  |   |");
    Serial.println("  O   |");
    Serial.println("  |   |");
    Serial.println("      |");
    Serial.println("      |");
    Serial.println("=========");
  }
  else if (numRemainingGuesses == 3)
  {
    Serial.println("  +---+");
    Serial.println("  |   |");
    Serial.println("  O   |");
    Serial.println(" /|   |");
    Serial.println("      |");
    Serial.println("      |");
    Serial.println("=========");
  }
  else if (numRemainingGuesses == 2)
  {
    Serial.println("  +---+");
    Serial.println("  |   |");
    Serial.println("  O   |");
    Serial.println(" /|\  |");
    Serial.println("      |");
    Serial.println("      |");
    Serial.println("=========");
  }
  else if (numRemainingGuesses == 1)
  {
    Serial.println("  +---+");
    Serial.println("  |   |");
    Serial.println("  O   |");
    Serial.println(" /|\  |");
    Serial.println(" /    |");
    Serial.println("      |");
    Serial.println("=========");
  }
  else if (numRemainingGuesses == 0)
  {
    Serial.println("  +---+");
    Serial.println("  |   |");
    Serial.println("  O   |");
    Serial.println(" /|\  |");
    Serial.println(" / \  |");
    Serial.println("      |");
    Serial.println("=========");
  }
}

void evaluateGuess(
    char guess,
    String word,
    char outListOfCorrectGuesses[],
    char outListOfWrongGuesses[],
    int &outNumWrongGuesses,
    int maxGuesses)
{
  // Matches the guess to the word and updates the lists

  int outLetterPositions[MAX_WORD_LENGTH];
  int outNumPositionsFound = 0;
  findLetterPositionsInWord(guess, word, outNumPositionsFound, outLetterPositions);
  bool guessIsNotInWord = outNumPositionsFound == 0;

  if (guessIsNotInWord)
  {
    outListOfWrongGuesses[outNumWrongGuesses] = guess;
    outListOfWrongGuesses++;
    String remainingGuessesString = String(maxGuesses - outNumWrongGuesses);
    Serial.println("Wrong guess. You have " + remainingGuessesString + " guesses left");
  }
  else
  {
    for (int i = 0; i < outNumPositionsFound; i++)
    {
      int position = outLetterPositions[i];
      outListOfCorrectGuesses[position] = guess;
    }
    Serial.println("Correct guess");
  }
}

// https://www.arduino.cc/reference/en/language/variables/data-types/string/functions/indexof/
bool isLetterInString(char letter, String word)
{
  return word.indexOf(letter) != -1;
}

// https://www.arduino.cc/reference/en/language/variables/data-types/string/functions/indexof/
bool isLetterInCharArray(char letter, char charArray[])
{
  for (int i = 0; i < MAX_WORD_LENGTH; i++)
  {
    if (charArray[i] == letter)
    {
      return true;
    }
  }
  return false;
}

String joinCharArray(char charArray[], int charArraySize, String separator)
{
  String joinedString = "";
  for (int i = 0; i < charArraySize; i++)
  {
    joinedString += charArray[i];
    if (i < charArraySize - 1)
    {
      joinedString += separator;
    }
  }
  return joinedString;
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Welcome to Hangman!");
}

void loop()
{
  String word = requestWordFromUser();
  int outNumWrongGuesses = 0;
  char outWrongGuesses[MAX_GUESSES] = {};
  // correct guesses will contain an underscore _ for each letter
  // the game is won when all underscores are replaced with the correct letter
  char outCorrectGuesses[MAX_WORD_LENGTH] = {};
  for (int i = 0; i < word.length(); i++)
  {
    if (word[i] == ' ')
    {
      outCorrectGuesses[i] = ' ';
    }
    else
    {
      outCorrectGuesses[i] = '_';
    }
  }

  while (true)
  {
    bool outInputWasValid = false;
    char guess = requestLetterFromUser(outInputWasValid);

    if (!outInputWasValid)
    {
      // the input was not valid and we
      // restart the loop to ask for new input
      continue;
    }

    if (isLetterInString(guess, word) || isLetterInCharArray(guess, outCorrectGuesses))
    {
      Serial.println("You already guessed that letter");
      // restart the loop to ask for new input
      continue;
    }

    evaluateGuess(
        guess,
        word,
        outCorrectGuesses,
        outWrongGuesses,
        outNumWrongGuesses,
        MAX_GUESSES);

    drawFigure(MAX_GUESSES - outNumWrongGuesses);

    // https://www.programiz.com/python-programming/methods/string/join
    String correctGuessesString = joinCharArray(outCorrectGuesses, word.length(), "");
    String wrongGuessesString = joinCharArray(outWrongGuesses, outNumWrongGuesses, ", ");

    Serial.println("Correct guesses: " + correctGuessesString);
    Serial.println("Wrong guesses: " + wrongGuessesString);

    bool noMoreEmptyLetters = isLetterInCharArray('_', outCorrectGuesses);
    if (noMoreEmptyLetters)
    {
      Serial.println("You won!");
      break;
    }

    bool exceededMaxGuesses = outNumWrongGuesses == MAX_GUESSES;
    if (exceededMaxGuesses)
    {
      Serial.println("You lost!");
      Serial.println("The word was " + word);
      break;
    }
  }
}
