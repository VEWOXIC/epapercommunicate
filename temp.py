import func
import blueserial
a=func.word_bmpmaker('lol')
with open('d:/a.bmp','wb') as f:
    f.write(bytes(a))
#blueserial.sendframe(a,'COM8')