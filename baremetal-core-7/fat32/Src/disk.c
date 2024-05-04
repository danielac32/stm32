#include <disk.h>


int sd_init(void)
{
    disk_initialize(0);
    return 0;
}
//-----------------------------------------------------------------
// sd_readsector: Read a number of blocks from SD card
//-----------------------------------------------------------------
int sd_readsector(uint32 start_block, uint8 *buffer, uint32 sector_count)
{
    disk_read (0, buffer, start_block, sector_count);
 
    return 1;
}
//-----------------------------------------------------------------
// sd_writesector: Write a number of blocks to SD card
//-----------------------------------------------------------------
int sd_writesector(uint32 start_block, uint8 *buffer, uint32 sector_count)
{
    disk_write (0, buffer, start_block, sector_count);
    
    return 1;
}




