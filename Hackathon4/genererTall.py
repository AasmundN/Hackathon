from random import randint

antall = 10000
missingNumber = 365
number = []

for i in range(antall):
    if i != missingNumber:
        number.append(i)
        
#scramble the list
def swap(a, b):
    temp = number[a]
    number[a] = number[b]
    number[b] = temp

for i in range(len(number)):
    swap(i, randint(0, len(number)-1))

print(number)

#print to file tall
file = open("tall", "w")
file.write("")
file = open("tall", "a")
for i in range(len(number)):
    file.write(str(number[i]) + ",") 