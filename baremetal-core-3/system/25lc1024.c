#include <xinu.h>

static uint8 UNIQUEID[8];



/// Select the flash chip
static void select(uint8 cs) {
  SYS_ENTRY();
  if(CS_RAM1){
  	hw_set_pin(GPIOx(GPIO_B), 12, 0);
  }else{
  	hw_set_pin(GPIOx(GPIO_A), 8, 0);
  }
}

/// UNselect the flash chip
static void unselect(uint8 cs) {
  if(CS_RAM1){
  	hw_set_pin(GPIOx(GPIO_B), 12, 1);
  }else{
  	hw_set_pin(GPIOx(GPIO_A), 8, 1);
  }
  SYS_EXIT();
}

static void writeEnable(uint8 cs) {
  select(cs);
  transfer2(SPIFLASH_WRITEENABLE);
  unselect(cs);
  while(busy(cs));
}

static void writeDisable(uint8 cs) {
  select(cs);
  transfer2(SPIFLASH_WRITEDISABLE);
  unselect(cs);
  while(busy(cs));
}
/*
static void chipErase() {
  writeEnable();
  select();
  transfer2(SPIFLASH_CHIPERASE);
  unselect();
  while(busy());
  writeDisable();
}

/// Get the manufacturer and device ID bytes (as a short word)
uint16 readDeviceId()
{
  select();
  transfer2(SPIFLASH_IDREAD);
  uint16 jedecid = transfer2(0) << 8;
  jedecid |= transfer2(0);
  unselect();
  return jedecid;
}

*/



void SPI25lc1024(){
	  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    hw_cfg_pin(GPIOx(GPIO_B),   13/*sck_pin*/,    GPIOCFG_MODE_ALT | GPIO_AF5_SPI2 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(GPIO_B),   14/*miso_pin*/,   GPIOCFG_MODE_ALT | GPIO_AF5_SPI2 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(GPIO_B),   15/*mosi_pin*/,   GPIOCFG_MODE_ALT | GPIO_AF5_SPI2 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(GPIO_B), 12/*cs*/,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_set_pin(GPIOx(GPIO_B), 12, 1);
    hw_cfg_pin(GPIOx(GPIO_A), 8/*cs*/,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_set_pin(GPIOx(GPIO_A), 8, 1);
	  spi2_init();




    //chipErase();
   

   // select(CS_RAM2);
   // transfer2(SPIFLASH_WAKE);
   // unselect(CS_RAM2);
    //while(busy());


    //kprintf("id flash: %x\n",readDeviceId());


    /*for (int i = 0; i < 32; ++i)
    {
    	writeByte(CS_RAM1,i,0x88);
    }
    

    for (int i = 0; i < 32; ++i)
    {
    	uint8 r = readByte(CS_RAM1,i);
    	kprintf("%d %x\n",i,r);
    }*/



    /*for (int i = 0; i < 32; ++i)
    {
    	writeByte(CS_RAM2,i,0x33);
    }
    

    for (int i = 0; i < 32; ++i)
    {
    	uint8 r = readByte(CS_RAM2,i);
    	kprintf("%x\n",r);
    }*/



    kprintf("\n\n");

   

    /*uint8 *buf=malloc(256);
    //uint8 buf[256];
    memset(buf,0x66,255);
    //writeBytes(CS_RAM1,0,buf,255);
    accessPSRAM(130000, 255,true,buf);
    
    //readBytes(CS_RAM1,0,buf,255);
    accessPSRAM(130000, 255,false,buf);
    //hexDump2(0,buf,255); 
    for (int i = 0; i < 255; ++i)
    {
    	kprintf("%d--%x\n",i,buf[i]);
    }
    free(buf);*/
    


    kprintf("test sram..\n");

    uint8 *buf=malloc(256);
    uint32 offset=0;
    uint32 star;
    uint32 end;

    star = get_timer(TIMER2);
    for (int i = 0; i < (RAM_HALF*2)/255; ++i)
    {
         memset(buf,0xbe,255);
         accessPSRAM(i+offset, 255,true,buf);
         offset += 255;
    }
    end=get_timer(TIMER2);

    kprintf("writed %d\n",end-star );
    memset(buf,0x00,255);
    offset=0;

    star = get_timer(TIMER2);
    for (int i = 0; i < (RAM_HALF*2)/255; ++i)
    {
        //kprintf("%d--",i);
        accessPSRAM(i+offset, 255,false,buf);
        //hexDump2(0,buf,255); 
        //for (int j = 0; j < 255; ++j)
        //{
          //kprintf("%d--%x\n",j,buf[j]);
        //}

        offset += 255;
    }
    end=get_timer(TIMER2);

    kprintf("readed %d\n",end-star );

    
    free(buf);
    
   

    
   
}


bool busy(uint8 cs)
{
  return readStatus(cs) & 1;
}

/// return the STATUS register
uint8 readStatus(uint8 cs)
{
  select(cs);
  transfer2(SPIFLASH_STATUSREAD);
  uint8 status = transfer2(0);
  unselect(cs);
  return status;
}



/// read 1 byte from flash memory
uint8 readByte(uint8 cs,uint32 addr) {
  select(cs);
  transfer2(SPIFLASH_ARRAYREADLOWFREQ);
  transfer2((addr >> 16) & 0xFF);
  transfer2((addr >> 8) & 0xFF);
  transfer2(addr & 0xff);
  uint8 result = transfer2(0);
  unselect(cs);
  while(busy(cs));
  return result;
}

void writeByte(uint8 cs,uint32 addr, uint8 byt) {
  //writeEnable();
  select(cs);
  transfer2(SPIFLASH_BYTEPAGEPROGRAM);
  transfer2((addr >> 16) & 0xFF);
  transfer2((addr >> 8) & 0xFF);
  transfer2(addr & 0xff);
  transfer2(byt);
  unselect(cs);
  while(busy(cs));
  //writeDisable();
}


void readBytes(uint8 cs,uint32 addr, void* buf, uint16 len){
  select(cs);
  transfer2(READ);
  transfer2((addr >> 16) & 0xFF);
  transfer2((addr >> 8) & 0xFF);
  transfer2(addr & 0xff);
  for (uint16 i = 0; i < len; ++i){
  	  ((uint8*) buf)[i] = transfer2(0);
  }
  unselect(cs);
  while(busy(cs));
}
void writeBytes(uint8 cs,uint32 addr, const void* buf, uint16 len){
  uint16 n;
  uint16 maxBytes = 256-(addr%256);  // force the first set of bytes to stay within the first page
  //uint16 offset = 0;
  //while (len>0)
  //{

    n = (len<=maxBytes) ? len : maxBytes;
    //writeEnable();
    select(cs);
    transfer2(WRITE);
    transfer2(addr >> 16);
    transfer2(addr >> 8);
    transfer2(addr & 0xff);
    
    for (uint16 i = 0; i < n; i++){
         transfer2(((uint8*) buf)[i]);
    }
    unselect(cs);
    while(busy(cs));
}


void accessPSRAM(uint32 addr, size_t size, bool write, void *bufP){
    uint8 *b = (uint8 *)bufP;
    uint32 cmdSize = 4;
    uint32 ramchip = CS_RAM1;
    uint8 cmdAddr[5];
    if (write)
        cmdAddr[0] = WRITE;
    else
    {
        cmdAddr[0] = READ;
        cmdSize++;
    }

    if (addr >= RAM_HALF)
    {
        ramchip = CS_RAM2;
        addr -= RAM_HALF;
    }

    cmdAddr[1] = (addr >> 16) & 0xff;
    cmdAddr[2] = (addr >> 8) & 0xff;
    cmdAddr[3] = addr & 0xff;

    select(ramchip);

    transfer2(cmdAddr[0]);
    transfer2(cmdAddr[1]);
    transfer2(cmdAddr[2]);
    transfer2(cmdAddr[3]);
    
    for (uint16 i = 0; i < size; i++){
         //transfer2(((uint8*) bufP)[i]);
        transfer2(b[i]);
    }

    unselect(ramchip);
    while(busy(ramchip));
}
