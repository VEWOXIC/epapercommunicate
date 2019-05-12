def sendframe(framedata):
    import serial
    import time
    ser=serial.Serial('COM8',115200,timeout=99999999999999999999,write_timeout=99999999999999999999)
    ser.write(b'I')
    print(str(ser.readline()))

    for i in range(0,9):
        ser.write(bytes(framedata[i*500:500+i*500]))
        print(str(ser.readline()))
        #ser.write(b'C')
    ser.write(bytes(framedata[4500:]))
    #ser.write(b'E')
    print(str(ser.readline()))
    time.sleep(0.05)
    ser.write(b'S')
    print(str(ser.readline()))
    ser.close()
    ser=serial.Serial('COM8',115200,timeout=99999999999999999999,write_timeout=99999999999999999999)
    ser.write(b'S')
    print(str(ser.readline()))
    ser.close()
