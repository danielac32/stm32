#include "os.h"

unsigned char buff[512];
unsigned char test_area[20000];
#define USER_DATA_FIELD_START 0




static int _read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    unsigned int address;
    address = c->block_size * block + off + USER_DATA_FIELD_START;
    memcpy(buffer, &test_area[address], size);
    //flash_read(address, buffer, size);
    return 0;
}

static int _prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    unsigned int address;
    address = c->block_size * block + off + USER_DATA_FIELD_START;
    //flash_write(address, (void *)buffer, size);
     memcpy(&test_area[address], buffer, size);
    return 0;
}

static int _erase(const struct lfs_config *c, lfs_block_t block)
{
    unsigned int address;
    address = c->block_size * block + USER_DATA_FIELD_START;
   // flash_erase(address);
    memset(&test_area[address], 0xFF, c->block_size);
    return 0;
}

int _sync(const struct lfs_config *c)
{
    return 0;
}

#define LFS_CACHE_SIZE      256
unsigned char read_buf[LFS_CACHE_SIZE];
unsigned char prog_buf[LFS_CACHE_SIZE];
unsigned char file_cache[LFS_CACHE_SIZE];
unsigned char lookahead_buf[4];
littlefs_t f030_fs;

struct lfs_config lfs_cfg = {
    // block device operations
    .read  = &_read,
    .prog  = &_prog,
    .erase = &_erase,
    .sync  = &_sync,

    // block device configuration
    .read_size = LFS_CACHE_SIZE,
    .prog_size = LFS_CACHE_SIZE,
    .block_size = 256,
    .block_count = 32,
    .cache_size = LFS_CACHE_SIZE,
    .lookahead_size = 4,
    .block_cycles = 100,
    .read_buffer = (void*)read_buf,
    .prog_buffer = (void*)prog_buf,
    .lookahead_buffer = (void*)lookahead_buf,
};

struct lfs_file_config lfs_file_cfg = {
    .buffer = (void*)file_cache,
    .attrs = NULL,
    .attr_count = 0
};


 




FATFS FatFs;   
FIL Fil;     
FILINFO fno;
FIL file;  


 

unsigned char Buffer[512];
unsigned int tmp;
unsigned int Clustervar;
unsigned char Dir_Attrib = 0;
unsigned long Size = 0;

int fs(){

/*
printf("iniciando ...\n");
uint32_t t = (uint32_t)timer_now2();

timer_sleep_us(10);


uint32_t t2 = (uint32_t)timer_now2();
printf("time curr : %d\n",t2 - t );*/

 


   
/*
   printf("iniciando sdcard\n");

  if (!mmc_init() && !fat_init()){
    
     printf("sdcard ok\n");



     for (int a = 1;a < 1024;a++){
          Clustervar = fat_read_dir_ent(0,a,&Size,&Dir_Attrib,Buffer);
          if(Clustervar == 0xffff){

              break;
          }
          printf("file: %s\n", (char *)Buffer);
     }
}else{
  printf("fail sd\n");
}

*/

/*
char buff[512];

DSTATUS res = disk_initialize(0);
printf("result %d\n",res );


for (int i = 0; i < 10; ++i)
{
  printf("\nsector: %d\n",i);
  disk_read(0,buff,i,1);
  hexdump(buff,512,true);
}
 */




 
 /*
    
 int ret;
    littlefs_init(&f030_fs, &lfs_cfg, &lfs_file_cfg);
    printf("filesystem_init: littlefs mount                  ");
    ret = f030_fs.mount2(f030_fs.lfs, &lfs_cfg);
    if(ret != LFS_ERR_OK)
    {
        printf("[FAIL]\n");
        printf("filesystem_init: STM32F030CCT6 Flash format      ");
        if(f030_fs.format(f030_fs.lfs, &lfs_cfg)) 
        {
            printf("[FAIL]\n");
            while(1);
        }
        printf("[ OK ]\n");
        printf("filesystem_init: littlefs remount                ");
        f030_fs.mount2(f030_fs.lfs, &lfs_cfg);
    }
    printf("[ OK ]\n");
    printf("filesystem_init: Flash available size: %d Bytes\n", f030_fs.free());
 
*/
 

uint32_t mask=disable();
printf("iniciando sdcard\n");
if (f_mount(&FatFs, "", 1) == FR_OK) { 
    if(f_open(&file,"config.txt",FA_OPEN_ALWAYS | FA_READ | FA_WRITE)==FR_OK){
       f_lseek(&file, 0);
       f_printf(&file,"%S\n","123456789abcdefghi");
       f_printf(&file,"%S\n","123456789abcdefghi");
       f_printf(&file,"%S\n","123456789abcdefghi");
       f_printf(&file,"%S\n","123456789abcdefghi");
       f_printf(&file,"%S\n","123456789abcdefghi");
       f_close(&file);
    }else printf("error file\n");
}else{
    printf("error sd\n");
}
restore(mask);
return 0;
}

