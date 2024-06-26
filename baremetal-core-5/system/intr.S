
  .syntax unified
  .cpu cortex-m4
  .fpu softvfp
  .thumb
  
.text
	.globl	irq_disable
	.globl	irq_restore
	.globl	irq_enable
	.globl	pause
	.globl	halt



/*------------------------------------------------------------------------
 * disable  -  Disable interrupts and return the previous state
 *------------------------------------------------------------------------
 */
irq_disable:
	mrs	r0, psr		/* Copy the CPSR into r0		*/
	cpsid	i		/* Disable interrupts			*/
	bx	lr		/* Return the CPSR			*/

/*------------------------------------------------------------------------
 * restore  -  Restore interrupts to value given by mask argument
   Cortex M3 hardware handles a lot, rewrite
 *------------------------------------------------------------------------
 */
irq_restore:
	msr	psr_nzcvq, r0	/* Restore the CPSR			*/
	cpsie	i		
	bx 	lr		/* Return to caller			*/

/*------------------------------------------------------------------------
 * enable  -  Enable interrupts
 *------------------------------------------------------------------------
 */
irq_enable:
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


.globl	syscall
syscall:
svc 0
nop
bx	lr


.thumb_func
.global     save_context
.type       save_context, %function
save_context:
    mrs     r0, psp
    STMDB   r0!, {r4-r11}
    msr     psp, r0
    bx      lr


.thumb_func
.global     restore_context
.type       restore_context, %function
restore_context:
    mrs     r0, psp
    LDMFD   r0!, {r4-r11} 
    msr     psp, r0
    bx      lr


.thumb_func
.globl     SVCall_Handler
.type       SVCall_Handler, %function
SVCall_Handler:
    mrs     r0, psp

    push    {lr}
    push    {r0}
    bl      save_context
    pop     {r0}
    bl      svccall_handler
    bl      restore_context
    pop     {lr}
    bx      lr



 