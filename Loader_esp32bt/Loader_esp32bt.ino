/**
  ******************************************************************************
    @file    Loader.h
    @author  Waveshare Team
    @version V2.0.0
    @date    10-August-2018
    @brief   The main file.
             This file provides firmware functions:
              + Initialization of Serial Port, SPI pins and server
              + Main loop

  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "srvr.h" // Server functions
#include "mpu.h"//mpu functions

/* Entry point ----------------------------------------------------------------*/
unsigned long countstart = 0; //用于计时两边缘之间的时间 ms
unsigned long last_wake = 0;
unsigned long wake_now = 0;
const unsigned int waittime=500;//两边缘之间容忍时间 ms
bool chs_readyflag=false;//ready to change page to chs
int time_to_sleep=60000;//无操作睡眠前容忍时间 ms
void setup()
{
  // Serial port initialization
  Serial.begin(115200);
  delay(10);

  // Bluetooth initialization
  Srvr__btSetup();
  //initialize the spiff
  init_spiff();//格式不统一 草 懒得改了
  // SPI initialization
  EPD_initSPI();
  // mpu initialization
  mpu_init();
  // Initialization is complete
  Serial.print("\r\nOk!\r\n");
  last_wake=millis();
  //检测上次睡眠原因
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();
  if (wakeup_reason==ESP_SLEEP_WAKEUP_TIMER)
  {
    EPD_dispInit();
    read_from_spiff(SPIFFS,"/syncs");
    EPD_dispMass[EPD_dispIndex].show();
    delay(2000);
    display_page(now_display_word,now_display_type);
  }
}

/* The main loop -------------------------------------------------------------*/
void loop()
{
  wake_now = millis();
  if((wake_now-last_wake)>time_to_sleep)
  {
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,1); 
    Serial.println("Going to sleep now, press p33 to wake me...");
    esp_deep_sleep_start();
  }
  if (Srvr__loop()) last_wake=millis();//重置最后激活时间;//蓝牙断开则在mainloop中循环srvrloop 再退出
  //Serial.println("main looping");

  mpu_read();
  //卡片正反面翻页
//尖峰检测函数，若单侧cliff后0.5s未检测到反向，则取消
    if (ayqueue[0]-ayqueue[sample_length-1]<-30) //y旋转正向为负边缘
  {
    Serial.println("y axis neg-cliff detected");
    chs_readyflag=true;//准备位置一
    countstart=millis();//开始计时
    unsigned long now = millis();
    while(chs_readyflag)
    {
      mpu_read();
      if (ayqueue[0]-ayqueue[sample_length-1]>30) //y旋转负方向为正边缘
        {
          Serial.println("y axis pos-cliff detected");
          if (now_display_type==48)
          {
            now_display_type=49;
            display_page(now_display_word,now_display_type);
          }
          else if(now_display_type==49)
          {
            now_display_type=48;
            display_page(now_display_word,now_display_type);
          }
          chs_readyflag=false;
          last_wake=millis();//重置最后激活时间
          break;
        }
      now =millis();
      if(now-countstart>=waittime)
      {
        Serial.println("canceled...");
        chs_readyflag=false;
      }
    }
  }
  if (ayqueue[0]-ayqueue[sample_length-1]>30) 
  {
    Serial.println("y axis pos-cliff detected");
    chs_readyflag=true;//准备位置一
    countstart=millis();//开始计时
    unsigned long now = millis();
    while(chs_readyflag)
    {
      mpu_read();
      if (ayqueue[0]-ayqueue[sample_length-1]<-30) 
        {
          Serial.println("y axis neg-cliff detected");
          if (now_display_type==48)
          {
            now_display_type=49;
            display_page(now_display_word,now_display_type);
          }
          else if(now_display_type==49)
          {
            now_display_type=48;
            display_page(now_display_word,now_display_type);
          }
          chs_readyflag=false;
          last_wake=millis();//重置最后激活时间
          break;
        }
      now =millis();
      if(now-countstart>=waittime)
      {
        Serial.println("canceled...");
        chs_readyflag=false;
      }
    }
  }
  //卡片左右翻页（下一页）
  if (axqueue[0]-axqueue[sample_length-1]<-30) //x旋转正向为负边缘
  {
    Serial.println("x axis neg-cliff detected");
    chs_readyflag=true;//准备位置一
    countstart=millis();//开始计时
    unsigned long now = millis();
    while(chs_readyflag)
    {
      mpu_read();
      if (axqueue[0]-axqueue[sample_length-1]>30) //x旋转负方向为正边缘
        {
          Serial.println("x axis pos-cliff detected");
          now_display_word++;
          display_page(now_display_word,now_display_type);
          chs_readyflag=false;
          last_wake=millis();//重置最后激活时间
          break;
        }
      now =millis();
      if(now-countstart>=waittime)
      {
        Serial.println("canceled...");
        chs_readyflag=false;
      }
    }
  }
  //上一页
  if (axqueue[0]-axqueue[sample_length-1]>30) //x旋转负向为正边缘
  {
    Serial.println("x axis pos-cliff detected");
    chs_readyflag=true;//准备位置一
    countstart=millis();//开始计时
    unsigned long now = millis();
    while(chs_readyflag)
    {
      mpu_read();
      if (axqueue[0]-axqueue[sample_length-1]<-30) //x旋转正方向为负边缘
        {
          Serial.println("x axis neg-cliff detected");
          now_display_word--;
          display_page(now_display_word,now_display_type);
          chs_readyflag=false;
          last_wake=millis();//重置最后激活时间
          break;
        }
      now =millis();
      if(now-countstart>=waittime)
      {
        Serial.println("canceled...");
        chs_readyflag=false;
      }
    }
  }
  //TODO:加入陀螺仪 DONE
  //TODO：加入deepsleep DONE
}
