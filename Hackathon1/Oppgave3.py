# Variabel som holder kontroll på om passordet er gyldig
good = False

# Løkke programmet skal kjøre i
while not good:
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
    
    # Sjekke om passordet er gyldig
    if letters > 2 and numbers > 2 and symbols > 0:
        good = True
        print("Password is good")
    else:
        print("Password is not good")
