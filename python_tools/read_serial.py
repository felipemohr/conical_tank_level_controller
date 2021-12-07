import serial
from os import remove
from os.path import isfile
from time import time

def getMillis():
  return int(round(time() * 1000))

if __name__ == "__main__":

  if isfile("system3_data.csv"):
    remove("system3_data.csv")

  ser = serial.Serial(port='/dev/ttyUSB0', baudrate=115200)

  with open("system3_data.csv", "a") as data_file:
    data_file.write("millis,setpoint,flow_raw,flow_filtered\n")
    start_time = getMillis()
    while getMillis() - start_time < 240000:
      data = ser.readline().decode()
      data_file.write(data)
      print(data)

  ser.close()
