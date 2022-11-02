#simpleste forslag i terminalen 
#draw a square
print("__________")
print("|        |")
print("|        |")
print("|        |")
print("__________")


#draw a triangle
print("  /\\")
print(" /  \\")
print("/____\\")

#draw a circle
print("  ___")
print(" /   \\")
print("/     \\")
print("\\     /")
print(" \\___/")


#do not mind the bad code, it is just a quick and dirty solution
import turtle as tr
t = tr.Turtle()
t.hideturtle()
t.speed(0)

t.penup()
t.forward(-200)
t.pendown()

# Draw a square
for i in range(0,4):
    t.forward(100)
    t.left(90)

t.penup()
t.forward(200)
t.pendown()

# Draw a triangle
for i in range(0,3):
    t.forward(100)
    t.left(120)
    
t.penup()
t.forward(300)
t.pendown()

# Draw a circle
for i in range(0,360):
    t.forward(1)
    t.left(1)



