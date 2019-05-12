/**
  ******************************************************************************
  * @file    buff.h
  * @author  Waveshare Team
  * @version V1.0.0
  * @date    23-January-2018
  * @brief   ESP8266 WiFi server.
  *          This file provides firmware functions:
  *           + Sending web page of the tool to a client's browser
  *           + Uploading images from client part by part
  *
  ******************************************************************************
  */ 

/* Size, current position index and byte array of the buffer -----------------*/
#define Buff__SIZE 4742
int16_t     Buff__bufInd;
char    Buff__bufArr[Buff__SIZE];
struct pic_buff
{
    char word_image[Buff__SIZE];
    char chs_image[Buff__SIZE];
};
pic_buff pic_bank[8]={};//待解决内存限制
/* Reads a byte from the buffer at specified position ------------------------*/
int Buff__getByte(int index)
{
    return Buff__bufArr[index];
}

/* Reads a word from the buffer at specified position ------------------------*/
int Buff__getWord(int index)
{
    if (index + 1 >= Buff__SIZE) return -1;
    return Buff__bufArr[index] + (Buff__bufArr[index + 1] << 8);
}

/* Reads a byte from the buffer at specified position ------------------------*/
int Buff__getN3(int index)
{
    return (index + 3 > Buff__SIZE) ? 0 :
        (Buff__bufArr[index    ]      ) + 
        (Buff__bufArr[index + 1] <<  8) + 
        (Buff__bufArr[index + 2] << 16);//返回已发送的长度 arr【4】帧号低八位 arr【5】帧号高八位 arr【3】余数
}

/* Checks if the buffer's data ends with specified string --------------------*/
int Buff__signature(int index, char*str)
{
    // characters of the string to the end of the string
    while (*str != 0)
    {
        // If the correspondent character in the buffer isn't equal
        // to the string's character, return false    
        if (Buff__bufArr[index++] != *str) return false;
        str++;
    }
    
    // Otherwise return true
    return true;
}
void Buff__load(int wordnumber,int wordtype)
{
  if (wordtype==0)
  {
    for (int i=0;i<4742;i++)
    {
      pic_bank[wordnumber].word_image[i]=Buff__bufArr[i];
      //pic_bank[wordnumber].word_image[i]=Buff__bufArr[i];
    }
  }
}
