from PIL import Image, ImageDraw, ImageFont, ImageFilter
import random
import numpy as np
import blueserial
import time
def word_bmpmaker(word):
    width = 296
    height = 128
    image = Image.new('L', (width, height), (255))

    # 创建Font对象:
    font1 = ImageFont.truetype('Arial.ttf', 45)
    #font2 = ImageFont.truetype('msyh.ttc', 50)
    # 创建Draw对象:
    draw = ImageDraw.Draw(image)
    length,_=draw.textsize(word,font1)#获取字体图像长度
    #print(length,_)
    draw.text((int((296-length)/2),35),word,font=font1,fill=0)#居中显示

    return image
def chs_bmpmaker(pron,chs):
    width = 296
    height = 128
    image = Image.new('L', (width, height), (255))

    # 创建Font对象:
    font1 = ImageFont.truetype('Arial.ttf', 25)
    font2 = ImageFont.truetype('msyh.ttc', 25)#待排版
    # 创建Draw对象:
    draw = ImageDraw.Draw(image)
    draw.text((5,5),pron,font=font1,fill=0)
    draw.multiline_text((5,29),chs,font=font2,fill=0)
    #image.save('code.bmp', 'bmp')
    return image
def framemaker(image,wordindex,pictype):
    imglst=(np.array(image)/255).T.tolist()
    imgarr=[]
    for i in imglst:
        i.reverse()
        imgarr.extend(i)
    #print(len(imgarr))
    k=0
    decacc=0
    byteslist=[76,134,18,wordindex,pictype,0]#帧头 长度低八 长度高吧 词号 图片（0词1释义）
    for j in range(int(((128*296)/8))):
        for i in [128,64,32,16,8,4,2,1]:
            decacc=decacc+int(imgarr[k])*i
            k+=1
        byteslist.append(decacc)
        decacc=0
    return byteslist
    
def send_new_word(new_word,pron,chs,word_index,comport):
    image=word_bmpmaker(new_word)
    byteslist=framemaker(image,word_index,0)#单词页0
    while True:
        a=blueserial.sendframe(byteslist,comport)
        #print(a)
        if a=='Ok!\n':
            break
    blueserial.screenshow(comport,word_index,0)
    time.sleep(1)
    image=chs_bmpmaker(pron,chs)
    byteslist=framemaker(image,word_index,1)#释义页1
    while True:
        a=blueserial.sendframe(byteslist,comport)
        if a=='Ok!\n':
            break
    #time.sleep(0.05)'''
    blueserial.screenshow(comport,word_index,1)
def send_pic_with_name(image,name,comport):
    byteslist=framemaker(image,0,3)#缓冲页
    while True:
        a=blueserial.sendframe(byteslist,comport)
        #print(a)
        if a=='Ok!\n':
            break
    blueserial.screenshow(comport,0,3)
    blueserial.rename(comport,name)
def init_device(comport):#clear spiffs and send error page
    blueserial.clearspiff(comport)
    width = 296
    height = 128
    image = Image.new('L', (width, height), (255))
    font1 = ImageFont.truetype('Arial.ttf', 30)
    font2 = ImageFont.truetype('malgun.ttf', 20)
    draw = ImageDraw.Draw(image)
    draw.text((5,5),'Error:',font=font1,fill=0)
    draw.text((5,30),'An error has occured.',font=font2,fill=0)
    draw.text((5,53),'Page not Found :(',font=font1,fill=0)
    send_pic_with_name(image,'error',comport)
    send_pic_with_name(word_bmpmaker('Syncing...'),'sync',comport)
    width = 296
    height = 128
    image = Image.new('L', (width, height), (255))
    font1 = ImageFont.truetype('Arial.ttf', 30)
    font2 = ImageFont.truetype('malgun.ttf', 20)
    draw = ImageDraw.Draw(image)
    draw.text((5,5),'Error:',font=font1,fill=0)
    draw.text((5,30),'An error has occured.',font=font2,fill=0)
    draw.text((5,53),'Sync failed. :(',font=font1,fill=0)
    send_pic_with_name(image,'syncf',comport)
    send_pic_with_name(word_bmpmaker('Sync success.'),'syncs',comport)
