# Main.py

import cv2
import numpy as np
import os

import DetectChars
import DetectPlates
import PossiblePlate

# Zak - extra imports
import time
import imutils
import serial
import difflib

# Sean - Alexa Interface Imports
import boto3

# Sean - Define the SCARY STUFF
AWS_KEY = 'AKIAIC5RW64OA5WVEIHQ'
SECRET_KEY = '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'
REGION = 'us-east-1'

# SQS Server Stuff...
sqs = boto3.resource('sqs')
queue = sqs.get_queue_by_name(QueueName="Inferno_Command")
client = boto3.client('sqs', aws_access_key_id = AWS_KEY, aws_secret_access_key = SECRET_KEY, region_name = REGION)
url = queue.url

# Function to pop message from the queue
def pop_message(client, url):

    # Get the response from the SQS Server
    response = client.receive_message(QueueUrl = url, MaxNumberOfMessage = 1)

    try:
        message = response['Messages'][0]['Body']
        receipt = response['Messages'][0]['ReceiptHandle']
        client.delete_message(QueueUrl = url, ReceiptHandle = receipt)

        return message

    except:

        return "No Messages"

# module level variables ##########################################################################
SCALAR_BLACK = (0.0, 0.0, 0.0)
SCALAR_WHITE = (255.0, 255.0, 255.0)
SCALAR_YELLOW = (0.0, 255.0, 255.0)
SCALAR_GREEN = (0.0, 255.0, 0.0)
SCALAR_RED = (0.0, 0.0, 255.0)

showSteps = False

###################################################################################################
def main():

    blnKNNTrainingSuccessful = DetectChars.loadKNNDataAndTrainKNN()         # attempt KNN training

    if blnKNNTrainingSuccessful == False:                               # if KNN training was not successful
        print "\nerror: KNN traning was not successful\n"               # show error message
        return                                                          # and exit program
    # end if

    # imgOriginalScene  = cv2.imread("1.png")               # open image
    cap = cv2.VideoCapture(0)
    
    connected = False

    ser = serial.Serial('/dev/ttyACM0',9600)
    
    while not connected:
        serin = ser.read()
        connected = True
    
    while(True):

        # Receive Commands from the SQS Server
        command_received = True
        turn_left = False
        turn_right = False
        alexa_command = pop_message(client, url)

        if alexa_command == "No Messages":
            command_received = False
        
        delay(0.125)

        # Interpret the Alexa Command
        if command_received:
            if alexa_command = "stop":
                stop_running = True
            elif alexa_command = "resume":
                stop_running = False
            elif alexa_command = "turn left":
                turn_left = True
            elif alexa_command = "turn right":
                turn_right = True
        
        (ret, imgOriginalScene) = cap.read()
        
        if not ret:
            print('Shit I broke')
            break

        if imgOriginalScene is None:                            # if image was not read successfully
            print "\nerror: image not read from file \n\n"      # print error message to std out
            os.system("pause")                                  # pause so user can see error message
            return                                              # and exit program
        # end if
        
        # might REMOVE
        imgOriginalScene = imutils.resize(imgOriginalScene, width=600)

        listOfPossiblePlates = DetectPlates.detectPlatesInScene(imgOriginalScene)           # detect plates

        listOfPossiblePlates = DetectChars.detectCharsInPlates(listOfPossiblePlates)        # detect chars in plates

        # cv2.imshow("imgOriginalScene", imgOriginalScene)            # show scene image

        if len(listOfPossiblePlates) == 0:                          # if no plates were found
            print "\nno license plates were detected\n"             # inform user no plates were found
        else:                                                       # else
                    # if we get in here list of possible plates has at leat one plate

                    # sort the list of possible plates in DESCENDING order (most number of chars to least number of chars)
            listOfPossiblePlates.sort(key = lambda possiblePlate: len(possiblePlate.strChars), reverse = True)

                    # suppose the plate with the most recognized chars (the first plate in sorted by string length descending order) is the actual plate
            licPlate = listOfPossiblePlates[0]

            # cv2.imshow("imgPlate", licPlate.imgPlate)           # show crop of plate and threshold of plate
            # cv2.imshow("imgThresh", licPlate.imgThresh)

            if len(licPlate.strChars) == 0:                     # if no chars were found in the plate
                print "\nno characters were detected\n\n"       # show message
                # return                                          # and exit program
            # end if

            # drawRedRectangleAroundPlate(imgOriginalScene, licPlate)             # draw red rectangle around plate

            print "\nlicense plate read from image = " + licPlate.strChars + "\n"       # write license plate text to std out
            print "----------------------------------------"

            percentMatch = difflib.SequenceMatcher(None, licPlate.strChars, "GLEN239V")
            print "\n Character ratio = "
            print percentMatch.ratio()
            print "\n"
            
            # Get horizontal center of letters
            ( (intPlateCenterX, intPlateCenterY), (intPlateWidth, intPlateHeight), fltCorrectionAngleInDeg ) = licPlate.rrLocationOfPlateInScene
            letterCenter = intPlateCenterX

            # Interpret and Perform the Alexa Command
            if stop_running:
                ser.write('A')
            elif not stop_running:
                ser.write('B')

            elif turn_left:
                ser.write('C')

            elif turn_right:
                ser.write('D')

            lower = 150
            upper = 450

            if percentMatch.ratio() > 0.2 and not stop_running and not command_received:
                if letterCenter <= lower:
                    ser.write('L')
                    print('Going left')
                elif letterCenter >= upper:
                    ser.write('R')
                    print('Turn right dumbass')
                else:
                    ser.write('S')
                    print('AHHHHHHHHHHHHHHHHHHHHHHH')
            
            # writeLicensePlateCharsOnImage(imgOriginalScene, licPlate)           # write license plate text on the image

            # cv2.imshow("imgOriginalScene", imgOriginalScene)                # re-show scene image

            # cv2.imwrite("imgOriginalScene.png", imgOriginalScene)           # write image out to file

    # end if else

    # cv2.waitKey(0)                    # hold windows open until user presses a key
    cap.release()
    cv2.destroyAllWindows()

    return
# end main

###################################################################################################
def drawRedRectangleAroundPlate(imgOriginalScene, licPlate):

    p2fRectPoints = cv2.boxPoints(licPlate.rrLocationOfPlateInScene)            # get 4 vertices of rotated rect

    cv2.line(imgOriginalScene, tuple(p2fRectPoints[0]), tuple(p2fRectPoints[1]), SCALAR_RED, 2)         # draw 4 red lines
    cv2.line(imgOriginalScene, tuple(p2fRectPoints[1]), tuple(p2fRectPoints[2]), SCALAR_RED, 2)
    cv2.line(imgOriginalScene, tuple(p2fRectPoints[2]), tuple(p2fRectPoints[3]), SCALAR_RED, 2)
    cv2.line(imgOriginalScene, tuple(p2fRectPoints[3]), tuple(p2fRectPoints[0]), SCALAR_RED, 2)
# end function

def getLettersLocation(licPlate):
    
    p2fRectPoints = cv2.boxPoints(licPlate.rrLocationOfPlateInScene)            # get 4 vertices of rotated rect
    
    letterCenter = (p2fRectPoints[0]) - (p2fRectPoints[1])
    
    return letterCenter

###################################################################################################
def writeLicensePlateCharsOnImage(imgOriginalScene, licPlate):
    ptCenterOfTextAreaX = 0                             # this will be the center of the area the text will be written to
    ptCenterOfTextAreaY = 0

    ptLowerLeftTextOriginX = 0                          # this will be the bottom left of the area that the text will be written to
    ptLowerLeftTextOriginY = 0

    sceneHeight, sceneWidth, sceneNumChannels = imgOriginalScene.shape
    plateHeight, plateWidth, plateNumChannels = licPlate.imgPlate.shape

    intFontFace = cv2.FONT_HERSHEY_SIMPLEX                      # choose a plain jane font
    fltFontScale = float(plateHeight) / 30.0                    # base font scale on height of plate area
    intFontThickness = int(round(fltFontScale * 1.5))           # base font thickness on font scale

    textSize, baseline = cv2.getTextSize(licPlate.strChars, intFontFace, fltFontScale, intFontThickness)        # call getTextSize

            # unpack roatated rect into center point, width and height, and angle
    ( (intPlateCenterX, intPlateCenterY), (intPlateWidth, intPlateHeight), fltCorrectionAngleInDeg ) = licPlate.rrLocationOfPlateInScene

    intPlateCenterX = int(intPlateCenterX)              # make sure center is an integer
    intPlateCenterY = int(intPlateCenterY)

    ptCenterOfTextAreaX = int(intPlateCenterX)         # the horizontal location of the text area is the same as the plate

    if intPlateCenterY < (sceneHeight * 0.75):                                                  # if the license plate is in the upper 3/4 of the image
        ptCenterOfTextAreaY = int(round(intPlateCenterY)) + int(round(plateHeight * 1.6))      # write the chars in below the plate
    else:                                                                                       # else if the license plate is in the lower 1/4 of the image
        ptCenterOfTextAreaY = int(round(intPlateCenterY)) - int(round(plateHeight * 1.6))      # write the chars in above the plate
    # end if

    textSizeWidth, textSizeHeight = textSize                # unpack text size width and height

    ptLowerLeftTextOriginX = int(ptCenterOfTextAreaX - (textSizeWidth / 2))           # calculate the lower left origin of the text area
    ptLowerLeftTextOriginY = int(ptCenterOfTextAreaY + (textSizeHeight / 2))          # based on the text area center, width, and height

            # write the text on the image
    cv2.putText(imgOriginalScene, licPlate.strChars, (ptLowerLeftTextOriginX, ptLowerLeftTextOriginY), intFontFace, fltFontScale, SCALAR_YELLOW, intFontThickness)
# end function

###################################################################################################

###################################################################################################
def delay(length):
    start = time.time()

    while time.time() - start < length:
        continue
# end function

###################################################################################################


if __name__ == "__main__":
    main()


















