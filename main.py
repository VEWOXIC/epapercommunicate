from func import *
import blueserial
import time
#text1=input('input text1')
#text2=input('input text2')
wordimage=word_bmpmaker('test2')

byteslist=framemaker(wordimage)
blueserial.load_frame(byteslist)
time.sleep(0.05)
blueserial.screen_show()
