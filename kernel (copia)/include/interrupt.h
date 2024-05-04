/* interrupt.h */

#define NVIC_STIR	(uint32 *) 0xE000EF00

/* System control block */
#define SCB_ICSR	(uint32 *) 0xE000ED04
#define PENDSV_INTR	28

/* System control block base */
//#define SCB_BASE	0xE000ED00

/* Faults enable */
#define	MPUFAULT_EN	16
#define	BUSFAULT_EN	17
#define USAGEFAULT_EN	18

/* Exception vector */
#define MEMFAULT_ENTRY		4
#define BUSFAULT_ENTRY		5
#define USAGEFAULT_ENTRY	6
#define SVC_ENTRY		11
#define PENDSV_ENTRY		14
#define SYSTICK_ENTRY		15
