from func import *
import blueserial
#text1=input('input text1')
#text2=input('input text2')
image=bmpmaker('congratulation','')
byteslist=framemaker(image)
blueserial.sendframe(byteslist)