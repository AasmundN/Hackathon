with open("/Users/aasmundnorsett/Documents/Semester3/Studyass/Hackathon/Hacktahon4/numbers.csv") as file:
    numberList = [int(num) for num in file.read().split(",")]
    totalSum = int(numberList[-1]*(numberList[-1]+1)/2)
    actualSum = sum(numberList)
    print(totalSum-actualSum)