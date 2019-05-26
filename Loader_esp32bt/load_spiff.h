#include "FS.h"
#include "SPIFFS.h"
#define FORMAT_SPIFFS_IF_FAILED true
void init_spiff()
{
    if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
    {
        Serial.println("SPIFFS Mount Failed");
        return;
    }
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}
/*
void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while(file.available()){
        Serial.write(file.read());
    }
}*/
/*
void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- frite failed");
    }
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("- failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("- message appended");
    } else {
        Serial.println("- append failed");
    }
}
*/
void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("- file renamed");
    } else {
        Serial.println("- rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}

void deleteAll(fs::FS &fs, const char * path)
{
  listDir(SPIFFS,path,0);
  File root = fs.open(path);
  if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
        while(file){
          deleteFile(SPIFFS,file.name());
        file = root.openNextFile();
    }
    listDir(SPIFFS,path,0);
}
void load_to_spiff(fs::FS &fs, const char * path,const char * buff)
{
    Serial.printf("getting file %s\r\n",path);
    File file = fs.open(path,FILE_WRITE);
    for (int i=0;i<4742;i++)
    {
      byte value[1]={(byte)buff[i]};
      //Serial.print((int)value[0]);
      file.write(value,1);
    }
    file.close();
    //readFile(SPIFFS,path);
    //listDir(SPIFFS,"/",0);
}//TODO:改写srvr中load逻辑 代替buffload DONE
void read_from_spiff(fs::FS &fs, const char * path)
{
    //char path_fix[]={path[0],path[1],path[2]};
    //readFile(SPIFFS,path);
    File file = fs.open(path);
    if(!file)
    {
        Serial.printf("no such file\r\n");
        return;
    }
    //Serial.write(file.read());
    Serial.printf("reading %s\r\n",path);
    byte value[1]={};
    file.read(value,1);
    if((int)value[0]!=76)//若不是一个图片文件，则加载error页，并且置回/a0
    {
      Serial.print("error\r\n");
      now_display_word=97;
      now_display_type=48;
      const char filename[]={'/','e','r','r','o','r','\0'};
      read_from_spiff(SPIFFS,filename);
      return;
      }
    for (int i=1;i<6;i++)
    {
      byte value[1]={};
      file.read(value,1);//移动指针到第六位
    }
    for (int i=6;i<4742;i++)
    {
      
    //Buff__bufArr[i]=(byte)file.read();
    byte value[1]={};
    file.read(value,1);
    //Serial.print((int)value[0]);
    if (EPD_invert) value[0]=~value[0];
    EPD_SendData(value[0]);
    }
    file.close();
    return;
}//TODO：先从文件中读取，在调用 最最最原始的loadA DONE
//TODO：每个单词一个文件夹，改变帧结构写入单词？？？词长为变值
