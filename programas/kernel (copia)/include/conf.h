#include "kernel.h"

struct	dentry	{
	int32   dvnum;
	int32   dvminor;
	char    *dvname;
	devcall (*dvinit) (struct dentry *);
	devcall (*dvopen) (struct dentry *, char *, char *);
	devcall (*dvclose)(struct dentry *);
	devcall (*dvread) (struct dentry *, void *, uint32);
	devcall (*dvwrite)(struct dentry *, void *, uint32);
	devcall (*dvseek) (struct dentry *, int32);
	devcall (*dvgetc) (struct dentry *);
	devcall (*dvputc) (struct dentry *, char);
	//devcall (*dvcntl) (struct dentry *, int32, int32, int32);
	//void    *dvcsr;
	//void    (*dvintr)(void);
	//byte    dvirq;
};




extern	struct	dentry	devtab[];
#define CONSOLE              0	/* type tty      */
#define GPIO                 1	/* type gpio     */
#define NAMESPACE            3	/* type nam      */

/* Control block sizes */

#define	Nnull	1
#define	Ngpio	4
#define	Ntty	1
#define	Nram	1
#define	Nnam	1
#define	Nspi	1

#define NDEVS 3


extern	devcall	ttyinit(struct dentry *);

extern	devcall	ttyputc(struct dentry *, char);

extern	devcall	ttyread(struct dentry *, char *, int32);

extern	devcall	ttywrite(struct dentry *, char *, int32);

extern	devcall	ttygetc(struct dentry *);


extern	devcall	gpioinit(struct dentry *);

extern	devcall	gpioread(struct dentry *, char *, int32);

extern	devcall	gpiowrite(struct dentry *, char *, int32);


extern devcall	namopen( struct dentry *devptr,char	*name,char	*mode);
extern	devcall	naminit();