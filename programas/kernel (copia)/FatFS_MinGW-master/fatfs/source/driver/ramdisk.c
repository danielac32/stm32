#include <string.h>
#include "ff.h"
#include "diskio.h"

#define CFG_RAMDISK_SIZE               (/*500 * 1024*/100000)
#define CFG_RAMDISK_SECTOR_SIZE        (512)
#define RAMDISK_SECTOR_TOTAL           ((CFG_RAMDISK_SIZE) / (CFG_RAMDISK_SECTOR_SIZE))

static unsigned char rambuf[RAMDISK_SECTOR_TOTAL][CFG_RAMDISK_SECTOR_SIZE];

int RAM_disk_status(void)
{
    return 0;
}

int RAM_disk_initialize(void)
{
    return 0;
}

int RAM_disk_read(unsigned char *buff, unsigned int sector, unsigned int count)
{
    memcpy(buff, &rambuf[sector], count * CFG_RAMDISK_SECTOR_SIZE);
    return 0;
}

int RAM_disk_write(const unsigned char *buff, unsigned int sector, unsigned int count)
{
    memcpy(&rambuf[sector], buff, count * CFG_RAMDISK_SECTOR_SIZE);
    return 0;
}

int RAM_disk_ioctl(unsigned char cmd, void *buff)
{
    int res;

    switch (cmd) {
    case CTRL_SYNC:
        res = RES_OK;
        break;
    case GET_SECTOR_SIZE:
        *(DWORD *)buff = CFG_RAMDISK_SECTOR_SIZE;
        res = RES_OK;
        break;
    case GET_BLOCK_SIZE:
        *(WORD *)buff = 1;
        res = RES_OK;
        break;
    case GET_SECTOR_COUNT:
        *(DWORD *)buff = RAMDISK_SECTOR_TOTAL;
        res = RES_OK;
        break;
    default:
        res = RES_PARERR;
        break;
    }

    return res;
}

DWORD get_fattime(void)
{
	return 0;
}

