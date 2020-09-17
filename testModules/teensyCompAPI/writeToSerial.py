import serial

ser = serial.Serial('/dev/input/teensy')
print(ser.name)         # check which port was really used
ser.write(b'commandSpeed_2000\n')     # write a string
ser.close()

