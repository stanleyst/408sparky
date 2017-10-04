import numpy as np
import imutils
import cv2
from matplotlib import pyplot as plt

# Instantiate the camera
cap = cv2.VideoCapture(0)

greenLower = (29, 86, 6)
greenUpper = (64, 255, 255)

# I will want to put the following in a while loop later to do
# tracking live time with a camera feed

while(True):

    (ret, image) = cap.read()

    if not ret:
        break

    image = imutils.resize(image, width=600)
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

    # Create a mask for the chosen color
    mask = cv2.inRange(hsv, greenLower, greenUpper)
    mask = cv2.erode(mask, None, iterations=2)
    mask = cv2.dilate(mask, None, iterations=2)


#plt.imshow(mask)
#plt.show()

    cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
        cv2.CHAIN_APPROX_SIMPLE)[-2]
    center = None

    if len(cnts) > 0:
        c = max(cnts, key=cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(c)
        M = cv2.moments(c)
        center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

        if radius > 10:
            cv2.circle(image, (int(x), int(y)), int(radius),
                (0, 255, 255), 2)
            cv2.circle(image, center, 5, (0,0,255), -1)



    cv2.imshow('what i see', image)
