import numpy as np
import imutils
import cv2
from matplotlib import pyplot as plt



greenLower = (29, 86, 6)
greenUpper = (64, 255, 255)

# I will want to put the following in a while loop later to do
# tracking live time with a camera feed


image = cv2.imread('ball.jpg')

image = imutils.resize(image, width=600)
hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

# Create a mask for the chosen color
mask = cv2.inRange(hsv, greenLower, greenUpper)
mask = cv2.erode(mask, None, iterations=2)
mask = cv2.dilate(mask, None, iterations=2)


plt.imshow(mask)
plt.show()
