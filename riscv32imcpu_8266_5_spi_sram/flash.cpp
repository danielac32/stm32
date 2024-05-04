#include "Arduino.h"
#include "flash.h"

extern "C"{
  #include <spi_flash.h>
  uint32_t _empty_start; 
}
uint32_t base = ((uint32_t)&_empty_start - 0x40200000);
uint32_t sector = base / BLOCK;

FLASHClass::FLASHClass(){
  
}


void FLASHClass::begin(uint32_t size){
   max=size;
   block1=sector;
   block2=sector+1;
   block3=sector+2;
   block4=sector+3;

   dirty1=0;
   dirty2=0;
   dirty3=0;
   dirty4=0;
 
   NextCacheUse=0;
   memset(lowmemRAM,0,sizeof(lowmemRAM));
   memset(cacheRAM1,0,BLOCK);
   memset(cacheRAM2,0,BLOCK);
   memset(cacheRAM3,0,BLOCK);
   memset(cacheRAM4,0,BLOCK);

   for(int i=0;i<(size/BLOCK)+1;i++){
       spi_flash_erase_sector(sector+i);
   }
   
}

uint32_t FLASHClass::get_size(){
       return max;
}
void FLASHClass::write(uint32_t address,uint8_t value){
    if (address<sizeof(lowmemRAM)) {
        lowmemRAM[address]=value; //If RAM lower than 0x600 write direct RAM
        return;
    }
    uint32_t block=(address&0xfffff000)+0x100000; //Else write into cached block
    
    if (block1==block) {
      cacheRAM1[address&0x00000fff]=value; //Write in cache1
      dirty1=1; //mark cache1 as dirty
      return; 
    }
    
    if (block2==block) {
      cacheRAM2[address&0x00000fff]=value; //Write in cache2
      dirty2=1; //mark cache2 as dirty
      return; 
    }
    
    if (block3==block) {
      cacheRAM3[address&0x00000fff]=value; //Write in cache3
      dirty3=1; //mark cache3 as dirty
      return; 
    }
    
    if (block4==block) {
      cacheRAM4[address&0x00000fff]=value; //Write in cache4
      dirty4=1; //mark cache4 as dirty
      return; 
    }

     //Cache miss, fetch it
    
    if (NextCacheUse==0) {
      if (dirty1) {           //Write back the cached block if it's dirty
        
        spi_flash_erase_sector(block1>>12);
        
        spi_flash_write(block1, reinterpret_cast<uint32_t*>(cacheRAM1), BLOCK);
        
      }
      
      spi_flash_read(block, reinterpret_cast<uint32_t*>(cacheRAM1), BLOCK);
      
      block1=(address&0xfffff000)+0x100000;
      NextCacheUse+=1;
      NextCacheUse&=3;
      cacheRAM1[address&0x00000fff]=value; //Now write it in cache1
      dirty1=1;
      return;
    }
    
    if (NextCacheUse==1) {
      if (dirty2) {           //Write back the cached block if it's dirty
        
        spi_flash_erase_sector(block2>>12);
        
        spi_flash_write(block2, reinterpret_cast<uint32_t*>(cacheRAM2), BLOCK);
        
      }
      
      spi_flash_read(block, reinterpret_cast<uint32_t*>(cacheRAM2), BLOCK);
      
      block2=(address&0xfffff000)+0x100000;
      NextCacheUse+=1;
      NextCacheUse&=3;
      cacheRAM2[address&0x00000fff]=value; //Now write it in cache2
      dirty2=1;
      return;
    }
    
    if (NextCacheUse==2) {
      if (dirty3) {           //Write back the cached block if it's dirty
        
        spi_flash_erase_sector(block3>>12);
        
        spi_flash_write(block3, reinterpret_cast<uint32_t*>(cacheRAM3), BLOCK);
        
      }
      
      spi_flash_read(block, reinterpret_cast<uint32_t*>(cacheRAM3), BLOCK);
      
      block3=(address&0xfffff000)+0x100000;
      NextCacheUse+=1;
      NextCacheUse&=3;
      cacheRAM3[address&0x00000fff]=value; //Now write it in cache3
      dirty3=1;
      return;
    }
    
    if (NextCacheUse==3) {
      if (dirty4) {           //Write back the cached block if it's dirty
        
        spi_flash_erase_sector(block4>>12);
        
        spi_flash_write(block4, reinterpret_cast<uint32_t*>(cacheRAM4), BLOCK);
        
      }
      
      spi_flash_read(block, reinterpret_cast<uint32_t*>(cacheRAM4), BLOCK);
      
      block4=(address&0xfffff000)+0x100000;
      NextCacheUse+=1;
      NextCacheUse&=3;
      cacheRAM4[address&0x00000fff]=value; //Now write it in cache4
      dirty4=1;
      return;
    }
}

uint8_t FLASHClass::read(uint32_t address){
    if (address<sizeof(lowmemRAM)) return lowmemRAM[address]; //If RAM lower than 0x600 return direct RAM
    uint32_t block=(address&0xfffff000)+0x100000; //Else look into cached RAM
    if (block1==block) return cacheRAM1[address&0x00000fff]; //Cache hit in cache1
    if (block2==block) return cacheRAM2[address&0x00000fff]; //Cache hit in cache2
    if (block3==block) return cacheRAM3[address&0x00000fff]; //Cache hit in cache3
    if (block4==block) return cacheRAM4[address&0x00000fff]; //Cache hit in cache4
    //Cache miss, fetch it
    
    if (NextCacheUse==0) {
      if (dirty1) {           //Write back the cached block if it's dirty
        
        spi_flash_erase_sector(block1>>12);
        
        spi_flash_write(block1, reinterpret_cast<uint32_t*>(cacheRAM1), BLOCK);
        
        dirty1=0;
      }
      
      spi_flash_read(block, reinterpret_cast<uint32_t*>(cacheRAM1), BLOCK);
      
      block1=(address&0xfffff000)+0x100000;
      NextCacheUse+=1;
      NextCacheUse&=3;
      return cacheRAM1[address&0x00000fff]; //Now found it in cache1
    }
    
    if (NextCacheUse==1) {
      if (dirty2) {           //Write back the cached block if it's dirty
        
        spi_flash_erase_sector(block2>>12);
        
        spi_flash_write(block2, reinterpret_cast<uint32_t*>(cacheRAM2), BLOCK);
        
        dirty2=0;
      }
      
      spi_flash_read(block, reinterpret_cast<uint32_t*>(cacheRAM2), BLOCK);
      
      block2=(address&0xfffff000)+0x100000;
      NextCacheUse+=1;
      NextCacheUse&=3;
      return cacheRAM2[address&0x00000fff]; //Now found it in cache2
    }
    
    if (NextCacheUse==2) {
      if (dirty3) {           //Write back the cached block if it's dirty
        
        spi_flash_erase_sector(block3>>12);
        
        spi_flash_write(block3, reinterpret_cast<uint32_t*>(cacheRAM3), BLOCK);
        
        dirty3=0;
      }
      
      spi_flash_read(block, reinterpret_cast<uint32_t*>(cacheRAM3), BLOCK);
      
      block3=(address&0xfffff000)+0x100000;
      NextCacheUse+=1;
      NextCacheUse&=3;
      return cacheRAM3[address&0x00000fff]; //Now found it in cache3
    }
    
    if (NextCacheUse==3) {
      if (dirty4) {           //Write back the cached block if it's dirty
        
        spi_flash_erase_sector(block4>>12);
        
        spi_flash_write(block4, reinterpret_cast<uint32_t*>(cacheRAM4), BLOCK);
        
        dirty4=0;
      }
      
      spi_flash_read(block, reinterpret_cast<uint32_t*>(cacheRAM4), BLOCK);
      
      block4=(address&0xfffff000)+0x100000;
      NextCacheUse+=1;
      NextCacheUse&=3;
      return cacheRAM4[address&0x00000fff]; //Now found it in cache4
    }
}
