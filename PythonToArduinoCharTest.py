'''
Created on 2011-12-02

@author: Bobby Wood
'''

## import the serial library
import serial

## Boolean variable that will represent 
## whether or not the arduino is connected
connected = False

## open the serial port that your ardiono 
## is connected to.
## ser = serial.Serial("COM11", 9600)
ser = serial.Serial("/dev/ttyACM0", 9600)

## loop until the arduino tells us it is ready
while not connected:
    serin = ser.read()
    connected = True

## Tell the arduino to blink!
## ser.write("L\n".encode())

ser.write('L')
ser.write('R')
ser.write('L')
ser.write('S')

## Wait until the arduino tells us it 
## is finished blinking
## while ser.read() == '1':
##    ser.read()
	
while True:
    if ser.read() == '0':
		break

## close the port and end the program
ser.close()