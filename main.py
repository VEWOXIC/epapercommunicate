from func import *
import blueserial
import time
from PIL import Image, ImageDraw, ImageFont, ImageFilter
import numpy as np
comport='COM8'
#text1=input('input text1')
#text2=input('input text2')

#send_new_word('retribution','/ˌrɛtrəˈbjuːʃən/','n.\n惩罚；报应',0,comport)
#time.sleep(2)
#send_new_word('spontaneous','/spɑnˈteɪnijəs/','adj.\n自发的；即兴的；非勉强的',1,comport)
'''while True:
    a,b=input().split(',')
    blueserial.screenshow(comport,int(a),int(b))'''
#init_device(comport)
#time.sleep(2)
blueserial.wifi_sync(comport,'VEWOXIC','Vewoxic16',0)
#time.sleep(3)
blueserial.get_daily_words(comport)
#blueserial.rename(comport,'shit')
#blueserial.delete_file(comport,'error')
#init_device(comport)
#time.sleep(3)
#blueserial.screenshow(comport,16,0)
#send_pic_with_name(word_bmpmaker('Syncing...'),'sync',comport)
'''sync fail pic generate
width = 296
height = 128
image = Image.new('L', (width, height), (255))
font1 = ImageFont.truetype('Arial.ttf', 30)
font2 = ImageFont.truetype('malgun.ttf', 20)
draw = ImageDraw.Draw(image)
draw.text((5,5),'Error:',font=font1,fill=0)
draw.text((5,30),'An error has occured.',font=font2,fill=0)
draw.text((5,53),'Sync failed. :(',font=font1,fill=0)
send_pic_with_name(image,'syncf',comport)'''
#send_pic_with_name(word_bmpmaker('Sync Stage1.'),'sync1',comport)
#send_pic_with_name(word_bmpmaker('Sync Stage2.'),'sync2',comport)



