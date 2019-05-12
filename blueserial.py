import serial
def sendframe(framedata,comport):
    import time
    ser=serial.Serial(comport,115200,timeout=99999999999999999999,write_timeout=99999999999999999999)
    ser.write(b'I')
    print(str(ser.readline()))

    for i in range(0,9):
        ser.write(bytes(framedata[i*500:500+i*500]))
        print(str(ser.readline()))
        #ser.write(b'C')
    ser.write(bytes(framedata[4500:]))
    print(str(ser.readline()))
    #ser.close()
    #ser=serial.Serial(comport,115200,timeout=99999999999999999999,write_timeout=99999999999999999999)
    #ser.write(b'EEE')#to end the read loop
    print(str(ser.readline()))
    ser.close()

def sereenshow(comport,wordindex,pictype):
    ser=serial.Serial(comport,115200,timeout=99999999999999999999,write_timeout=99999999999999999999)
    ser.write(bytes([83,0,0,wordindex,pictype]))
    print(str(ser.readline()))
    ser.close()
