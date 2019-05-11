import serial
ser=serial.Serial('COM8',115200)
ser.write(bytes([255,254]))
print(str(ser.readline()))
ser.close()
