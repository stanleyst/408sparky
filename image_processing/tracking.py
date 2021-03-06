import numpy as np
import time
import imutils
import cv2
from matplotlib import pyplot as plt
import serial
import boto3

#May have to manually add the Keys... just use the ones from the lambda stuff

sqs = boto3.resource('sqs')
queue = sqs.get_queue_by_name(QueueName='Inferno_Command')
client = boto3.client('sqs')
url = queue.url

def pop_message(client, url):
    response = client.receive_message(QueueUrl = url, MaxNumberOfMessage = 1)
    try:
        message = response['Messages'][0]['Body']
        receipt = response['Messages'][0]['ReceiptHandle']
        client.delete_message(QueueUrl = url, ReceiptHandle = receipt)

        return message

    except:

        return "No Messages"


def delay(length):
    start = time.time()

    while time.time() - start < length:
        continue

def tracking(ser):
    # Instantiate the camera
    cap = cv2.VideoCapture(0)

    redLower = (160,30,30)
    redUpper = (255, 180, 180)

    # I will want to put the following in a while loop later to do
    # tracking live time with a camera feed

    while(True):

        command_received = True
        alexa_command = pop_message(client, url)

        if alexa_command == "No Messages":
            command_received = False

        delay(0.25)

        if command_received:

            if alexa_command = "stop":
                stop_running = True
            elif alexa_command = "resume":
                stop_running = False

        (ret, image) = cap.read()

        if not ret:
            break

        image = imutils.resize(image, width=600)
        hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

        # Create a mask for the chosen color
        mask = cv2.inRange(hsv, redLower, redUpper)
        mask = cv2.erode(mask, None, iterations=2)
        mask = cv2.dilate(mask, None, iterations=2)

        cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
            cv2.CHAIN_APPROX_SIMPLE)[-2]
        center = None

        if stop_running:
            ser.write('A')
        elif not stop_running:
            ser.write('B')

        if len(cnts) > 0 and not stop_running and not command_received:
            c = max(cnts, key=cv2.contourArea)
            ((x, y), radius) = cv2.minEnclosingCircle(c)
            M = cv2.moments(c)
            center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

            if radius > 10:
                cv2.circle(image, (int(x), int(y)), int(radius),
                    (0, 255, 255), 2)
                cv2.circle(image, center, 5, (0,0,255), -1)

            lower = 600/4

            if x <= lower:
                ser.write('L')
                print('Going left')
            elif x >= lower*3:
                ser.write('R')
                print('Turn right dumbass')
            else:
                ser.write('S')
                print('AHHHHHHHHHHHHHHHHHHHHHHH')
        #cv2.imshow('frame',image)
        #if cv2.waitKey(1) & 0xFF == ord('q'):
        #    break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':

    connected = False

    ser = serial.Serial('/dev/ttyACM0',9600)

    while not connected:
        serin = ser.read()
        connected = True

    tracking(ser)

    ser.close()
