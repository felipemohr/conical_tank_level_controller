import serial
from os import remove
from os.path import isfile
from time import time, sleep

def getMillis():
  return int(round(time() * 1000))


if __name__ == "__main__":

  if isfile("vazao.txt"):
    remove("vazao.txt")


  sleep(1)

  ser = serial.Serial(port='/dev/ttyACM0', baudrate=9600)

  with open("vazao.txt", "a") as raw_file:
    start_time = getMillis()
    while getMillis() - start_time < 20000:
      try:
        val = ser.readline().decode()
        raw_file.write(val)
      except:
        pass
    raw_file.close()

  ser.close()
