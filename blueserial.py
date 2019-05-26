import serial
import time
def sendframe(framedata,comport):
    import time
    ser=serial.Serial(comport,115200,timeout=100,write_timeout=1)
    #print(str(ser.readline()))
    ser.write(b'I')
    print(str(ser.readline()))
    for i in range(0,9):
        ser.write(bytes(framedata[i*500:500+i*500]))
        #print(str(ser.readline()))
        
        print("PC:frame send done...")
    ser.write(bytes(framedata[4500:]))
    #print(str(ser.readline()))
    endcode=str(ser.readline(),encoding='utf-8')
    print(endcode)
    ser.close()
    return endcode

def screenshow(comport,wordindex,pictype):
    ser=serial.Serial(comport,115200,timeout=100,write_timeout=1)
    ser.write(b'I')
    print(str(ser.readline()))#每个操作前必须初始化！！！
    ser.write(bytes([83,0,0,wordindex,pictype]))
    print("PC:show done")
    print(str(ser.readline()))
    ser.close()

def clearspiff(comport):
    ser=serial.Serial(comport,115200,timeout=100,write_timeout=1)
    ser.write(b'I')
    print(str(ser.readline()))#每个操作前必须初始化！！！
    ser.write(b'C')
    print("deleting...")
    print(str(ser.readline()))
    ser.close()
def rename(comport,name):
    ser=serial.Serial(comport,115200,timeout=100,write_timeout=1)
    ser.write(bytes('R/'+name,encoding='utf-8')+bytes([0]))
    print(str(ser.readline()))
    ser.close()
def wifi_sync(comport,ssid,password,word_index):
    ser=serial.Serial(comport,115200,timeout=100,write_timeout=1)
    ser.write(bytes('W'+ssid,encoding='utf-8')+bytes([0])+bytes(password,encoding='utf-8')+bytes([0,word_index]))
    #print(bytes('W'+ssid,encoding='utf-8')+bytes([0])+bytes(password,encoding='utf-8')+bytes([0]))
    print(str(ser.readline()))
    ser.close()
def delete_file(comport,name):
    ser=serial.Serial(comport,115200,timeout=100,write_timeout=1)
    ser.write(bytes('D/'+name,encoding='utf-8')+bytes([0]))
    print(str(ser.readline()))
    ser.close()
def get_daily_words(comport):
    ser=serial.Serial(comport,115200,timeout=100,write_timeout=1)
    ser.write(b'G')
    print(str(ser.readline()))
    ser.close()