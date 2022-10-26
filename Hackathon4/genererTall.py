from random import randint

antall = 10000  # number of numbers to generate but 0 indexed
missingNumber = 364
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

#clear file tall, and write the numbers to it
open('tall', 'w').close()
with open('tall', 'w') as f:
    for i in range(len(number)):
        f.write(str(number[i])+",")
    f.close()