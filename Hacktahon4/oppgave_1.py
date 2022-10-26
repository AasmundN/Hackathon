with open("numbers.csv") as file:
    numberList = [int(num) for num in file.read().split(",")]
    totalSum = int(numberList[-1]*(numberList[-1]+1)/2)
    actualSum = sum(numberList)
    print(totalSum-actualSum)

