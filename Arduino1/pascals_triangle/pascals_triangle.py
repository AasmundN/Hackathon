def print_line(width, max_width):
    num_spaces = max_width-width
    output = ""
    for x in range(num_spaces):
        output += ' '
    for x in range(width):
        output += str(width)
        output += ' '
    print(output)


def print_pyramid():
    max_width = 6
    for x in range(max_width):
        print_line(x, max_width)
        
print_pyramid()
