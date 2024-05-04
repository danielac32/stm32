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
#include <usb.h>
#include "usb_cdc.h"
#include "usb-msc.h"
#include "sd-spi.h"
#include "disk.h"
#include "fat_filelib.h"
#include <flash.h>

extern uint32_t SystemCoreClock;
//extern void init_kernel();
extern void check_msc();





void toggle(int m){
  for (int i = 0; i < m*2; ++i)
  {
    hw_toggle_pin(GPIOx(GPIO_C),13);
    delay(300);
  }
  hw_set_pin(GPIOx(GPIO_C),13, 1);
}


int main()
{
    
    hw_cfg_pin(GPIOx(GPIO_C),13,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(GPIO_A),0,GPIOCFG_MODE_INP | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_PUP);
    hw_set_pin(GPIOx(GPIO_C),13, 0);
    //hw_cfg_pin(GPIOx(GPIO_C),9,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(GPIO_A),8,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

    
    
    check_msc();
    

    
    uint32 size;
    again:
    size = sd_init();
    fl_init();
    if (fl_attach_media(sd_readsector, sd_writesector) != FAT_INIT_OK){
        //kprintf("ERROR: Failed to init file system\n");
        if (fl_format(size, "")){
            kprintf("format ok\n");
            goto again;
        }
      toggle(2);
      return -1;
    }
    

    /*if (fl_format(size, "")){
          while(1){
            hw_toggle_pin(GPIOx(GPIO_C),13);
            delay(50);
          }
    }*/
    fl_listdirectory("/");

    FILE *fptr;
    char bb[64];
    strcpy(bb,getUrlTargetFileBoot());
    char *file=&bb;

    if ((fptr = fopen(file,"r")) == NULL){
       //while(1){
         // hw_toggle_pin(GPIOx(GPIO_C),13);
          //delay(100);
       //}
       toggle(3);
       delay(1000);
       goto jump_app;
    }
    delay(1000);

    int part=0;
    fseek(fptr, 0, SEEK_END);
    int lsize = ftell(fptr);
    unlock_flash();
    if (lsize < 64000){
        erase_flash_sector4();
    }else if (lsize > 64000 && lsize < 192000){
        erase_flash_sector4();
        erase_flash_sector5();
    }else if (lsize > 192000 && lsize < 320000){
        erase_flash_sector4();
        erase_flash_sector5();
        erase_flash_sector6();
    }else if (lsize>320000 && lsize < 448000){
        erase_flash_sector4();
        erase_flash_sector5();
        erase_flash_sector7();
    }


    lock_flash();
    #define SIZEBUF 4
    uint8_t b[SIZEBUF];
    uint32_t hex=0;
    uint32_t count=0;
    fseek(fptr, 0, SEEK_SET);
    unlock_flash();
  
     //irq_enable();
    while(!feof(fptr)){
       fread(b,SIZEBUF,1, fptr);
       hex = *(uint32_t *)b;
       write_flash(FLASH_BASE2+count, hex);
       count+=SIZEBUF;
       hw_toggle_pin(GPIOx(GPIO_C),13);
    }
    lock_flash();
    fclose(fptr);

    //char *rmfile=getUrlTargetFileBoot();
    remove(file);

    
    static uint32_t jump_addr;
    jump_app:
    toggle(5);
    irq_disable();
    jump_addr = *((uint32_t*)(FLASH_BASE2+4u));

    // Disable all interrupts
    NVIC->ICER[0] = 0xFFFFFFFF;
    NVIC->ICER[1] = 0xFFFFFFFF;
    NVIC->ICER[2] = 0xFFFFFFFF;

    NVIC->ICPR[0] = 0xFFFFFFFF;
    NVIC->ICPR[1] = 0xFFFFFFFF;
    NVIC->ICPR[2] = 0xFFFFFFFF;

    SysTick->CTRL = 0;
    SCB->VTOR = FLASH_BASE2;
    __DSB();
    __ISB();
    __set_MSP((*(__IO uint32_t*)FLASH_BASE2));

    ((void (*) (void)) (jump_addr)) ();
    while(1);
 
    while(1){
         

       // if(usart_available()){
         //   kprintf("%c",usart_getc());
        //}





       /*if (!hw_get_pin(GPIOx(GPIO_A),0)){
           hw_set_pin(GPIOx(GPIO_C),13, 1);
           hw_set_pin(GPIOx(GPIO_C),9, 1);
           hw_set_pin(GPIOx(GPIO_A),8, 1);
       }*/
      
       hw_toggle_pin(GPIOx(GPIO_C),13);
       delay(1000);
       //hw_set_pin(GPIOx(GPIO_C),13, 0);
       //hw_set_pin(GPIOx(GPIO_C),9, 1);
       //hw_set_pin(GPIOx(GPIO_C),13, 1);
       //delay(1000);
       //kprintf("blink\n");
    }

    return 0;
}