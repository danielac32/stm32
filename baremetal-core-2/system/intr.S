
.syntax unified
.thumb
/* intr.S - enable, disable, restore, halt, pause, (ARM) */

	.text
	.globl	disable
	.globl	restore
	.globl	enable
	.globl	pause
	.globl	halt
	.globl _data_start
	.globl _data_end

/*------------------------------------------------------------------------
 * disable  -  Disable interrupts and return the previous state
 *------------------------------------------------------------------------
 */
disable:
	mrs	r0, psr		/* Copy the CPSR into r0		*/
	cpsid	i		/* Disable interrupts			*/
	bx	lr		/* Return the CPSR			*/

/*------------------------------------------------------------------------
 * restore  -  Restore interrupts to value given by mask argument
   Cortex M3 hardware handles a lot, rewrite
 *------------------------------------------------------------------------
 */
restore:
	msr	psr_nzcvq, r0	/* Restore the CPSR			*/
	cpsie	i		
	bx 	lr		/* Return to caller			*/

/*------------------------------------------------------------------------
 * enable  -  Enable interrupts
 *------------------------------------------------------------------------
 */
enable:
	cpsie	i		/* Enable interrupts			*/
	bx	lr		/* Return				*/

/*------------------------------------------------------------------------
 * pause or halt  -  Place the processor in a hard loop
 *------------------------------------------------------------------------
 */
halt:
pause:
	cpsid	i		/* Disable interrupts		*/
dloop:	b	dloop		/* Dead loop			*/


.globl	__syscall
__syscall:
svc 0
nop
bx	lr

#.type isr_svcall, %function
#.global isr_svcall
#isr_svcall:
#mrs r0, msp
#blx svccall_handler_c
#bx lr


.globl	__syscall2
__syscall2:
push {r0-r11}
svc 1 
pop {r0-r11}
bx lr


.globl	__syscall3
__syscall3:
push {r0-r11}
pop {r0-r11}
svc 1 
bx lr


/*
.data
.align 4
_data_start:
	.incbin "../blink.bin"
_data_end:
*/


