import numpy as np
from collections import deque
import cv2

image = cv2.imread('shape.jpg')

lower = np.array([0, 0, 0])
upper = np.array([15, 15, 15])
shapeMask = cv2.inRange(image, lower, upper)

# find the contours in the mask
(cnts) = cv2.findContours(shapeMask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
print "I found %d black shapes" % (len(cnts))
cv2.imshow("Mask", shapeMask)
 
# loop over the contours
for c in cnts:
	# draw the contour and show it
	cv2.drawContours(image, [c], -1, (0, 255, 0), 2)
	cv2.imshow("Image", image)
	cv2.waitKey(0)

'''
greenLower = (29, 86, 6)
greenUpper = (64, 255, 255)

hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
'''
