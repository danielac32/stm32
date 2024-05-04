#include <string.h>
#include <stdio.h>
#include <diskio.h>
#include <spiflash.h>

uint8_t lowmemRAM[BLOCK];
uint8_t cacheRAM1[BLOCK];
uint8_t cacheRAM2[BLOCK];
//uint8_t cacheRAM3[BLOCK];
//uint8_t cacheRAM4[BLOCK];

uint32_t block1=0;
uint32_t block2=0;
//uint32_t block3=0;
//uint32_t block4=0;

uint8_t dirty1=0;
uint8_t dirty2=0;
//uint8_t dirty3=0;
//uint8_t dirty4=0;

uint8_t NextCacheUse=0;
uint32_t max=0;

static const uint32_t base = 4000000;
uint32_t sector = base / BLOCK;

void cachebegin(uint32_t size){
   max=size;
   block1=sector;
   block2=sector+1;
   //block3=sector+2;
   //block4=sector+3;

   dirty1=0;
   dirty2=0;
   //dirty3=0;
   //dirty4=0;
 
   NextCacheUse=0;
   memset(lowmemRAM,0,sizeof(lowmemRAM));
   memset(cacheRAM1,0,BLOCK);
   memset(cacheRAM2,0,BLOCK);

   for(int i=0;i<(size/BLOCK)+1;i++){
   	   //kprintf("%d %d\n",i,sector+i);
   }
   disk_initialize(0);
   //printf("\nblocks %d %d %d %d\n",block1,block2,block3,block4 );
   kprintf("size memory: %d, base: %08x, sector: %d\n",max,base,sector );
}

uint32_t cacheget_size(){
       return max;
}



void cachewrite(uint32_t address,uint8_t value){
    if (address<sizeof(lowmemRAM)) {
        lowmemRAM[address]=value; //If RAM lower than 0x600 write direct RAM
        return;
    }
    uint32_t block=(address&0xfffff000); //Else write into cached block
    
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
    
    if (NextCacheUse==0) {
      if (dirty1) {           //Write back the cached block if it's dirty
        disk_write (0, (cacheRAM1), block1, 1);
      }
      
      disk_read (0, (cacheRAM1), block, 1);
      block1=(address&0xfffff000);
      NextCacheUse+=1;
      NextCacheUse&=2;
      cacheRAM1[address&0x00000fff]=value; //Now write it in cache1
      dirty1=1;
      return;
    }
    
    if (NextCacheUse==1) {
      if (dirty2) {           //Write back the cached block if it's dirty
          disk_write (0, (cacheRAM2), block2, 1);
      }
      disk_read (0, (cacheRAM2), block, 1);
      block2=(address&0xfffff000);
      NextCacheUse+=1;
      NextCacheUse&=2;
      cacheRAM2[address&0x00000fff]=value; //Now write it in cache2
      dirty2=1;
      return;
    }
}

uint8_t cacheread(uint32_t address){
    if (address<sizeof(lowmemRAM)) return lowmemRAM[address]; //If RAM lower than 0x600 return direct RAM
    uint32_t block=(address&0xfffff000); //Else look into cached RAM
    if (block1==block) return cacheRAM1[address&0x00000fff]; //Cache hit in cache1
    if (block2==block) return cacheRAM2[address&0x00000fff]; //Cache hit in cache2
    if (NextCacheUse==0) {
      if (dirty1) {           //Write back the cached block if it's dirty
        disk_write (0, (cacheRAM1), block1, 1);
        dirty1=0;
      }
      
      disk_read (0, (cacheRAM1), block, 1);
      block1=(address&0xfffff000);
      NextCacheUse+=1;
      NextCacheUse&=2;
      return cacheRAM1[address&0x00000fff]; //Now found it in cache1
    }
    
    if (NextCacheUse==1) {
      if (dirty2) {           //Write back the cached block if it's dirty
        disk_write (0, (cacheRAM2), block2, 1);
        dirty2=0;
      }
      disk_read (0, (cacheRAM2), block, 1);
      block2=(address&0xfffff000);
      NextCacheUse+=1;
      NextCacheUse&=2;
      return cacheRAM2[address&0x00000fff]; //Now found it in cache2
    }
}



