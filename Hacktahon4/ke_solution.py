file = open("num.csv", "r")
content = file.read().split(",")
file.close()
print(isinstance(content[3], str))
content = list(map(int, content))
print(isinstance(content[3], int))
print(len(content))


def run():
    for i in range(100000):
#        print(f"testing {i}")
        if i not in content:
            if i == 0:
                pass
            else:
                print(f"The number is {i}")
                break


if __name__ == '__main__':
    run()
