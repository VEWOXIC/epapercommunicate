from func import *
import blueserial
import time
comport='COM8'
#text1=input('input text1')
#text2=input('input text2')
'''
image=word_bmpmaker(input())
byteslist=framemaker(image,1,0)
blueserial.sendframe(byteslist,comport)
image=word_bmpmaker(input())
byteslist=framemaker(image,1,1)
blueserial.sendframe(byteslist,comport)
#time.sleep(0.05)'''
blueserial.sereenshow(comport,1,0)
#time.sleep(10)
#blueserial.sereenshow(comport,1,1)