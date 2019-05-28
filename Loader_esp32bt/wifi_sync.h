#include <WiFi.h>
#include <HTTPClient.h>
#include <StreamString.h>
HTTPClient http;
RTC_DATA_ATTR char ssid[20];
RTC_DATA_ATTR char password[20];//掉电保存
char url[]="http://159.203.0.110:8005/";
void wifi_init()
{
  EPD_dispInit();
  read_from_spiff(SPIFFS,"/sync");
  EPD_dispMass[EPD_dispIndex].show();
  //now_display_word=97;
  //now_display_type=48;
  unsigned long startconnectTime=millis();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  //delay(1000);
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED)
  {
    if (millis()-startconnectTime>=15000)
    {
      Serial.println("timeout");
      //display_page(now_display_word,now_display_type);
      return;
    }
  }
  Serial.println("connected");
}
bool recvframe(fs::FS &fs,int word_index)
{
    char word_index_s[10];
    //char url[]="http://47.101.165.34:8005/";加入这行就会炸！！！
    itoa(word_index,word_index_s,10);
    if (WiFi.status()==WL_CONNECTED)
  {
    strcat(url,word_index_s);
    http.begin(url);//TODO:待在服务器端加入返回指定文件 DONE
    url[26]='\0';
    url[27]='\0';
    int httpcode=http.GET();
    if (httpcode==HTTP_CODE_OK)
    {
      StreamString streamstr;
      http.writeToStream(&streamstr);//可返回正常串
      //int size=http.getSize();
      //Serial.println(streamstr);
      byte framehead[6];
      framehead[0]=(byte)streamstr.read();//1 L
      if (framehead[0]!=76) return false;
      framehead[1]=(byte)streamstr.read();//2
      framehead[2]=(byte)streamstr.read();//3
      framehead[3]=(byte)streamstr.read();
      framehead[4]=(byte)streamstr.read();
      framehead[5]=(byte)streamstr.read();
      //Serial.println(framehead[3]);
      //Serial.println(framehead[4]);
      int wordnumber=(int)framehead[3]+97;//4
      now_display_word=wordnumber;
      int pictype=(int)framehead[4]+48;//5
      now_display_type=pictype;
      char filename[]={'/',(char)wordnumber,(char)pictype,'\0'};
      //char filename[]="/a0";

      Serial.println(filename);
      File file = fs.open(filename,FILE_WRITE);
      file.write(framehead,6);
      for (int i=0;i<4736;i++)
      {
        byte value[1]={(byte)streamstr.read()};
        //Serial.print((int)value[0]);
        file.write(value,1);
      }
      //Serial.println();
      file.close();
      //两图连发测试
      //byte framehead[6];
      framehead[0]=(byte)streamstr.read();//1 L
      if (framehead[0]!=76) return false;
      framehead[1]=(byte)streamstr.read();//2
      framehead[2]=(byte)streamstr.read();//3
      framehead[3]=(byte)streamstr.read();
      framehead[4]=(byte)streamstr.read();
      framehead[5]=(byte)streamstr.read();
      //Serial.println(framehead[3]);
      //Serial.println(framehead[4]);
      wordnumber=(int)framehead[3]+97;//4
      pictype=(int)framehead[4]+48;//5
      char filename2[]={'/',(char)wordnumber,(char)pictype,'\0'};
      //char filename[]="/a0";
      
      Serial.println(filename2);
      File file2 = fs.open(filename2,FILE_WRITE);
      file2.write(framehead,6);
      for (int i=0;i<4736;i++)
      {
        byte value2[1]={(byte)streamstr.read()};
        //Serial.print((int)value[0]);
        file2.write(value2,1);
      }
      file2.close();
      streamstr.flush();
      //测试结束 DONE 可用
        //EPD_dispInit();
        //read_from_spiff(SPIFFS,filename);
        //EPD_dispMass[EPD_dispIndex].show();
    }
    else 
    {
    Serial.println("GET ERROR");
    return false;
    }
  //display_page(now_display_word,now_display_type);
  }
  else
  {
    Serial.println("CONNECTION LOST");
    return false;
  }
  
  return true;
}
/*
bool get_today_words()
{
  wifi_init();
  for(int i=0;i<10;i++)
  {
    if(recvframe(SPIFFS,i)) continue;
    else return false;
  }
  //display_page(now_display_word,now_display_type);
  WiFi.disconnect();
  Serial.println("Sync done.");
      //EPD_dispInit();
    //read_from_spiff(SPIFFS,"/syncs");
    //EPD_dispMass[EPD_dispIndex].show();
    //delay(1000);会炸！！！
    
  return true;
}*/
