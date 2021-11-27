import serial
from os import remove
from os.path import isfile
from time import time

def getMillis():
  return int(round(time() * 1000))

if __name__ == "__main__":

  if isfile("system1.csv"):
    remove("system1.csv")

  ser = serial.Serial(port='/dev/ttyUSB0', baudrate=115200)

  with open("system1.csv", "a") as data_file:
    data_file.write("millis,dist_raw,dist_filtered,water_height,setpoint,pid_val\n")
    start_time = getMillis()
    while getMillis() - start_time < 10000:
      data = ser.readline().decode()
      data_file.write(data)
      print(data)

  ser.close()
