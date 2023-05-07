import serial
import time
import os

class SerialReader:
  def __init__():
    self.stop = false

port = '/dev/ttyACM0'
baud = 9600
seconds_of_collection = 60

def read_serial(filename=None):
  ser = serial.Serial(port, baud)
  print("Serial port " + port + " opened with Baudrate of " +  str(baud))
  # start = time.time()
  while True:
    if filename:
      f.write(ser.readline().rstrip().decode('utf-8'))
    else:
      print(ser.readline().rstrip().decode('utf-8'))
  ser.cancel_read()
  ser.cancel_write()
  ser.close()
  print("Serial Closed")

read_serial()
