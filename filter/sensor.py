import serial
from os import remove
from os.path import isfile
from time import time, sleep

def getMillis():
  return int(round(time() * 1000))


if __name__ == "__main__":

  if isfile("raw_data.txt"):
    remove("raw_data.txt")

  sleep(2)

  ser = serial.Serial(port='/dev/ttyUSB0', baudrate=9600)

  with open("raw_data.txt", "a") as file:
    start_time = getMillis()
    while getMillis() - start_time < 3000:
      val = str(float(ser.readline())) + '\n'
      file.write(val)
    file.close()

  ser.close()
