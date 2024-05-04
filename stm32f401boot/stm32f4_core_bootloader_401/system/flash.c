#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
#include <memory.h>
#include <kernel.h>
#include <delay.h>
#include <uart.h>
#include <stdlib.h>
#include "fat_filelib.h"
#include <flash.h>
 
 
void unlock_flash() {
    if (FLASH->CR & FLASH_CR_LOCK) {
        FLASH->KEYR = KEY1;
        FLASH->KEYR = KEY2;
    }
}

void lock_flash() {
    FLASH->CR |= FLASH_CR_LOCK; // bit 31
}


void erase_flash_sector(uint32_t sec) {
   irq_disable();
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR |= FLASH_CR_SER;
    FLASH->CR |= (sec << 3); // SNB bit 3:6
    FLASH->CR |= FLASH_CR_STRT; // start
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR &= ~FLASH_CR_SER; // clear SER bit
    FLASH->CR &= ~(0xFU << 3); // clear SNB bit 3:6
    irq_enable();
}

void erase_flash_sector3() {
    const uint32_t sec = 3;
    irq_disable();
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR |= FLASH_CR_SER;
    FLASH->CR |= (sec << 3); // SNB bit 3:6
    FLASH->CR |= FLASH_CR_STRT; // start
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR &= ~FLASH_CR_SER; // clear SER bit
    FLASH->CR &= ~(0xFU << 3); // clear SNB bit 3:6
    irq_enable();
}
void erase_flash_sector4() {     //64
    const uint32_t sec = 4;
    irq_disable();
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR |= FLASH_CR_SER;
    FLASH->CR |= (sec << 3); // SNB bit 3:6
    FLASH->CR |= FLASH_CR_STRT; // start
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR &= ~FLASH_CR_SER; // clear SER bit
    FLASH->CR &= ~(0xFU << 3); // clear SNB bit 3:6
    irq_enable();
}

void erase_flash_sector5() {     //128
    const uint32_t sec = 5;
    irq_disable();
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR |= FLASH_CR_SER;
    FLASH->CR |= (sec << 3); // SNB bit 3:6
    FLASH->CR |= FLASH_CR_STRT; // start
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR &= ~FLASH_CR_SER; // clear SER bit
    FLASH->CR &= ~(0xFU << 3); // clear SNB bit 3:6
    irq_enable();
}

void erase_flash_sector6() {      //128
    const uint32_t sec = 6;
    irq_disable();
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR |= FLASH_CR_SER;
    FLASH->CR |= (sec << 3); // SNB bit 3:6
    FLASH->CR |= FLASH_CR_STRT; // start
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR &= ~FLASH_CR_SER; // clear SER bit
    FLASH->CR &= ~(0xFU << 3); // clear SNB bit 3:6
    irq_enable();
}

void erase_flash_sector7() {     //128
    const uint32_t sec = 7;
    irq_disable();
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR |= FLASH_CR_SER;
    FLASH->CR |= (sec << 3); // SNB bit 3:6
    FLASH->CR |= FLASH_CR_STRT; // start
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR &= ~FLASH_CR_SER; // clear SER bit
    FLASH->CR &= ~(0xFU << 3); // clear SNB bit 3:6
    irq_enable();
}

void erase_flash_sector8() {       //128
    const uint32_t sec = 8;
    irq_disable();
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR |= FLASH_CR_SER;
    FLASH->CR |= (sec << 3); // SNB bit 3:6
    FLASH->CR |= FLASH_CR_STRT; // start
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR &= ~FLASH_CR_SER; // clear SER bit
    FLASH->CR &= ~(0xFU << 3); // clear SNB bit 3:6
    irq_enable();
}

void erase_flash_sector9() {       //128
    const uint32_t sec = 9;
    irq_disable();
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR |= FLASH_CR_SER;
    FLASH->CR |= (sec << 3); // SNB bit 3:6
    FLASH->CR |= FLASH_CR_STRT; // start
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR &= ~FLASH_CR_SER; // clear SER bit
    FLASH->CR &= ~(0xFU << 3); // clear SNB bit 3:6
    irq_enable();
}

void erase_flash_sector10() {     //128
    const uint32_t sec = 10;
    irq_disable();
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR |= FLASH_CR_SER;
    FLASH->CR |= (sec << 3); // SNB bit 3:6
    FLASH->CR |= FLASH_CR_STRT; // start
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR &= ~FLASH_CR_SER; // clear SER bit
    FLASH->CR &= ~(0xFU << 3); // clear SNB bit 3:6
    irq_enable();
}

void erase_flash_sector11() {     //128
    const uint32_t sec = 11;
    irq_disable();
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR |= FLASH_CR_SER;
    FLASH->CR |= (sec << 3); // SNB bit 3:6
    FLASH->CR |= FLASH_CR_STRT; // start
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR &= ~FLASH_CR_SER; // clear SER bit
    FLASH->CR &= ~(0xFU << 3); // clear SNB bit 3:6
    irq_enable();
}

void erase_flash_sector12() {     //128
    const uint32_t sec = 12;
    irq_disable();
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR |= FLASH_CR_SER;
    FLASH->CR |= (sec << 3); // SNB bit 3:6
    FLASH->CR |= FLASH_CR_STRT; // start
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR &= ~FLASH_CR_SER; // clear SER bit
    FLASH->CR &= ~(0xFU << 3); // clear SNB bit 3:6
    irq_enable();
}

void write_flash(uint32_t addr, uint32_t data){
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR |= FLASH_CR_PG;
    FLASH->CR &= ~(0x3U << 8); // clear PSIZE bit 8:9
    FLASH->CR |= (0x2 << 8);   // program PSIZE
    *(volatile uint32_t*)addr = data;
    while(FLASH->SR & FLASH_SR_BSY); // check if busy
    FLASH->CR &= (~FLASH_CR_PG); // disable PG bit
}


char *getUrlTargetFileBoot(){
	//char *s=full_path("/config/kernel");
	FILE *fptr;
    char buff[64];
    int i=0;
	if ((fptr = fopen("/config/kernel","r")) == NULL){
		kprintf("Error! opening file\n");
		kprintf("jump app\n");
		return NULL;
	}
	fseek(fptr, 0, SEEK_END);
	int lsize = ftell(fptr);
	fseek(fptr, 0, SEEK_SET);
    char *b=&buff;// malloc(lsize+1);

    while(!feof(fptr)){
         buff[i]=fgetc(fptr);
         i++;
    }
	//fread(b,lsize,1, fptr);
    //b[lsize]=0;
    //b[lsize+1]=0;
	//kprintf("%s %s\n",t,b);
	fclose(fptr);
    return (char *)b;
}

void setUrlTargetFileBoot(char *str){
    FILE* fd;
    remove("/config/kernel");
    fd = fopen("/config/kernel", "w");
    if(!fd){
       kprintf(" found!\n" );
       return;
    }
    str[strlen(str)]='\0';
    fputs(str,fd);
    fputc(0x00,fd);
    fclose(fd);
}
