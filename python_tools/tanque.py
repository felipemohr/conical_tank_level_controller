import serial
from os import remove
from os.path import isfile
from time import time, sleep

def getMillis():
  return int(round(time() * 1000))


if __name__ == "__main__":

  if isfile("tanque.txt"):
    remove("tanque.txt")


  # sleep(2)

  ser = serial.Serial(port='/dev/ttyUSB0', baudrate=9600)

  with open("tanque.txt", "a") as raw_file:
    start_time = getMillis()
    while getMillis() - start_time < 240000:
      try:
        val = ser.readline().decode()
        print(val)
        raw_file.write(val)
      except:
        pass
    raw_file.close()

  ser.close()
