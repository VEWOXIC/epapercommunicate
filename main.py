from func import *
import blueserial
import time
#text1=input('input text1')
#text2=input('input text2')
image=word_bmpmaker('congratulation')
byteslist=framemaker(image)
blueserial.sendframe(byteslist)
time.sleep(0.05)
#blueserial.sereenshow()#不知道为啥要多一次才能接收到
blueserial.sereenshow()