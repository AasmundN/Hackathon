from requests import get
print(sum(range(100001))-sum([int(n) for n in get("/numbers.txt").text.split(",")]))
