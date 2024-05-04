#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
#include <usbapi.h>
#include <queue.h>
#include <memory.h>
#include <kernel.h>
#include <process.h>
#include <delay.h>
#include <syscall.h>
//#include <ff.h>
//#include <w25q.h>
#include <spiflash.h>
#include "disk.h"
#include "fat_filelib.h"
 
syscall_t syscallp;
extern void create_task();
 

int main()
{
	
	hw_cfg_pin(GPIOx(GPIO_C),13,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(GPIO_A),0,GPIOCFG_MODE_INP | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_PUP);
    hw_set_pin(GPIOx(GPIO_C),13, 1);
 


    while(1){
        //usbd_ep_read(&udev, CDC_RXD_EP, &fifo, CDC_DATA_SZ);

        if (!hw_get_pin(GPIOx(GPIO_A),0) || !strncmp(fifo,"ok",2)){
            delay(10000);
            break;
        }

    } 
    
 
      struct  memblk  *memptr;  /* Ptr to memory block    */
      uint32  free_mem;   /* Total amount of free memory  */

      /* Output Xinu memory layout */
      free_mem = 0;
      for (memptr = memlist.mnext; memptr != NULL; memptr = memptr->mnext) {
           free_mem += memptr->mlength;
      }
      kprintf("%10d bytes of free memory.  Free list:\n", free_mem);
      for (memptr=memlist.mnext; memptr!=NULL;memptr = memptr->mnext) {
          kprintf("           [0x%08X to 0x%08X]\n",
        (uint32)memptr, ((uint32)memptr) + memptr->mlength - 1);
      }

     sd_init();
     fl_init();

      // Attach media access functions to library
      if (fl_attach_media(sd_readsector, sd_writesector) != FAT_INIT_OK)
      {
          kprintf("ERROR: Failed to init file system\n");
          return -1;
      }
 
  // List the root directory
      fl_listdirectory("/");



    /*kprintf("%10d bytes of code.\n",
        (uint32)&etext - (uint32)&text);
    kprintf("           [0x%08X to 0x%08X]\n",
        (uint32)&text, (uint32)&etext - 1);
    kprintf("%10d bytes of data.\n",
        (uint32)&ebss - (uint32)&data);
    kprintf("           [0x%08X to 0x%08X]\n\n",
        (uint32)&data, (uint32)&ebss - 1); */ 
 

    /*cachebegin(4000000);
    for (int i = 0; i < 500000; ++i)
    {
        cachewrite(i,0x33);
    }
    cachewrite(500000,0xbe);
    cachewrite(500000-1,0xbe);
    cachewrite(500000-2,0xbe);
    cachewrite(500000-3,0xca);
    cachewrite(500000-4,0xfe);
 
    uint8 c = cacheread(500000);
    kprintf("%02x\n",c);


    c = cacheread(0);
    kprintf("%02x\n",c);

    c = cacheread(500000-1);
    kprintf("%02x\n",c);

    c = cacheread(500000-2);
    kprintf("%02x\n",c);

    c = cacheread(500000-3);
    kprintf("%02x\n",c);

    c = cacheread(500000-4);
    kprintf("%02x\n",c);*/


     
     
 
    create_task();

 
    while(1){

       hw_set_pin(GPIOx(GPIO_C),13, 0);
       delay(1000000);
       hw_set_pin(GPIOx(GPIO_C),13, 1);
       delay(1000);
       //kprintf("blink\n");
    }

	return 0;
}