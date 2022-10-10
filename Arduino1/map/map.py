def map(x, in_min, in_max, out_min, out_max):
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

# 5 in the range between 0 and 10 is 50 in the range between 0 and 100
print(map(5, 0, 10, 0, 100))
