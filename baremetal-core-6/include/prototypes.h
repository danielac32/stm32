//#include "kernel.h"
#include <stdint.h>

extern	int32	initintc(void);
extern	int32	initevec(void);
extern	int32	set_evec(uint32, uint32);
extern	void	trap(int32);
extern  void  svccall_handler(uint32 *sp);
extern  void isr_svcall();
extern	uint32	ioerr(void);
extern	uint32	ionull(void);



extern  uint32	init(uint32	descrp);
extern	uint32	open(uint32, char *, char *);
extern	uint32	putc(uint32, char);
extern	uint32	read(uint32, char *, uint32);
extern	uint32	seek(uint32, uint32);
extern	uint32	getc(uint32);


extern void clkinit(void);
extern void clkhandler();
extern __attribute__ ((naked)) void resched(void);






