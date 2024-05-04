
#include <SPI.h>
uint8_t Buffer[512];
uint16_t tmp;
uint16_t Clustervar;
uint8_t Dir_Attrib = 0;
uint32_t Size = 0;
//78fd0c18 ram

uint8_t data;
uint8_t str[]={"Un microcontrolador (abreviado uC, UC o MCU)  "};
 
 
void setup() {
Serial.begin(115200);
Serial.println("System Ready!\r\n");  


byte i= disk_initialize(0);
i=fat_init();

Serial.println(i);
if(!i){
  for (int a = 1;a < 1024;a++){
              Clustervar = fat_read_dir_ent(0,a,&Size,&Dir_Attrib,Buffer);
              if(Clustervar == 0xffff){
                  break;
              }
              Serial.println((char *)Buffer);
        }
}
/*
if(!i){
  for(uint32_t i=0,k=0;i<4;i++){
      if(i==2){
         memset(Buffer,0xcc,512);
         disk_write(0, Buffer,2,1);
         memset(Buffer,0x66,512);
         disk_write(0, Buffer,3,1);
      }
      disk_read(0,Buffer,i,1);
      for(uint32_t j=0;j<512;j++){
          if(!(j % 2))Serial.print(" "); 
          if(!(j % 16))Serial.printf("    (%08lx)\n",k); 
          if(!(j % 16))k+=16;
          Serial.printf("%02x",Buffer[j]); 
          ESP.wdtDisable();
      }
      Serial.printf(" ->%i ",i);
  }
}else{
    Serial.println("sd card error");
}*/
/*
if (!mmc_init()){
     
     for(uint32_t i=0,k=0;i<4;i++){
         
         if(i==2){
            memset(Buffer,0xca,512);
            mmc_write_sector2(2, Buffer);
         }
         mmc_read_sector(i, Buffer);
         for(uint32_t j=0;j<512;j++){
             if(!(j % 2))Serial.print(" "); 
             if(!(j % 16))Serial.printf("    (%08lx)\n",k); 
             if(!(j % 16))k+=16;
             Serial.printf("%02x",Buffer[j]); 
             ESP.wdtDisable();
         }
         Serial.printf("\n->%i\n",i);
     }
     memset(Buffer,0xd0,512);
     mmc_write_sector2(2, Buffer);

      
}else{
    Serial.println("sd card error");
}*/

}

 


void loop() {
  
}
