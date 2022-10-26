content = open('tall').read()
num = content.split(",")

for i in range(len(num)):
    if num[i] != "":        
        num[i] = int(num[i])
    else:
        num.pop(i)
        
num.sort()
print(num)
for i in range(len(num)):
    if num[i] != i:
        print("Missing number: " + str(i))
        break