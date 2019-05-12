from PIL import Image, ImageDraw, ImageFont, ImageFilter
import random
import numpy as np
#import blueserial
def bmpmaker(text1,text2):
    width = 296
    height = 128
    image = Image.new('L', (width, height), (255))

    # 创建Font对象:
    font1 = ImageFont.truetype('Arial.ttf', 45)
    font2 = ImageFont.truetype('msyh.ttc', 50)
    # 创建Draw对象:
    draw = ImageDraw.Draw(image)
    for t in range(4):
        draw.text((0,35),text1,font=font1,fill=0)
        draw.text((10,27),text2,font=font2,fill=0)
    image.save('code.bmp', 'bmp')
    return image
def framemaker(image):
    imglst=(np.array(image)/255).T.tolist()
    imgarr=[]
    for i in imglst:
        i.reverse()
        imgarr.extend(i)
    #print(len(imgarr))
    k=0
    decacc=0
    byteslist=[76,134,18,0,0,0]
    for j in range(int(((128*296)/8))):
        for i in [128,64,32,16,8,4,2,1]:
            decacc=decacc+int(imgarr[k])*i
            k+=1
        byteslist.append(decacc)
        decacc=0
    return byteslist

