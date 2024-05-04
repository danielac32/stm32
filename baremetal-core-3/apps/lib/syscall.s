.text
.globl	__syscall
__syscall:
svc 0
nop
bx	lr