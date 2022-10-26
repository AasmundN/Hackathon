with open("./numbers.txt") as file:
    numberList = [int(num) for num in file.read().split(",")]
    totalSum = int((len(numberList)+1)*(len(numberList)+2)/2)
    actualSum = sum(numberList)
    print(totalSum-actualSum)
