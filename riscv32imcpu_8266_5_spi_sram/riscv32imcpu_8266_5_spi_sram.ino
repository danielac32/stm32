#define F_CPU    160000000L 
#define CS_PIN  15

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
 
#include "flash.h"
#include "sram.h"
 
FLASHClass flash;
SRAMClass sram;





File root;

void setup() {
uint32_t i=0,p=0,k=0;
uint8_t data;
long start,end;
 
 
  ESP.wdtDisable();
  Serial.begin(115200);
  
  pinMode(CS_PIN,OUTPUT);
  digitalWrite(CS_PIN,true);
  if(!SD.begin(CS_PIN)) {
      Serial.println("\ninitialization failed!");
      return;
  }
 
  Serial.println("\nSD OK!\n");
  root = SD.open("/");
  printDirectory(root, 0);
 
   //SPI.setFrequency();
  Serial.println("\n test ram");
  start=millis();
  int c=0;
  int target=0x3a;
  bool check=false;
  
  for(long i=0;i<8;i++){
      sram.write(i,target);
      c=sram.read(i);
      if(c!=target){
        Serial.println("fail............!!!");
        check=true;
      }
      Serial.printf("%02x\n",c); 
  }
  if(check)while(1);
  end=millis();
  Serial.printf("write bytes: %d\n",end-start);
  init_soc();
/*
   Serial.printf("\n\n\dump flash \n");
   start=millis();
   for(uint32_t i=0;i<10000;i++){
       if(!(i % 2))Serial.print(" "); 
       if(!(i % 16))Serial.printf("    (%08lx)\n",k); 
       if(!(i % 16))k++;
       Serial.printf("%02x",flash.read(i)); 
       ESP.wdtDisable();
   }
   end=millis();
   Serial.printf("\n\ndump flash time: %d\n",end-start);*/
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
        Serial.println("/");
      //printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
void loop() {
  // put your main code here, to run repeatedly:
ESP.wdtDisable();
}
