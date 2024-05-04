

.align 4
	.text
	.globl main
	.align	4

lui x2, 0x1F
jal x1, main
main:
	addi x2,x2,-32
	sw  x8,28(x2)
	addi  x8,x2,16
	li x30,100
	sw x30,-8+16(x8)
	li x30,100
	sw x30,-12+16(x8)
	lw x30,-8+16(x8)
	lw x29,-12+16(x8)
	bne x30,x29,L.2
	li x30,0xFA002
	li x29,97
	sb x29,0(x30)
L.2:
	addi x10,x0,0
L.1:
	lw  x8,28(x2)
	addi  x2,x2,32
	jalr x0,x1,0

	.align 4

