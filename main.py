from func import *
import blueserial
import time

comport='COM8'
#text1=input('input text1')A
#text2=input('input text2')
image=word_bmpmaker(input())
byteslist=framemaker(image,1,1)
blueserial.sendframe(byteslist,comport)
blueserial.sereenshow(comport,1,0)#不知道为什么必须显示一次才能正确存储
image=word_bmpmaker(input())
byteslist=framemaker(image,1,1)
blueserial.sendframe(byteslist,comport)
#time.sleep(0.05)
blueserial.sereenshow(comport,1,1)
time.sleep(5)
blueserial.sereenshow(comport,1,0)
