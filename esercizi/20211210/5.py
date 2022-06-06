import turtle
import random
 
def cy(r):
    turtle.setup(400,400)
    turtle.penup()
    turtle.hideturtle()
    turtle.speed(0)
    for i in range(1000):
        x,y = random.randint(-200,200),random.randint(-200,200)
        if (x*x + y*y)**0.5 < r:
            color = "red"
        else:
            color = "blue"
        turtle.goto(x,y)
        turtle.dot(5,color)
 
if __name__ == "__main__":
    r = float(input("scrivi un numero (consigliato da 100 a 200): "))
    print("guarda il risultato della funzione misteriosa")
    cy(r)
    turtle.exitonclick()


