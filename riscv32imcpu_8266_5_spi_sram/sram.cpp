#include <SPI.h>
#include "Arduino.h"
#include "sram.h"

SRAMClass::SRAMClass(){
    pinMode(CS_RAM1,OUTPUT);
    pinMode(CS_RAM2,OUTPUT);
    digitalWrite(CS_RAM1,true);
    digitalWrite(CS_RAM2,true);
}

void SRAMClass::write(uint32_t address, uint8_t data_byte){
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  if(address>125000){
     digitalWrite(CS_RAM1,true);
     digitalWrite(CS_RAM2,false);        //set SPI_SS low
     SPI.transfer(WRITE);
     SPI.transfer((uint8_t)(address >> 16) & 0xff);
     SPI.transfer((uint8_t)(address >> 8) & 0xff);
     SPI.transfer((uint8_t)address);
     SPI.transfer(data_byte);
     digitalWrite(CS_RAM2,true); 
  }else{
     digitalWrite(CS_RAM2,true);
     digitalWrite(CS_RAM1,false);        //set SPI_SS low
     SPI.transfer(WRITE);
     SPI.transfer((uint8_t)(address >> 16) & 0xff);
     SPI.transfer((uint8_t)(address >> 8) & 0xff);
     SPI.transfer((uint8_t)address);
     SPI.transfer(data_byte);
     digitalWrite(CS_RAM1,true); 
  }
  SPI.endTransaction();
}
uint8_t SRAMClass::read(uint32_t address){
  uint8_t read_byte;
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  if(address>125000){
     digitalWrite(CS_RAM1,true);
     digitalWrite(CS_RAM2,false);        //set SPI_SS low
     SPI.transfer(READ);
     SPI.transfer((uint8_t)(address >> 16) & 0xff);
     SPI.transfer((uint8_t)(address >> 8) & 0xff);
     SPI.transfer((uint8_t)address);
     read_byte = SPI.transfer(0x00);
     digitalWrite(CS_RAM2,true); 
  }else{
     digitalWrite(CS_RAM2,true);
     digitalWrite(CS_RAM1,false);        //set SPI_SS low
     SPI.transfer(READ);
     SPI.transfer((uint8_t)(address >> 16) & 0xff);
     SPI.transfer((uint8_t)(address >> 8) & 0xff);
     SPI.transfer((uint8_t)address);
     read_byte = SPI.transfer(0x00);
     digitalWrite(CS_RAM1,true); 
  }
  SPI.endTransaction();
  return read_byte;
  /*uint8_t read_byte;
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CS_RAM1,false);         //set SPI_SS low
  SPI.transfer(READ);
  SPI.transfer((uint8_t)(address >> 16) & 0xff);
  SPI.transfer((uint8_t)(address >> 8) & 0xff);
  SPI.transfer((uint8_t)address);
  read_byte = SPI.transfer(0x00);
  digitalWrite(CS_RAM1,true);          //set SPI_SS high
  SPI.endTransaction();
  return read_byte;*/
}


uint8_t SRAMClass::readBlock(uint32_t address,uint8_t *data,int size){
  uint8_t read_byte;
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));

  if(address>125000){
     digitalWrite(CS_RAM1,true);
     digitalWrite(CS_RAM2,false);        //set SPI_SS low
     SPI.transfer(READ);
     SPI.transfer((uint8_t)(address >> 16) & 0xff);
     SPI.transfer((uint8_t)(address >> 8) & 0xff);
     SPI.transfer((uint8_t)address);
     for(int i=0;i<size;i++){
         *data++ = SPI.transfer(0xff);
     }
     digitalWrite(CS_RAM2,true); 
  }else{
     digitalWrite(CS_RAM2,true);
     digitalWrite(CS_RAM1,false);        //set SPI_SS low
     SPI.transfer(READ);
     SPI.transfer((uint8_t)(address >> 16) & 0xff);
     SPI.transfer((uint8_t)(address >> 8) & 0xff);
     SPI.transfer((uint8_t)address);
     for(int i=0;i<size;i++){
         *data++ = SPI.transfer(0xff);
     }
     digitalWrite(CS_RAM1,true); 
  }
  SPI.endTransaction();
  return read_byte;
}

void SRAMClass::writeBlock(uint32_t address, uint8_t *data,int size){
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  if(address>125000){
     digitalWrite(CS_RAM1,true);
     digitalWrite(CS_RAM2,false);        //set SPI_SS low
     SPI.transfer(WRITE);
     SPI.transfer((uint8_t)(address >> 16) & 0xff);
     SPI.transfer((uint8_t)(address >> 8) & 0xff);
     SPI.transfer((uint8_t)address);

     for(int i=0;i<size;i++){
         SPI.transfer(*data++);
     }
     digitalWrite(CS_RAM2,true); 
  }else{
     digitalWrite(CS_RAM2,true);
     digitalWrite(CS_RAM1,false);        //set SPI_SS low
     SPI.transfer(WRITE);
     SPI.transfer((uint8_t)(address >> 16) & 0xff);
     SPI.transfer((uint8_t)(address >> 8) & 0xff);
     SPI.transfer((uint8_t)address);

     for(int i=0;i<size;i++){
         SPI.transfer(*data++);
     }
     digitalWrite(CS_RAM1,true); 
  }
  SPI.endTransaction();
}
