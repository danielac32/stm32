	.align 4
	.text
	.align	4
cvt:
	addi x2,x2,-80
	sw  x8,76(x2)
	addi  x8,x2,64
	sw x1,24(x2)
	sw x20,28(x2)
	sw x21,32(x2)
	sw x22,36(x2)
	sw x23,40(x2)
	sw x24,44(x2)
	sw x25,48(x2)
	sw x26,52(x2)
	sw x27,56(x2)
	mv x27,x12
	mv x26,x13
	mv x25,x14
	sw x15,28(x8)
	mv x24,x16
	mv x23,x17
	bge x26,x0,L.6
	addi x26,x0,0
L.6:
	li x30,79
	blt x26,x30,L.8
	li x26,78
L.8:
	addi x20,x0,0
	lw x29,12+16(x8)
	sw x0,0(x29)
	addi x21,x24,0
	addi x12,x27,0
	la x30,L.12
	lw x13,0(x30)
	jal x1,float32_ge
	bne x10,x0,L.10
	lw x30,12+16(x8)
	li x29,1
	sw x29,0(x30)
	addi x12,x27,0
	jal x1,float32_neg
	addi x27,x10,0
L.10:
	addi x12,x27,0
	addi x13,x8,-12+16
	jal x1,modf
	addi x27,x10,0
	li x30,80
	add x22,x24,x30
	lw x12,-12+16(x8)
	la x30,L.12
	lw x13,0(x30)
	jal x1,float32_eq
	bne x10,x0,L.13
	li x30,80
	add x22,x24,x30
	jal x0,L.16
L.15:
	lw x12,-12+16(x8)
	la x30,L.18
	lw x13,0(x30)
	jal x1,float32_div
	addi x30,x10,0
	addi x12,x30,0
	addi x13,x8,-12+16
	jal x1,modf
	sw x10,-8+16(x8)
	li x30,-1
	add x30,x22,x30
	sw x30,-16+16(x8)
	addi x22,x30,0
	lw x12,-8+16(x8)
	la x29,L.19
	lw x13,0(x29)
	jal x1,float32_add
	addi x30,x10,0
	la x29,L.18
	lw x12,0(x29)
	addi x13,x30,0
	jal x1,float32_mul
	addi x30,x10,0
	addi x12,x30,0
	jal x1,float32_to_int32
	addi x30,x10,48
	lw x29,-16+16(x8)
	sb x30,0(x29)
	addi x20,x20,1
L.16:
	lw x12,-12+16(x8)
	la x30,L.12
	lw x13,0(x30)
	jal x1,float32_ne
	bne x10,x0,L.15
	jal x0,L.21
L.20:
	addi x30,x21,0
	li x29,1
	add x21,x30,x29
	addi x28,x22,0
	add x22,x28,x29
	lb x29,0(x28)
	sb x29,0(x30)
L.21:
	addi x30,x22,0
	li x29,80
	add x29,x24,x29
	bltu x30,x29,L.20
	jal x0,L.14
L.13:
	addi x12,x27,0
	la x30,L.12
	lw x13,0(x30)
	jal x1,float32_le
	bne x10,x0,L.23
	jal x0,L.26
L.25:
	lw x27,-8+16(x8)
	addi x20,x20,-1
L.26:
	la x30,L.18
	lw x12,0(x30)
	addi x13,x27,0
	jal x1,float32_mul
	sw x10,-8+16(x8)
	addi x12,x10,0
	la x30,L.28
	lw x13,0(x30)
	jal x1,float32_lt
	bne x10,x0,L.25
L.23:
L.14:
	add x22,x26,x24
	bne x23,x0,L.29
	add x22,x20,x22
L.29:
	sw x20,0(x25)
	addi x30,x22,0
	addi x29,x24,0
	bgeu x30,x29,L.34
	sb x0,0(x24)
	addi x10,x24,0
	jal x0,L.5
L.33:
	la x30,L.18
	lw x12,0(x30)
	addi x13,x27,0
	jal x1,float32_mul
	addi x30,x10,0
	addi x27,x30,0
	addi x12,x27,0
	addi x13,x8,-8+16
	jal x1,modf
	addi x27,x10,0
	addi x30,x21,0
	sw x30,-16+16(x8)
	li x29,1
	add x21,x30,x29
	lw x12,-8+16(x8)
	jal x1,float32_to_int32
	addi x30,x10,48
	lw x29,-16+16(x8)
	sb x30,0(x29)
L.34:
	addi x29,x22,0
	bgtu x21,x29,L.36
	li x29,80
	add x29,x24,x29
	bltu x21,x29,L.33
L.36:
	addi x30,x22,0
	li x29,80
	add x29,x24,x29
	bltu x30,x29,L.37
	li x30,79
	add x30,x24,x30
	sb x0,0(x30)
	addi x10,x24,0
	jal x0,L.5
L.37:
	addi x21,x22,0
	lb x30,0(x22)
	addi x30,x30,5
	sb x30,0(x22)
	jal x0,L.40
L.39:
	li x30,48
	sb x30,0(x22)
	addi x30,x22,0
	addi x29,x24,0
	bleu x30,x29,L.42
	li x30,-1
	add x30,x22,x30
	addi x22,x30,0
	lb x29,0(x30)
	addi x29,x29,1
	sb x29,0(x30)
	jal x0,L.43
L.42:
	li x30,49
	sb x30,0(x22)
	lw x30,0(x25)
	addi x30,x30,1
	sw x30,0(x25)
	bne x23,x0,L.44
	addi x30,x21,0
	addi x29,x24,0
	bleu x30,x29,L.46
	li x30,48
	sb x30,0(x21)
L.46:
	li x30,1
	add x21,x21,x30
L.44:
L.43:
L.40:
	lb x30,0(x22)
	li x29,57
	bgt x30,x29,L.39
	sb x0,0(x21)
	addi x10,x24,0
L.5:
	lw x1,24(x2)
	lw x20,28(x2)
	lw x21,32(x2)
	lw x22,36(x2)
	lw x23,40(x2)
	lw x24,44(x2)
	lw x25,48(x2)
	lw x26,52(x2)
	lw x27,56(x2)
	lw  x8,76(x2)
	addi  x2,x2,80
	jalr x0,x1,0

	.globl ecvt
	.align	4
ecvt:
	addi x2,x2,-48
	sw  x8,44(x2)
	addi  x8,x2,32
	sw x1,24(x2)
	sw x12,16(x8)
	sw x13,20(x8)
	sw x14,24(x8)
	sw x15,28(x8)
	lw x12,0+16(x8)
	lw x13,4+16(x8)
	lw x14,8+16(x8)
	lw x15,12+16(x8)
	la x16,CVTBUF
	li x17,1
	jal x1,cvt
	addi x30,x10,0
L.48:
	lw x1,24(x2)
	lw  x8,44(x2)
	addi  x2,x2,48
	jalr x0,x1,0

	.globl ecvtbuf
	.align	4
ecvtbuf:
	addi x2,x2,-48
	sw  x8,44(x2)
	addi  x8,x2,32
	sw x1,24(x2)
	sw x12,16(x8)
	sw x13,20(x8)
	sw x14,24(x8)
	sw x15,28(x8)
	sw x16,32(x8)
	lw x12,0+16(x8)
	lw x13,4+16(x8)
	lw x14,8+16(x8)
	lw x15,12+16(x8)
	lw x16,16+16(x8)
	li x17,1
	jal x1,cvt
	addi x30,x10,0
L.49:
	lw x1,24(x2)
	lw  x8,44(x2)
	addi  x2,x2,48
	jalr x0,x1,0

	.globl fcvt
	.align	4
fcvt:
	addi x2,x2,-48
	sw  x8,44(x2)
	addi  x8,x2,32
	sw x1,24(x2)
	sw x12,16(x8)
	sw x13,20(x8)
	sw x14,24(x8)
	sw x15,28(x8)
	lw x12,0+16(x8)
	lw x13,4+16(x8)
	lw x14,8+16(x8)
	lw x15,12+16(x8)
	la x16,CVTBUF
	addi x17,x0,0
	jal x1,cvt
	addi x30,x10,0
L.50:
	lw x1,24(x2)
	lw  x8,44(x2)
	addi  x2,x2,48
	jalr x0,x1,0

	.globl fcvtbuf
	.align	4
fcvtbuf:
	addi x2,x2,-48
	sw  x8,44(x2)
	addi  x8,x2,32
	sw x1,24(x2)
	sw x12,16(x8)
	sw x13,20(x8)
	sw x14,24(x8)
	sw x15,28(x8)
	sw x16,32(x8)
	lw x12,0+16(x8)
	lw x13,4+16(x8)
	lw x14,8+16(x8)
	lw x15,12+16(x8)
	lw x16,16+16(x8)
	addi x17,x0,0
	jal x1,cvt
	addi x30,x10,0
L.51:
	lw x1,24(x2)
	lw  x8,44(x2)
	addi  x2,x2,48
	jalr x0,x1,0

	.align 4
	.bss
	.align	1
CVTBUF:
	.space	80
	.align 4
	.data
	.align	4
L.28:
	.word	0x3f800000
	.align	4
L.19:
	.word	0x3cf5c28f
	.align	4
L.18:
	.word	0x41200000
	.align	4
L.12:
	.word	0x0
	.align 4
