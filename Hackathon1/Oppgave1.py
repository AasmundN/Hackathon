# Introduksjon til if/else
passord = input("Skriv inn passord: ")
if passord == "123":
    print("Passordet er riktig")
else:
    print("Passordet er feil")

# Introduksjon til for-løkke
for bokstav in "Hello world!":
    print(bokstav)

# Introduksjon til while-løkke
teller = 0
while teller < 10:
    print(teller)
    teller += 1

# Introduksjon til isalpha()
print("abcd".isalpha())
print("abcd123".isalpha())
print("123".isalpha())

# Introduksjon til isnumeric()
print("abcd".isnumeric())
print("abcd123".isnumeric())
print("123".isnumeric())
