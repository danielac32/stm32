/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
//#include "ramdisk.h"
#include <string.h>








/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */




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


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv) {
	case DEV_RAM :
	    stat = RAM_disk_status();

		// translate the reslut code here

		return stat;

	case DEV_MMC :
		/// result = MMC_disk_status();

		// translate the reslut code here

		return stat;

	case DEV_USB :
		/// result = USB_disk_status();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv) {
	case DEV_RAM :
	    stat = RAM_disk_initialize();

		// translate the reslut code here

		return stat;

	case DEV_MMC :
		/// result = MMC_disk_initialize();

		// translate the reslut code here

		return stat;

	case DEV_USB :
		/// result = USB_disk_initialize();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res = RES_PARERR;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

	    res = RAM_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here

		/// result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

		/// result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = RES_PARERR;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

	    res = RAM_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here

		/// result = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

		/// result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_PARERR;

	switch (pdrv) {
	case DEV_RAM :
        res = RAM_disk_ioctl(cmd, buff);
		// Process of the command for the RAM drive

		return res;

	case DEV_MMC :

		// Process of the command for the MMC/SD card

		return res;

	case DEV_USB :

		// Process of the command the USB drive

		return res;
	}

	return RES_PARERR;
}

