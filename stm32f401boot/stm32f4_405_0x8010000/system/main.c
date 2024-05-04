#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
#include <memory.h>
#include <kernel.h>
#include <delay.h>
#include <gpio.h>
#include <uart.h>
#include <process.h>
#include <syscall.h>
#include "sd-spi.h"
#include "disk.h"
#include "fat_filelib.h"
#include <flash.h>
#include <usb_cdc_conf.h>


extern uint32_t SystemCoreClock;
extern void init_kernel();
extern void check_msc();
syscall_t syscallp;


void toggle(int m){
  for (int i = 0; i < m*2; ++i)
  {
    hw_toggle_pin(GPIOx(GPIO_A),8);
    delay(300);
  }
  hw_set_pin(GPIOx(GPIO_A),8, 0);
}


int main()
{
   // kprintf("iniciando....\n");
    //hw_cfg_pin(GPIOx(GPIO_C),13,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(GPIO_A),0,GPIOCFG_MODE_INP | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_PUP);
    //hw_set_pin(GPIOx(GPIO_C),13, 0);
    //hw_cfg_pin(GPIOx(GPIO_C),9,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(GPIO_A),8,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);


    while(1){
        if(!hw_get_pin(GPIOx(GPIO_A),0) || usb_available()){
            if(usb_getc()){
                break;
            }
        }
        hw_toggle_pin(GPIOx(GPIO_A),8);
        delay(50);
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

    
    uint32 size;
    again:
    size = sd_init();
    fl_init();
    if (fl_attach_media(sd_readsector, sd_writesector) != FAT_INIT_OK){
        kprintf("ERROR: Failed to init file system\n");
        
      return -1;
    }
    
    fl_listdirectory("/");

    kprintf("file booted -> %s\n",getUrlTargetFileBoot());
    init_kernel();
    while(1);

    return 0;
}