/**
  ******************************************************************************
  * @file    srvr.h
  * @author  Waveshare Team
  * @version V2.0.0
  * @date    10-August-2018
  * @brief   ESP8266 WiFi server.
  *          This file provides firmware functions:
  *           + Sending web page of the tool to a client's browser
  *           + Uploading images from client part by part
  * framehead:
  * I
  * 初始化屏幕，刷新显存，每次显示前都需要执行
  * L
  * load蓝牙接收图像到spiffs
  * S
  * show根据蓝牙信号写入显存
  * C
  * 清空Spiffs 慎用
  * W
  * 启动无线同步
  *
  ******************************************************************************
  */ 

/* Library includes ----------------------------------------------------------*/
#include <BluetoothSerial.h>

bool Srvr__btIsOn;// It's true when bluetooth is on
bool Srvr__btConn;// It's true when bluetooth has connected client 
int  Srvr__msgPos;// Position in buffer from where data is expected
int  Srvr__length;// Length of loaded data
bool EPD_invert= false;//默认不反色
RTC_DATA_ATTR int now_display_word=97,now_display_type=48;//保存到RTC中 调电可保存
/* Client ---------------------------------------------------------------------*/
BluetoothSerial Srvr__btClient; // Bluetooth client 

/* Avaialble bytes in a stream ------------------------------------------------*/
int Srvr__available()
{
    return Srvr__btIsOn ? Srvr__btClient.available() : false;
}

void Srvr__write(const char*value)
{
    // Write data to bluetooth
    if (Srvr__btIsOn) Srvr__btClient.write((const uint8_t*)value, strlen(value));
}

int Srvr__read()
{
    return Srvr__btIsOn ? Srvr__btClient.read() : -1;
}

void Srvr__flush()
{
    // Clear Bluetooth's stream
    if (Srvr__btIsOn) Srvr__btClient.flush();  
}

/* Project includes ----------------------------------------------------------*/
#include "buff.h"       // POST request data accumulator
#include "epd.h"        // e-Paper driver
#include "load_spiff.h"
#include "wifi_sync.h"//wifisync functions
/* display function------------------------------------------------------------*/
void display_page(int wordnumber,int pictype)
{
  EPD_dispInit();
  if (pictype==49) EPD_invert=true;
  const char filename[]={'/',(char)wordnumber,(char)pictype,'\0'};
  Serial.printf("loading %s",filename);
  read_from_spiff(SPIFFS,filename);
  EPD_dispMass[EPD_dispIndex].show();
}
bool Srvr__btSetup()                                              
{
    // Name shown in bluetooth device list of App part (PC or smartphone)
    String devName("esp32");

    // Turning on
    Srvr__btIsOn = Srvr__btClient.begin(devName);

    // Show the connection result
    if (Srvr__btIsOn) Serial.println("Bluetooth is on");
    else Serial.println("Bluetooth is off");

    // There is no connection yet
    Srvr__btConn = false;

    // Return the connection result
    return Srvr__btIsOn;
}

/* The server state observation loop -------------------------------------------*/
bool Srvr__loop() 
{
    //Serial.println("in Srvr loop");
    // Bluetooh connection checking
    if (!Srvr__btIsOn) 
    {
      //Serial.println("exit for not on");
      return false;
    }

    // Show and update the state if it was changed
    /*
    if (Srvr__btConn != Srvr__btClient.hasClient())
    {
        //Serial.print("Bluetooth status:");
        //Serial.println(Srvr__btConn = !Srvr__btConn ? "connected" : "disconnected"); 
    }
*/
    // Exit if there is no bluetooth connection
    if (!Srvr__btClient.hasClient()) //无连接时强制退出srvr循环
    {
      //Serial.println("exit for no conn");
      return false; 
    }

    // Waiting the client is ready to send data
    while(!Srvr__btClient.available())
    {
      if(!Srvr__btClient.hasClient()) return false;
      //Serial.println("waiting");
      delay(1);
    }

    // Set buffer's index to zero
    // It means the buffer is empty initially
    Buff__bufInd = 0;

    // While the stream of 'client' has some data do...
  for (;Buff__bufInd < 4742;)
  {
    //Srvr__write("ready\r\n");
        while (Srvr__available())
    {
        // Read a character from 'client'
        uint8_t q = Srvr__read();
        //Serial.println(q);
        // Save it in the buffer and increment its index
        Buff__bufArr[Buff__bufInd++] = (byte)q;
        //Serial.print(q);
        //Serial.print(',');
        //Serial.println(Buff__bufInd);
        if (Buff__bufInd==4742) break;//数据接受结束后立即退出接受循环，
    }
    if (Buff__bufArr[0]!='L') break;//数据帧接受完毕后依旧会进入等待循环
    //Srvr__write("continue?\r\n");
    //Serial.println("continue?\r\n");
    for (int timeout=0;Srvr__available()!=1;timeout++)
    {
      if (Buff__bufInd==4742) goto endrecv;//等待循环强制退出，开始载入
      if (timeout==999999)//TODO：使用gettime改写超时！！！
      {
        //Srvr__flush();
        Srvr__write("timeout\r\n");
        Serial.println("timeout\r\n");
        return false;
      }
      //delay(1);//超时检测
      
      //Srvr__write("continue\r\n");
      }
      //Srvr__write("continue\r\n");
  }
  endrecv:


    // Initialization
    if (Buff__bufArr[0] == 'I')
    {
        Srvr__length = 0;

        // Getting of e-Paper's type
        //EPD_dispIndex = Buff__bufArr[1];
        EPD_dispIndex = 0;
        // Print log message: initialization of e-Paper (e-Paper's type)
        Serial.printf("<<<EPD %s", EPD_dispMass[EPD_dispIndex].title);


        // Initialization
        EPD_dispInit();

        Buff__bufInd = 0;
        Srvr__flush();
    }

    // Loading of pixels' data
    else if (Buff__bufArr[0] == 'L')
    {
        // Print log message: image loading
        Serial.print("<<<LOAD");
        int dataSize = Buff__getWord(1);
        Serial.printf(" - %d",dataSize);
        Srvr__length += dataSize;//已接收data长度
                
        //if ((Buff__bufInd < dataSize) || Srvr__length != Buff__getN3(3))//第一项 若本帧长度比当前指针位置长 第二项 是余数关于第三项，见buff n3注释 校验用 可删
        if ((Buff__bufInd < dataSize))
        {
            Buff__bufInd = 0;
            Srvr__flush();

            Serial.print(" - failed!>>>");
            Srvr__write("Error!");
            return true;
        }
       
        // Load data into the e-Paper 
        // if there is loading function for current channel (black or red)
        //if (EPD_dispLoad != 0) EPD_dispLoad();     //调用epd中
        int wordnumber=Buff__getByte(3)+97;//第4位是序号(因数量少 用a-z标号)
        int pictype=Buff__getByte(4)+48;//第五位是类型（用0、1编号）
        char filename[]={'/',(char)wordnumber,(char)pictype,'\0'};
        load_to_spiff(SPIFFS,filename,Buff__bufArr);
        //Srvr__write("load done\n");
        //Buff__load(wordnumber,pictype);
        Buff__bufInd = 0;
        Srvr__flush();
    }


    // Show loaded picture
    else if (Buff__bufArr[0] == 'S')
    {
        //int wordnumber=Buff__getByte(3)+97;//第4位是序号(因数量少 用a-z标号)
        //int pictype=Buff__getByte(4)+48;//第五位是类型（用0、1编号）
        now_display_word=Buff__getByte(3)+97;
        now_display_type=Buff__getByte(4)+48;//保存至全局变量，便于其他地方知道当前显示图像
        display_page(now_display_word,now_display_type);
        Buff__bufInd = 0;
        Srvr__flush();

        //Print log message: show
        Serial.print("<<<SHOW");
    }
    
    else if (Buff__bufArr[0] == 'C')
    {
      deleteAll(SPIFFS,"/");
      Serial.print("<<<DELETING ALL");
    }
    else if (Buff__bufArr[0] == 'W')//fixed 待加入多次get逻辑
    {
        //中间未测试WiFi用代码
        int i=1;
        for(int j=0;Buff__bufArr[i]!='\0';i++)
        {
          ssid[j]=Buff__bufArr[i];
          //ssid[j+1]='\0';
          j++;
        }
        i++;
        Serial.println(ssid);
        for(int j=0;Buff__bufArr[i]!='\0';i++)
        {
          password[j]=Buff__bufArr[i];
          password[j+1]='\0';
          j++;
        }
        Serial.println(password);
        i++;
        int word_index=Buff__bufArr[i];//最后一位为需要的标号
        wifi_init();
        recvframe(SPIFFS,word_index);
        WiFi.disconnect();
        //display_page(now_display_word,now_display_type);
  //WiFi测试代码结束
    }
    else if (Buff__bufArr[0]=='G')
    {
      Serial.println("get daily words!");
      /*
      EPD_dispInit();
      read_from_spiff(SPIFFS,"/sync1");
      EPD_dispMass[EPD_dispIndex].show();
      now_display_word=97;
      now_display_type=48;
      //get_today_words();
      //重写daily循环*/
      wifi_init();
      for (int i=0;i<10;i++)
      {
        if(recvframe(SPIFFS,i)) continue;
        else 
        {
          EPD_dispInit();
          read_from_spiff(SPIFFS,"/syncf");
          EPD_dispMass[EPD_dispIndex].show();
          now_display_word=97;
          now_display_type=48;
          delay(2000);
          goto endofrecv;
        }
      }
      WiFi.disconnect();
      delay(1000);
      /*EPD_dispInit();
      read_from_spiff(SPIFFS,"/sync2");
      EPD_dispMass[EPD_dispIndex].show();
      now_display_word=97;
      now_display_type=48;*/
      wifi_init();
      for (int i=10;i<20;i++)
      {
        if(recvframe(SPIFFS,i)) continue;
        else 
        {
          EPD_dispInit();
          read_from_spiff(SPIFFS,"/syncf");
          EPD_dispMass[EPD_dispIndex].show();
          now_display_word=97;
          now_display_type=48;
          delay(2000);
          break;
        }
      }
      
      endofrecv:
      WiFi.disconnect();
      Serial.println("done");
      return false;//强制睡眠
      //EPD_dispInit();
      display_page(now_display_word=97,now_display_type=48);
      
      //
      /*
      if (get_today_words()) 
      {
        Serial.println("out");
        EPD_dispInit();
        read_from_spiff(SPIFFS,"/syncs");
        EPD_dispMass[EPD_dispIndex].show();
        now_display_word=97;
        now_display_type=48;
        display_page(now_display_word,now_display_type); 
      }

      else 
      {
        EPD_dispInit();
        read_from_spiff(SPIFFS,"/syncf");
        EPD_dispMass[EPD_dispIndex].show();
        now_display_word=97;
        now_display_type=48;
      }*/
    }
    else if (Buff__bufArr[0]=='R')//rename a3 page
    {
      char alter_name[20];
      for (int i=1;i<20;i++)
      {
        alter_name[i-1]=Buff__bufArr[i];
        //alter_name[i]='\0';
      }
      renameFile(SPIFFS,"/a3",alter_name);
      //Serial.println(alter_name);
    }
    else if (Buff__bufArr[0]=='D')
    {
      char delete_name[20];
      for (int i=1;i<20;i++)
      {
        delete_name[i-1]=Buff__bufArr[i];
        //alter_name[i]='\0';
      }
      deleteFile(SPIFFS,delete_name);
      //Serial.println(alter_name);
    }
    // Send message "Ok!" to continue
    Srvr__write("Ok!\n");
    delay(1);

    // Print log message: the end of request processing
    Serial.print(">>>\n");
    return true;
}
