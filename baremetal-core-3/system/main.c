#include <kernel.h>
#include <fat_filelib.h>
#include <stm32f401xc.h>

#define FLASH_APP_START_ADDRESS ((uint32)0x08010000)
typedef void (*fnc_ptr)(void);

void jump(){
	fnc_ptr jump_to_app;
    //uint32 r=readflash(FLASH_APP_START_ADDRESS);
    //uint32 r2=readflash(FLASH_APP_START_ADDRESS + 4u);
    //printf(">>>>%08x %08x\n", r,r2);
    //RCC->CIR = 0x00000000;
    //SCB->VTOR = FLASH_APP_START_ADDRESS | 0x00;
    //asm volatile ("MSR msp, %0\n" : : "r" (0x2001f400) : "sp");
    //asm volatile ("bx %0" : : "r" (FLASH_APP_START_ADDRESS+0x1ac));


    jump_to_app = (fnc_ptr)((uint32*)(FLASH_APP_START_ADDRESS));
    
    //int (*p)(void )=(void *)((uint32)FLASH_APP_START_ADDRESS +4u);
    //p();
    //jump_to_app = (fnc_ptr)(*(volatile uint32*) (0x08010000+4u) |1);
    jump_to_app();
}
int main()
{
     
	printf("main\n");
    /*printf("erase sector 4 \n");
    unlock_flash();
    erase_flash_sector(4);
	lock_flash();
    FILE *fd;

    if ((fd = syscall_fopen("/xinu.bin","r")) == NULL){
	   	panic("Error! opening");
	    return -1;
	}

	printf("\n");
    
    syscall_fseek(fd, 0, SEEK_END);
    unsigned int fileLength = syscall_ftell(fd);
    syscall_fseek(fd, 0, SEEK_SET);
    

    uint8 buf[4];
    uint32 offset = FLASH_APP_START_ADDRESS;
    uint32 r1,r2;
    unlock_flash();
    while(!syscall_feof(fd)){
    	syscall_fread(buf,4,1, fd);
        r1 = *(volatile uint32*)buf;
        writeflash(offset,r1);
        r2=readflash(offset);
        if(r1 != r2)panic("error escribiendo\n");
    	//printf("%08x\n",*(uint32*)buf );
    	//writeflash(offset,*(uint32*)buf);
    	offset+=4;
    }
    lock_flash();
    
   // SYS_ENTRY();
    enable_tasks=1;

    //hexDump2(0x08010000, (void *)0x08010000,fileLength);*/


    //hexDump2(FLASH_APP_START_ADDRESS, (void *)FLASH_APP_START_ADDRESS,64);
    jump();



    /*uint8 *Data = syscall_malloc(fileLength);

    if (syscall_fread((uint8 *)Data, fileLength,1,fd) != fileLength) {
        syscall_fclose(fd);
        syscall_free(Data);
        return -1;
    }

    
    hexDump2(0x08010000, Data,fileLength);


    syscall_fclose(fd);
    syscall_free(Data);*/


	/*
	printf("erased sector 4 \n");
    
    unlock_flash();
    writeflash(0x08010000,0xbebecafe);
    lock_flash();
    uint32 r=readflash(0x08010000);
    printf("%08x\n",r );*/

	return 0;
}