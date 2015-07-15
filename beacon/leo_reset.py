#!/usr/bin/python

# 
#  reset the Arduino Leonardo board
#   open theserial port at 1200bps, then close it.
#  Might have to reset the baud rate back to 57600

import serial

ser = serial.Serial("/dev/ttyACM0",1200)
ser.open()
ser.close()


