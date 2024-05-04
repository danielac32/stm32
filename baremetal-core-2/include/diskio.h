
/*-----------------------------------------------------------------------/
/  Low level disk interface modlue include file   (C)ChaN, 2014          /
/-----------------------------------------------------------------------*/

#ifndef _DISKIO_DEFINED
#define _DISKIO_DEFINED

#ifdef __cplusplus
extern "C" {
#endif

#define _USE_WRITE	1	/* 1: Enable disk_write function */
#define _USE_IOCTL	1	/* 1: Enable disk_ioctl fucntion */

#define _SD_SPI 0
/* This option defines the SPI port to be used.
/
/   0: Default port (for devices with a single MSSP)
/   1: SPI 1
/   2: SPI 2 */




/* Status of Disk Functions */
typedef BYTE	DSTATUS;

/* Results of Disk Functions */
typedef enum {
	RES_OK = 0,		/* 0: Successful */
	RES_ERROR,		/* 1: R/W Error */
	RES_WRPRT,		/* 2: Write Protected */
	RES_NOTRDY,		/* 3: Not Ready */
	RES_PARERR		/* 4: Invalid Parameter */
} DRESULT;


/*---------------------------------------*/
/* Prototypes for disk control functions */

int sd_init(void);
int sd_writesector(uint32 sector, uint8 *buffer, uint32 sector_count);
int sd_readsector(uint32 sector, uint8 *buffer, uint32 sector_count);


int RAM_disk_status(void);
int RAM_disk_initialize(void);
int RAM_disk_read(unsigned char *buff, unsigned int sector, unsigned int count);
int RAM_disk_write(const unsigned char *buff, unsigned int sector, unsigned int count);
int RAM_disk_ioctl(unsigned char cmd, void *buff);



DSTATUS disk_initialize (BYTE pdrv);
DSTATUS disk_status (BYTE pdrv);
DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff);
DWORD get_fattime(void);

DRESULT read_flash(BYTE* buff, DWORD sector,BYTE len);
DRESULT write_flash(BYTE* buff, DWORD sector,BYTE len);

void __delay_ms__(unsigned long t);
void __delay_us__(unsigned long t);


/* Disk Status Bits (DSTATUS) */

#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */
#define STA_PROTECT		0x04	/* Write protected */


/* Command code for disk_ioctrl fucntion */

/* Generic command (Used by FatFs) */
#define CTRL_SYNC			0	/* Complete pending write process (needed at _FS_READONLY == 0) */
#define GET_SECTOR_COUNT	1	/* Get media size (needed at _USE_MKFS == 1) */
#define GET_SECTOR_SIZE		2	/* Get sector size (needed at _MAX_SS != _MIN_SS) */
#define GET_BLOCK_SIZE		3	/* Get erase block size (needed at _USE_MKFS == 1) */
#define CTRL_TRIM			4	/* Inform device that the data on the block of sectors is no longer used (needed at _USE_TRIM == 1) */

/* Generic command (Not used by FatFs) */
#define CTRL_POWER			5	/* Get/Set power status */
#define CTRL_LOCK			6	/* Lock/Unlock media removal */
#define CTRL_EJECT			7	/* Eject media */
#define CTRL_FORMAT			8	/* Create physical format on the media */

/* MMC/SDC specific ioctl command */
#define MMC_GET_TYPE		10	/* Get card type */
#define MMC_GET_CSD			11	/* Get CSD */
#define MMC_GET_CID			12	/* Get CID */
#define MMC_GET_OCR			13	/* Get OCR */
#define MMC_GET_SDSTAT		14	/* Get SD status */

/* ATA/CF specific ioctl command */
#define ATA_GET_REV			20	/* Get F/W revision */
#define ATA_GET_MODEL		21	/* Get model name */
#define ATA_GET_SN			22	/* Get serial number */


/*-------------------------------------*/
/* PIC18 SPI definitions and functions */

/* Note: sd_init() must open SPI at a speed between 100-400 KHz */

#define SYSCALL_DISK_INITIALIZE 1
#define SYSCALL_WRITE_SECTOR  2
#define SYSCALL_READ_SECTOR  3
#define SYSCALL_IOCTL 4
#define SYSCALL_STATUS 5
#ifdef __cplusplus
}
#endif

#endif

