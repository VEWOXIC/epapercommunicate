from func import *
import blueserial
import time
comport='COM8'
#text1=input('input text1')
#text2=input('input text2')

#send_new_word('retribution','/ˌrɛtrəˈbjuːʃən/','n.\n惩罚；报应',0,comport)
#time.sleep(2)
send_new_word('spontaneous','/spɑnˈteɪnijəs/','adj.\n自发的；即兴的；非勉强的',0,comport)
'''while True:
    a,b=input().split(',')
    blueserial.screenshow(comport,int(a),int(b))'''