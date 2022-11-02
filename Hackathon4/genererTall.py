from random import randint

antall = 10000
missingNumber = 365
numbers = []

for i in range(antall):
    if i != missingNumber:
        numbers.append(i)
        
#scramble the list
def swap(a, b):
    temp = numbers[a]
    numbers[a] = numbers[b]
    numbers[b] = temp

for i in range(len(numbers)):
    swap(i, randint(0, len(numbers)-1))

print(numbers)

#print to file tall
file = open("tall.txt", "w")
file.write("")
file = open("tall.txt", "a")
for i in range(len(numbers)):
    file.write(str(numbers[i]) + ",")
file.close()