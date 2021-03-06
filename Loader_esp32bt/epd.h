/**
  ******************************************************************************
  * @file    epd.h
  * @author  Waveshare Team
  * @version V1.0.0
  * @date    23-January-2018
  * @brief   This file provides e-Paper driver functions
  *           void EPD_SendCommand(byte command);
  *           void EPD_SendData(byte data);
  *           void EPD_WaitUntilIdle();
  *           void EPD_Send_1(byte c, byte v1);
  *           void EPD_Send_2(byte c, byte v1, byte v2);
  *           void EPD_Send_3(byte c, byte v1, byte v2, byte v3);
  *           void EPD_Send_4(byte c, byte v1, byte v2, byte v3, byte v4);
  *           void EPD_Send_5(byte c, byte v1, byte v2, byte v3, byte v4, byte v5);
  *           void EPD_Reset();
  *           void EPD_dispInit();
  *           
  *          varualbes:
  *           EPD_dispLoad;                - pointer on current loading function
  *           EPD_dispIndex;               - index of current e-Paper
  *           EPD_dispInfo EPD_dispMass[]; - array of e-Paper properties
  *           
  ******************************************************************************
  */
/* SPI pin definition --------------------------------------------------------*/
#define PIN_SPI_SCK  13
#define PIN_SPI_DIN  14
#define PIN_SPI_CS   15
#define PIN_SPI_BUSY 25//19
#define PIN_SPI_RST  26//21
#define PIN_SPI_DC   27//22

/* Pin level definition ------------------------------------------------------*/
#define LOW             0
#define HIGH            1

#define GPIO_PIN_SET   1
#define GPIO_PIN_RESET 0
void EPD_initSPI()
{
    //Serial.println(SPI._spi_num);
    //Serial.println(SPI.get);

    pinMode(PIN_SPI_BUSY,  INPUT);
    pinMode(PIN_SPI_RST , OUTPUT);
    pinMode(PIN_SPI_DC  , OUTPUT);
    
    pinMode(PIN_SPI_SCK, OUTPUT);
    pinMode(PIN_SPI_DIN, OUTPUT);
    pinMode(PIN_SPI_CS , OUTPUT);

    digitalWrite(PIN_SPI_CS , HIGH);
    digitalWrite(PIN_SPI_SCK, LOW);
    //SPI.begin(); 
}

/* Lut mono ------------------------------------------------------------------*/
byte lut_full_mono[] =
{
    0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22, 
    0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88, 
    0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51, 
    0x35, 0x51, 0x51, 0x19, 0x01, 0x00
};



/* The procedure of sending a byte to e-Paper by SPI -------------------------*/
void EpdSpiTransferCallback(byte data) 
{
    //SPI.beginTransaction(spi_settings);
    digitalWrite(PIN_SPI_CS, GPIO_PIN_RESET);

    for (int i = 0; i < 8; i++)
    {
        if ((data & 0x80) == 0) digitalWrite(PIN_SPI_DIN, GPIO_PIN_RESET); 
        else                    digitalWrite(PIN_SPI_DIN, GPIO_PIN_SET);

        data <<= 1;
        digitalWrite(PIN_SPI_SCK, GPIO_PIN_SET);     
        digitalWrite(PIN_SPI_SCK, GPIO_PIN_RESET);
    }

    //SPI.transfer(data);
    digitalWrite(PIN_SPI_CS, GPIO_PIN_SET);
    //SPI.endTransaction();
}

/* Sending a byte as a command -----------------------------------------------*/
void EPD_SendCommand(byte command) 
{
    digitalWrite(PIN_SPI_DC, LOW);
    EpdSpiTransferCallback(command);
}

/* Sending a byte as a data --------------------------------------------------*/
void EPD_SendData(byte data) 
{
    digitalWrite(PIN_SPI_DC, HIGH);
    EpdSpiTransferCallback(data);
}


/* Waiting the e-Paper is ready for further instructions ---------------------*/
void EPD_WaitUntilIdle() 
{
    //0: busy, 1: idle
    while(digitalRead(PIN_SPI_BUSY) == 0) delay(100);  

}

/* Send a one-argument command -----------------------------------------------*/
void EPD_Send_1(byte c, byte v1)
{
    EPD_SendCommand(c);
    EPD_SendData(v1);
}

/* Send a two-arguments command ----------------------------------------------*/
void EPD_Send_2(byte c, byte v1, byte v2)
{
    EPD_SendCommand(c);
    EPD_SendData(v1);
    EPD_SendData(v2);
}

/* Send a three-arguments command --------------------------------------------*/
void EPD_Send_3(byte c, byte v1, byte v2, byte v3)
{
    EPD_SendCommand(c);
    EPD_SendData(v1);
    EPD_SendData(v2);
    EPD_SendData(v3);
}

/* Send a four-arguments command ---------------------------------------------*/
void EPD_Send_4(byte c, byte v1, byte v2, byte v3, byte v4)
{
    EPD_SendCommand(c);
    EPD_SendData(v1);
    EPD_SendData(v2);
    EPD_SendData(v3);
    EPD_SendData(v4);
}

/* Send a five-arguments command ---------------------------------------------*/


/* Writting lut-data into the e-Paper ----------------------------------------*/
void EPD_lut(byte c, byte l, byte*p)
{
    // lut-data writting initialization
    EPD_SendCommand(c);

    // lut-data writting doing
    for (int i = 0; i < l; i++, p++) EPD_SendData(*p);
}

/* Writting lut-data of the black-white channel ------------------------------*/



/* This function is used to 'wake up" the e-Paper from the deep sleep mode ---*/
void EPD_Reset() 
{
    digitalWrite(PIN_SPI_RST, LOW);    
    delay(200);
    
    digitalWrite(PIN_SPI_RST, HIGH); 
    delay(200);    
}

/* e-Paper initialization functions ------------------------------------------*/ 

#include "epd2in9.h"

          // If true, then image data bits must be inverted
int  EPD_dispIndex;        // The index of the e-Paper's type
int  EPD_dispX, EPD_dispY; // Current pixel's coordinates (for 2.13 only)
void(*EPD_dispLoad)();     // Pointer on a image data writting function

/* Image data loading function for a-type e-Paper ----------------------------*
void EPD_loadA()
{
    // Get the index of the image data begin
    int pos = 6;//6是数据位起始
    if ((int)Buff__bufArr[4]==1) EPD_invert=true;
    // Enumerate all of image data bytes
    while (pos < 4742)//buffind最后的位置
    {
        // Get current byte
        int value = Buff__getByte(pos);

        // Invert byte's bits in case of '2.7' e-Paper
        if (EPD_invert) value = ~value;

        // Write the byte into e-Paper's memory
        EPD_SendData((byte)value);

        // Increment the current byte index on 2 characters
        pos++;
    }
}
*/

/* Show image and turn to deep sleep mode (a-type, 4.2 and 2.7 e-Paper) ------*/
void EPD_showA() 
{
    // Refresh
    EPD_Send_1(0x22, 0xC4);//DISPLAY_UPDATE_CONTROL_2
    EPD_SendCommand(0x20);//MASTER_ACTIVATION
    EPD_SendCommand(0xFF);//TERMINATE_FRAME_READ_WRITE
    EPD_WaitUntilIdle();

    // Sleep
    EPD_SendCommand(0x10);//DEEP_SLEEP_MODE
    EPD_WaitUntilIdle();
}


/* The set of pointers on 'init', 'load' and 'show' functions, title and code */
struct EPD_dispInfo
{
    int(*init)(); // Initialization
    //void(*chBk)();// Black channel loading
    //int next;     // Change channel code
    //void(*chRd)();// Red channel loading
    void(*show)();// Show and sleep
    char*title;   // Title of an e-Paper
};

/* Array of sets describing the usage of e-Papers ----------------------------*/
EPD_dispInfo EPD_dispMass[] =
{
   
    { EPD_Init_2in9  ,EPD_showA, "2.9 inch"}// j 9

};

/* Initialization of an e-Paper ----------------------------------------------*/
void EPD_dispInit()
{
    // Call initialization function
    EPD_dispMass[EPD_dispIndex].init();

    // Set loading function for black channel
    //EPD_dispLoad = EPD_dispMass[EPD_dispIndex].chBk;

    // Set initial coordinates
    EPD_dispX = 0;
    EPD_dispY = 0;

    // The inversion of image data bits isn't needed by default
    EPD_invert = false;
    
}
