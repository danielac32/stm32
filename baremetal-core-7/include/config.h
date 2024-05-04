
#include <stdint.h>
struct	dentry	{
	int32   dvnum;
	int32   dvminor;
	char    *dvname;
	uint32 (*dvinit) (struct dentry *);
	uint32 (*dvopen) (struct dentry *, char *, char *);
	uint32 (*dvclose)(struct dentry *);
	uint32 (*dvread) (struct dentry *, void *, uint32);
	uint32 (*dvwrite)(struct dentry *, void *, uint32);
	uint32 (*dvseek) (struct dentry *, int32);
	uint32 (*dvgetc) (struct dentry *);
	uint32 (*dvputc) (struct dentry *, char);
	uint32 (*dvcntl) (struct dentry *, int32, int32, int32);
	void    *dvcsr;
	void    (*dvintr)(void);
	byte    dvirq;
};

extern	struct	dentry	devtab[]; /* one entry per device */

/* Device name definitions */

#define CONSOLE              0	/* type tty      */
#define GPIO0                1	/* type gpio     */
#define GPIO1                2	/* type gpio     */
#define GPIO2                3	/* type gpio     */
#define GPIO3                4	/* type gpio     */
#define NULLDEV              5	/* type null     */
#define NAMESPACE            6	/* type nam      */
//#define SPI1                 7	/* type spi      */
#define RAM0                 8	/* type ram      */

/* Control block sizes */

#define	Nnull	1
#define	Ngpio	4
#define	Ntty	1
#define	Nram	1
#define	Nnam	1
#define	Nspi	1

#define NDEVS 9


/* Configuration and Size Constants */

#define	NPROC	     10		/* number of user processes		*/
#define	NSEM	     10		/* number of semaphores			*/
#define	IRQBASE	     32		/* base ivec for IRQ0			*/
//#define	IRQ_TIMER    IRQ_HW5	/* timer IRQ is wired to hardware 5	*/
//#define	IRQ_ATH_MISC IRQ_HW4	/* Misc. IRQ is wired to hardware 4	*/
//#define CLKFREQ      200000000	/* 200 MHz clock			*/

//#define	LF_DISK_DEV	RAM0


#define	DEVNAMLEN	16	/* maximum size of a device name	*/

/* Macro used to verify device ID is valid  */

#define isbaddev(f)  ( ((f) < 0) | ((f) >= NDEVS) )
