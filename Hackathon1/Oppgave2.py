# Input for å skrive inn passordet
password = input("Enter your password: ")

# Variabler for å holde styr på antall bokstaver, tall og symboler
letters = 0
numbers = 0
symbols = 0

# For-løkke for å gå gjennom hvert tegn i passordet
for i in password: 
    if i.isalpha():
        letters += 1
    elif i.isnumeric():
        numbers += 1
    else:
        symbols += 1

# Printe ut resultatet
print("Letters: ", letters)
print("Numbers: ", numbers)
print("Symbols: ", symbols)