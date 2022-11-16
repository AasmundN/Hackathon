with open("/numbers.txt") as file:
    content = file.read().split(",")

content = list(map(int, content))

def run():
    for i in range(len(content)+1):
#        print(f"testing {i}")
        if i not in content:
            if i == 0:
                pass
            else:
                print(f"The number is {i}")
                break


run()
