from PIL import Image, ImageDraw, ImageFont, ImageFilter
import random
import numpy as np
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
    #draw.text((60 * t + 10, 10),, font=font, fill=0)
# 模糊:
#image = image.filter(ImageFilter.BLUR)
img=np.array(image).flatten()
k=0
decacc=''
byteslist=[]
for j in range(int(((128*296)/8))):
    for i in range(8):
        decacc=decacc+str(int(img[k]))
        k=k+1
    hexacc=int(eval('0x'+decacc))
    #byteslike=eval('0x'+str(hexacc))
    byteslist.append(hexacc)
    decacc=''
#img=img.tostring()
print(bytes([255,254,253,232]))
#bytestr=bytes(byteslist)
#print(bytestr,len(bytestr))

image.save('code.bmp', 'bmp')