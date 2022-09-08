# Introduksjon til if/else
passord = input("Skriv inn passord: ")
if passord == "123":
    print("Passordet er riktig")
elif passord == "abc":
    print("Dette er ditt gamle passord!")
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

tall = "123"
bokstaver = "apc"
tallord = "abc123"
# Prøv  ̊a bytte ut hva du tester for  ̊a skje hva som skjer!
if tall.isnumeric():
    print("Strengen inneholder bare tall")
else:
    print("Strengen er ikke bare tall")

if bokstaver.isalpha():
    print("Strengen inneholder bare bokstaver")
else:
    print("Strengen er ikke bare bokstaver")
