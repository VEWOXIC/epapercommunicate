#include <WiFi.h>
#include <HTTPClient.h>
#include <StreamString.h>
HTTPClient http;
char ssid[20];
char password[20];
void wifi_init()
{
  unsigned long startconnectTime=millis();
  //Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  //delay(1000);
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED)
  {
    if (millis()-startconnectTime>=15000)
    {
      Serial.println("timeout");
      return;
    }
    else
    {
      //Serial.println("connecting...");
    }
    //delay(500);
    
  }
  Serial.println("connected");
}
void recvframe(fs::FS &fs)
{
    if (WiFi.status()==WL_CONNECTED)
  {
    http.begin("http://47.101.165.34:8005/a.txt");//TODO:待在服务器端加入返回指定文件
    int httpcode=http.GET();
    if (httpcode==HTTP_CODE_OK)
    {
      StreamString streamstr;
      http.writeToStream(&streamstr);//可返回正常串
      //int size=http.getSize();
      Serial.println(streamstr);
      byte framehead[6];
      framehead[0]=(byte)streamstr.read();//1 L
      if (framehead[0]!=76) return;
      framehead[1]=(byte)streamstr.read();//2
      framehead[2]=(byte)streamstr.read();//3
      framehead[3]=(byte)streamstr.read();
      framehead[4]=(byte)streamstr.read();
      framehead[5]=(byte)streamstr.read();
      //Serial.println(framehead[3]);
      //Serial.println(framehead[4]);
      int wordnumber=(int)framehead[3]+97;//4不知道为什么 要多加个1
      int pictype=(int)framehead[4]+48;//5
      char filename[]={'/',(char)wordnumber,(char)pictype,'\0'};
      //char filename[]="/a0";
      
      //Serial.println(filename);
      File file = fs.open(filename,FILE_WRITE);
      file.write(framehead,6);
      for (int i=0;i<4736;i++)
      {
        byte value[1]={(byte)streamstr.read()};
        //Serial.print((int)value[0]);
        file.write(value,1);
      }
      file.close();
    }
    else Serial.println("GET ERROR");

  }
}
