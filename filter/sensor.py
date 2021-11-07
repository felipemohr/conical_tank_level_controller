import serial
from os import remove
from os.path import isfile
from time import time, sleep

def getMillis():
  return int(round(time() * 1000))


if __name__ == "__main__":

  if isfile("raw_data.txt"):
    remove("raw_data.txt")
  
  if isfile("filtered_data.txt"):
    remove("filtered_data.txt")

  sleep(2)

  ser = serial.Serial(port='/dev/ttyUSB0', baudrate=19200)

  with open("raw_data.txt", "a") as raw_file:
    with open("filtered_data.txt", "a") as filtered_file:
      start_time = getMillis()
      while getMillis() - start_time < 3000:
        val = ser.readline().decode().split('/')
        raw_file.write(val[0]+'\n')
        filtered_file.write(val[1])
      raw_file.close()
    filtered_file.close()

  ser.close()
