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
#include <littlefs.h>
#include <process.h>

littlefs_t disk;
extern void create_task();


//extern const spi_flash_drv_t flash_drv;
//#define SIZE   512

int main()
{
	
    /*uint32_t flash_size;
    uint32_t page_size;
    uint32_t erase_sector_size;
    uint8_t cnt;
    static uint8_t buf1[SIZE];
    static uint8_t buf2[SIZE];*/


	hw_cfg_pin(GPIOx(GPIO_C),13,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(GPIO_A),0,GPIOCFG_MODE_INP | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_PUP);
    hw_set_pin(GPIOx(GPIO_C),13, 1);



    while(1){
        usbd_ep_read(&udev, CDC_RXD_EP, &fifo, CDC_DATA_SZ);

        if (!hw_get_pin(GPIOx(GPIO_A),0) || !strncmp(fifo,"ok",2)){
              //delay(500000);
            //usbd_ep_write(&udev, CDC_TXD_EP, "usb ok\n", 7);
              //kprintf("SPI FLASH: %d\n",flash_size);
              //kprintf("page size: %d erase sector size: %d\n",page_size,erase_sector_size);
              //delay(1000000/2);
            break;
        }
    } 
    
   #if 1
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

    kprintf("%10d bytes of code.\n",
        (uint32)&etext - (uint32)&text);
    kprintf("           [0x%08X to 0x%08X]\n",
        (uint32)&text, (uint32)&etext - 1);
    kprintf("%10d bytes of data.\n",
        (uint32)&ebss - (uint32)&data);
    kprintf("           [0x%08X to 0x%08X]\n\n",
        (uint32)&data, (uint32)&ebss - 1);  
    #endif

    int ret;
    int disk_free;
    //int start=cycleCount();
    ret=littlefs_init(&disk, &config, 0);
    if(ret) 
    {
        kprintf("[FAIL]\n");
        kprintf("Error Code: %d\n", ret);
        return -1;
    }
    kprintf("[ OK ]\n");
    #if 0
    kprintf("startup_test: format    ");
    ret = disk.format(disk.lfs, &config);
    if(ret) 
    {
        kprintf("[FAIL]\n");
        kprintf("Error Code: %d\n", ret);
        return -1;
    }
    kprintf("[ OK ]\n");
    #endif 
    kprintf("startup_test: mount     ");
    ret = disk.mount(disk.lfs, &config);
    if(ret) 
    {
        kprintf("[FAIL]\n");
        kprintf("Error Code: %d\n", ret);
        return -1;
    }

    kprintf("[ OK ]\n");
    disk_free = disk.free();
    //int end=cycleCount();
    //int res = end-start;
    //kprintf("in %d us \n",res);
    kprintf("startup_test: available size = %d Bytes\n", disk_free);
    //delay(10);
    //int res = end-start;
    //kprintf("%d us\n",res);


    /*disk.mkdir("/opt");
    disk.mkdir("/bin");
    disk.mkdir("/app");
    disk.mkdir("/shell");
     
    FILE *fs=disk.open("/boot.bin",LFS_O_CREAT);
    disk.write(buf1,SIZE,fs);
    disk.close(fs);
    fs=disk.open("/bootloader.bin",LFS_O_CREAT);
    disk.close(fs);
    fs=disk.open("/kernel.bin",LFS_O_CREAT);
    disk.close(fs);



    fs=disk.open("/bin/boot.bin",LFS_O_CREAT);
    disk.close(fs);
    fs=disk.open("/bin/bootloader.bin",LFS_O_CREAT);
    disk.close(fs);
    fs=disk.open("/bin/kernel.bin",LFS_O_CREAT);
    disk.close(fs);
    fs=disk.open("/bin/firmware.bin",LFS_O_CREAT);
    disk.close(fs);
    fs=disk.open("/bin/start.bin",LFS_O_CREAT);
    disk.close(fs);
    disk.mkdir("/bin/test");

    kprintf("[ OK ]\n");
    disk_free = disk.free();
    kprintf("startup_test: available size = %d Bytes\n", disk_free);*/

    struct lfs_info info;
    DIR *dir = disk.openDir("/");
    if(dir==0){
        kprintf("not a directory");
    }
    while (disk.readDir(dir, &info)> 0)
        if (strcmp(info.name, ".") && strcmp(info.name, ".."))
            if (info.type == LFS_TYPE_DIR)
                if ((info.name[0] != '.'))
                    kprintf(" %7d [%s]\n", info.size, info.name);

    disk.rewindDir(dir);
    while (disk.readDir(dir, &info)> 0)
        if (strcmp(info.name, ".") && strcmp(info.name, ".."))
            if (info.type == LFS_TYPE_REG)
                if ((info.name[0] != '.'))
                    kprintf(" %7d %s\n", info.size, info.name);

    disk.closeDir(dir);

    /*
    flash_drv.init();
    flash_size = flash_drv.get_flash_size();
    page_size = flash_drv.get_page_size();
    erase_sector_size = flash_drv.get_erase_sector_size();

    kprintf("SPI FLASH: %d\n",flash_size);
    kprintf("page size: %d erase sector size: %d\n",page_size,erase_sector_size);
    kprintf("Write and read %d bytes from 0 page...\n", SIZE);

    memset(buf1, 0xca, SIZE);
    memset(buf2, 0, SIZE);
    flash_drv.erase(0, erase_sector_size);
    for (cnt = 0; cnt < SIZE / page_size; cnt++)
    {
        flash_drv.write_page(buf1, page_size * cnt, page_size);
    }
    flash_drv.read(buf2, 0, SIZE);
    for (int i = 0; i < 32; ++i)
    {
        kprintf("%02x\n",buf2[i]);
    }
    if (!memcmp(buf1, buf2, SIZE))
    {
        kprintf("Data matched.\n");
    }
    else
    {
        kprintf("Failed: Read data differs from the data written.\n");
    }*/
    


    create_task();
    while(1){

       hw_set_pin(GPIOx(GPIO_C),13, 0);
       delay(1000);
       hw_set_pin(GPIOx(GPIO_C),13, 1);
       delay(1000000);
       //kprintf("blink\n");
    }

	return 0;
}