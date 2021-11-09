# Using the PyQt library
from PyQt5.QtCore import *
from PyQt5.QtGui import *
import math

app = QCoreApplication([])

# Load the image
img = QImage(input("Enter name of bitmap: "))

file = open(input("Enter name of output file: "), "w")
file.write("{ 0b")

counter = 0

for i in range(img.height()):
	for j in range(img.width()):
		alpha = qAlpha(img.pixel(j, i)) # Get the pixel and turn it into a QColor
		if(counter >= 8):
			file.write(", 0b")
			counter = 0
		counter += 1
		if(alpha > 128):
			file.write("1")
		else:
			file.write("0")

while counter < 8:
	file.write("0")
	counter += 1

file.write(" };\n")