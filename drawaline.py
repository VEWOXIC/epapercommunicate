from PIL import Image, ImageDraw, ImageFont, ImageFilter
import random
import numpy as np
#import BytesIO

# 随机字母:
def rndChar():
    return chr(random.randint(65, 90))

width = 128
height = 296
image = Image.new('L', (width, height), (255))
'''
# 创建Font对象:
font1 = ImageFont.truetype('Arial.ttf', 36)
font2 = ImageFont.truetype('msyh.ttc', 36)
# 创建Draw对象:
draw = ImageDraw.Draw(image)
for t in range(4):
    draw.text((10,10),'[/ˈiːʤəs/]',font=font1,fill=0).rotate(270)
    draw.text((10,50),'嗷嗷嗷',font=font2,fill=0).rotate(270)
'''
draw = ImageDraw.Draw(image)
draw.line((0,0,0,296),fill=0)
image.save('code.bmp', 'bmp')