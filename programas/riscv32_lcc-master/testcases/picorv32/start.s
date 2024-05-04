.globl main
li x2, 10000
jal x1, main
ebreak
nop


.globl exit
.align 4
exit:
li x17,10
ecall
ret