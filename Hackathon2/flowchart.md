```tefcha
start
print "Welcome to Hangman!"
request_word_from_user
create_word_outline

while while True
    request_letter_from_user

    if not input_was_valid
        continue

    if has_letter_alredy_been_guessed
        print "You already guessed that letter"
        continue

    evaluate_guess

    draw_figure

    print_word_outline
    print_wrong_guesses

    if is_word_complete
        print "You won!"
        break

    if exceeded_max_guesses
        print "You lost!"
        print "The word was", word
        break
end

```
