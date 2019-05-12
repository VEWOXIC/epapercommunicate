from PIL import Image, ImageDraw, ImageFont, ImageFilter
import random
import numpy as np
import blueserial
#import BytesIO

# 随机字母:
width = 296
height = 128
image = Image.new('L', (width, height), (255))

# 创建Font对象:
font1 = ImageFont.truetype('Arial.ttf', 36)
font2 = ImageFont.truetype('msyh.ttc', 36)
# 创建Draw对象:
draw = ImageDraw.Draw(image)
for t in range(4):
    draw.text((10,10),'[/ˈiːʤəs/]',font=font1,fill=0)
    draw.text((10,50),'嗷嗷嗷',font=font2,fill=0)

imglst=(np.array(image)/255).T.tolist()
imgarr=[]
for i in imglst:
    i.reverse()
    imgarr.extend(i)
print(len(imgarr))
k=0
decacc=0
byteslist=[76,134,18,0,0,0]
for j in range(int(((128*296)/8))):
    for i in [128,64,32,16,8,4,2,1]:
        decacc=decacc+int(imgarr[k])*i
        k+=1
    byteslist.append(decacc)
    decacc=0
blueserial.sendframe(byteslist)
#print(len(byteslist))
image.save('code.bmp', 'bmp')