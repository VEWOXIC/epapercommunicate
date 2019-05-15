from func import *
import blueserial
import time
comport='COM8'
#text1=input('input text1')
#text2=input('input text2')
'''
image=word_bmpmaker(input())
byteslist=framemaker(image,0,0)
while True:
    a=blueserial.sendframe(byteslist,comport)
    print(a)
    if a=='Ok!\n':
        break
blueserial.sereenshow(comport,0,0)
image=word_bmpmaker(input())
byteslist=framemaker(image,0,1)
while True:
    a=blueserial.sendframe(byteslist,comport)
    if a=='Ok!\n':
        break
#time.sleep(0.05)
blueserial.sereenshow(comport,0,1)
image=word_bmpmaker(input())
byteslist=framemaker(image,1,0)
while True:
    a=blueserial.sendframe(byteslist,comport)
    if a=='Ok!\n':
        break
blueserial.sereenshow(comport,1,0)
time.sleep(5)
blueserial.sereenshow(comport,0,0)
'''
send_new_word('retribution','/ˌrɛtrəˈbjuːʃən/','n.\n惩罚；报应',0,comport)
send_new_word('spontaneous','/spɑnˈteɪnijəs/','adj.\n自发的；即兴的；非勉强的',1,comport)
while True:
    a,b=input().split(',')
    blueserial.screenshow(comport,int(a),int(b))