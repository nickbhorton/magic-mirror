# This was just for printing out the right spheres to put into the input file 
import math
import random


def fibonacci_sphere(samples=1000):

    points = []
    phi = math.pi * (math.sqrt(5.) - 1.)  # golden angle in radians

    for i in range(samples):
        y = 1 - (i / float(samples - 1)) * 2  # y goes from 1 to -1
        radius = math.sqrt(1 - y * y)  # radius at y

        theta = phi * i  # golden angle increment

        x = math.cos(theta) * radius
        z = math.sin(theta) * radius

        points.append((x, y, z))

    return points

# points = fibonacci_sphere(100)
# for p in points:
#     print("sphere ", p[0], p[1], p[2], 0.25, sep=" ")

# points = fibonacci_sphere(10)
# for p in points:
#     print("light ", p[0]/2, p[1]/2, p[2]/2, 1, abs(p[0]), abs(p[1]), abs(p[2]), sep=" ")

def gen_block_of_sphere(width):
    for i in range(0, width):
        for j in range(0, width):
            for k in range(0, width):
                print("sphere", i+0.5,j+0.5,-k+0.5,0.5)


def gen_random_sphere(number):
     for n in range(number):
        x = random.random() * 40
        if x < 0:
             x -= 1
        elif x > 0:
            x += 1
        y = random.random() * 40
        if y < 0:
             y -= 1
        elif y > 0:
            y += 1
        z = random.random() * 40
        if z < 0:
             z -= 1
        elif z > 0:
            z += 1
        print("sphere", x, y, z, 0.5)

gen_block_of_sphere(10)
        