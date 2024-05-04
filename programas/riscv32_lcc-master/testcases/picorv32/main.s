	.align 4
	.text
	.globl print
	.align	4
print:
	addi x2,x2,-32
	sw  x8,28(x2)
	addi  x8,x2,16
	jal x0,L.3
L.2:
	addi x30,x12,0
	li x29,1
	add x12,x30,x29
	li x29,0xfa002
	lb x30,0(x30)
	sb x30,0(x29)
L.3:
	lb x30,0(x12)
	bne x30,x0,L.2
L.1:
	lw  x8,28(x2)
	addi  x2,x2,32
	jalr x0,x1,0

	.globl xputc
	.align	4
xputc:
	addi x2,x2,-32
	sw  x8,28(x2)
	addi  x8,x2,16
	li x30,0xfa002
	sb x12,0(x30)
L.5:
	lw  x8,28(x2)
	addi  x2,x2,32
	jalr x0,x1,0

	.globl xputs
	.align	4
xputs:
	addi x2,x2,-64
	sw  x8,60(x2)
	addi  x8,x2,48
	sw x1,24(x2)
	sw x27,28(x2)
	mv x27,x12
	jal x0,L.8
L.7:
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	lb x12,0(x30)
	jal x1,xputc
L.8:
	lb x30,0(x27)
	bne x30,x0,L.7
L.6:
	lw x1,24(x2)
	lw x27,28(x2)
	lw  x8,60(x2)
	addi  x2,x2,64
	jalr x0,x1,0

	.align	4
xvprintf:
	addi x2,x2,-144
	sw  x8,140(x2)
	addi  x8,x2,80
	sw x1,24(x2)
	sw x18,28(x2)
	sw x19,32(x2)
	sw x20,36(x2)
	sw x21,40(x2)
	sw x22,44(x2)
	sw x23,48(x2)
	sw x24,52(x2)
	sw x25,56(x2)
	sw x26,60(x2)
	sw x27,64(x2)
	mv x27,x12
	mv x26,x13
L.11:
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	lb x23,0(x30)
	slli x30,x23,8*(4-1)
	srai x30,x30,8*(4-1)
	bne x30,x0,L.15
	jal x0,L.13
L.15:
	slli x30,x23,8*(4-1)
	srai x30,x30,8*(4-1)
	li x29,37
	beq x30,x29,L.17
	slli x12,x23,8*(4-1)
	srai x12,x12,8*(4-1)
	jal x1,xputc
	jal x0,L.11
L.17:
	addi x22,x0,0
	addi x18,x0,0
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	lb x23,0(x30)
	slli x30,x23,8*(4-1)
	srai x30,x30,8*(4-1)
	li x29,48
	bne x30,x29,L.19
	li x18,1
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	lb x23,0(x30)
	jal x0,L.20
L.19:
	slli x30,x23,8*(4-1)
	srai x30,x30,8*(4-1)
	li x29,45
	bne x30,x29,L.21
	li x18,2
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	lb x23,0(x30)
L.21:
L.20:
	slli x30,x23,8*(4-1)
	srai x30,x30,8*(4-1)
	li x29,42
	bne x30,x29,L.28
	li x30,4
	add x30,x26,x30
	addi x26,x30,0
	li x29,-4
	add x30,x30,x29
	lw x30,0(x30)
	sw x30,-44+64(x8)
	lw x30,-44+64(x8)
	bge x30,x0,L.25
	lw x30,-44+64(x8)
	sub x30,x0,x30
	sw x30,-44+64(x8)
	li x18,2
L.25:
	lw x30,-44+64(x8)
	addi x22,x30,0
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	lb x23,0(x30)
	jal x0,L.24
L.27:
	li x30,10
	mul x30,x30,x22
	slli x29,x23,8*(4-1)
	srai x29,x29,8*(4-1)
	add x30,x30,x29
	li x29,48
	sub x22,x30,x29
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	lb x23,0(x30)
L.28:
	slli x30,x23,8*(4-1)
	srai x30,x30,8*(4-1)
	li x29,48
	blt x30,x29,L.30
	li x29,57
	ble x30,x29,L.27
L.30:
L.24:
	slli x30,x23,8*(4-1)
	srai x30,x30,8*(4-1)
	li x29,108
	beq x30,x29,L.33
	li x29,76
	bne x30,x29,L.31
L.33:
	li x30,4
	or x18,x18,x30
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	lb x23,0(x30)
	slli x30,x23,8*(4-1)
	srai x30,x30,8*(4-1)
	li x29,108
	beq x30,x29,L.36
	li x29,76
	bne x30,x29,L.34
L.36:
	li x30,8
	or x18,x18,x30
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	lb x23,0(x30)
L.34:
L.31:
	slli x30,x23,8*(4-1)
	srai x30,x30,8*(4-1)
	bne x30,x0,L.37
	jal x0,L.13
L.37:
	addi x25,x23,0
	slli x30,x25,8*(4-1)
	srai x30,x30,8*(4-1)
	li x29,97
	blt x30,x29,L.39
	slli x30,x25,8*(4-1)
	srai x30,x30,8*(4-1)
	addi x30,x30,-32
	addi x25,x30,0
L.39:
	slli x30,x25,8*(4-1)
	srai x30,x30,8*(4-1)
	sw x30,-52+64(x8)
	lw x30,-52+64(x8)
	li x29,79
	beq x30,x29,L.58
	bgt x30,x29,L.62
L.61:
	lw x30,-52+64(x8)
	li x29,66
	beq x30,x29,L.57
	li x29,67
	beq x30,x29,L.56
	li x29,68
	beq x30,x29,L.59
	jal x0,L.41
L.62:
	lw x30,-52+64(x8)
	li x29,83
	beq x30,x29,L.44
	li x29,85
	beq x30,x29,L.59
	li x29,88
	beq x30,x29,L.60
	jal x0,L.41
L.44:
	li x30,4
	add x30,x26,x30
	addi x26,x30,0
	li x29,-4
	add x30,x30,x29
	lw x30,0(x30)
	sw x30,-48+64(x8)
	addi x20,x0,0
	jal x0,L.48
L.45:
L.46:
	li x30,1
	add x20,x20,x30
L.48:
	lw x30,-48+64(x8)
	add x30,x20,x30
	lb x30,0(x30)
	bne x30,x0,L.45
	jal x0,L.50
L.49:
	li x12,32
	jal x1,xputc
L.50:
	li x30,2
	and x30,x18,x30
	bne x30,x0,L.52
	addi x30,x20,0
	li x29,1
	add x20,x30,x29
	bltu x30,x22,L.49
L.52:
	lw x12,-48+64(x8)
	jal x1,xputs
	jal x0,L.54
L.53:
	li x12,32
	jal x1,xputc
L.54:
	addi x30,x20,0
	li x29,1
	add x20,x30,x29
	bltu x30,x22,L.53
	jal x0,L.11
L.56:
	li x30,4
	add x30,x26,x30
	addi x26,x30,0
	li x29,-4
	add x30,x30,x29
	lw x30,0(x30)
	slli x12,x30,8*(4-1)
	srai x12,x12,8*(4-1)
	jal x1,xputc
	jal x0,L.11
L.57:
	li x19,2
	jal x0,L.42
L.58:
	li x19,8
	jal x0,L.42
L.59:
	li x19,10
	jal x0,L.42
L.60:
	li x19,16
	jal x0,L.42
L.41:
	slli x12,x23,8*(4-1)
	srai x12,x12,8*(4-1)
	jal x1,xputc
	jal x0,L.11
L.42:
	li x30,8
	and x30,x18,x30
	beq x30,x0,L.63
	li x30,4
	add x30,x26,x30
	addi x26,x30,0
	li x29,-4
	add x30,x30,x29
	lw x30,0(x30)
	sw x30,-40+64(x8)
	jal x0,L.64
L.63:
	li x30,4
	and x30,x18,x30
	beq x30,x0,L.65
	slli x30,x25,8*(4-1)
	srai x30,x30,8*(4-1)
	li x29,68
	bne x30,x29,L.68
	li x30,4
	add x30,x26,x30
	addi x26,x30,0
	li x29,-4
	add x30,x30,x29
	lw x30,0(x30)
	sw x30,-56+64(x8)
	jal x0,L.69
L.68:
	li x30,4
	add x30,x26,x30
	addi x26,x30,0
	li x29,-4
	add x30,x30,x29
	lw x30,0(x30)
	sw x30,-56+64(x8)
L.69:
	lw x30,-56+64(x8)
	sw x30,-40+64(x8)
	jal x0,L.66
L.65:
	slli x30,x25,8*(4-1)
	srai x30,x30,8*(4-1)
	li x29,68
	bne x30,x29,L.71
	li x30,4
	add x30,x26,x30
	addi x26,x30,0
	li x29,-4
	add x30,x30,x29
	lw x30,0(x30)
	sw x30,-56+64(x8)
	jal x0,L.72
L.71:
	li x30,4
	add x30,x26,x30
	addi x26,x30,0
	li x29,-4
	add x30,x30,x29
	lw x30,0(x30)
	sw x30,-56+64(x8)
L.72:
	lw x30,-56+64(x8)
	sw x30,-40+64(x8)
L.66:
L.64:
	slli x30,x25,8*(4-1)
	srai x30,x30,8*(4-1)
	li x29,68
	bne x30,x29,L.73
	lw x30,-40+64(x8)
	bge x30,x0,L.73
	lw x30,-40+64(x8)
	sub x30,x0,x30
	sw x30,-40+64(x8)
	li x30,16
	or x18,x18,x30
L.73:
	addi x24,x0,0
	lw x30,-40+64(x8)
	addi x21,x30,0
L.75:
	remu x30,x21,x19
	addi x25,x30,0
	divu x21,x21,x19
	slli x30,x25,8*(4-1)
	srai x30,x30,8*(4-1)
	li x29,9
	ble x30,x29,L.78
	slli x30,x23,8*(4-1)
	srai x30,x30,8*(4-1)
	li x29,120
	bne x30,x29,L.81
	li x30,39
	sw x30,-56+64(x8)
	jal x0,L.82
L.81:
	li x30,7
	sw x30,-56+64(x8)
L.82:
	slli x30,x25,8*(4-1)
	srai x30,x30,8*(4-1)
	lw x29,-56+64(x8)
	add x30,x30,x29
	addi x25,x30,0
L.78:
	addi x30,x24,0
	li x29,1
	add x24,x30,x29
	addi x29,x8,-36+64
	add x30,x30,x29
	slli x29,x25,8*(4-1)
	srai x29,x29,8*(4-1)
	addi x29,x29,48
	sb x29,0(x30)
L.76:
	beq x21,x0,L.83
	li x30,32
	bltu x24,x30,L.75
L.83:
	li x30,16
	and x30,x18,x30
	beq x30,x0,L.84
	addi x30,x24,0
	li x29,1
	add x24,x30,x29
	addi x29,x8,-36+64
	add x30,x30,x29
	li x29,45
	sb x29,0(x30)
L.84:
	addi x20,x24,0
	li x30,1
	and x30,x18,x30
	beq x30,x0,L.87
	li x30,48
	sw x30,-56+64(x8)
	jal x0,L.88
L.87:
	li x30,32
	sw x30,-56+64(x8)
L.88:
	lw x30,-56+64(x8)
	addi x25,x30,0
	jal x0,L.90
L.89:
	slli x12,x25,8*(4-1)
	srai x12,x12,8*(4-1)
	jal x1,xputc
L.90:
	li x30,2
	and x30,x18,x30
	bne x30,x0,L.92
	addi x30,x20,0
	li x29,1
	add x20,x30,x29
	bltu x30,x22,L.89
L.92:
L.93:
	li x30,1
	sub x30,x24,x30
	addi x24,x30,0
	addi x29,x8,-36+64
	add x30,x30,x29
	lb x12,0(x30)
	jal x1,xputc
L.94:
	bne x24,x0,L.93
	jal x0,L.97
L.96:
	li x12,32
	jal x1,xputc
L.97:
	addi x30,x20,0
	li x29,1
	add x20,x30,x29
	bltu x30,x22,L.96
	jal x0,L.11
L.13:
L.10:
	lw x1,24(x2)
	lw x18,28(x2)
	lw x19,32(x2)
	lw x20,36(x2)
	lw x21,40(x2)
	lw x22,44(x2)
	lw x23,48(x2)
	lw x24,52(x2)
	lw x25,56(x2)
	lw x26,60(x2)
	lw x27,64(x2)
	lw  x8,140(x2)
	addi  x2,x2,144
	jalr x0,x1,0

	.globl printf
	.align	4
printf:
	addi x2,x2,-48
	sw  x8,44(x2)
	addi  x8,x2,32
	sw x1,24(x2)
	sw x12,16(x8)
	sw x13,20(x8)
	sw x14,24(x8)
	sw x15,28(x8)
	sw x16,32(x8)
	sw x17,36(x8)
	addi x30,x8,4+16
	sw x30,-8+16(x8)
	lw x12,0+16(x8)
	lw x13,-8+16(x8)
	jal x1,xvprintf
L.99:
	lw x1,24(x2)
	lw  x8,44(x2)
	addi  x2,x2,48
	jalr x0,x1,0

	.globl sprintf
	.align	4
sprintf:
	addi x2,x2,-48
	sw  x8,44(x2)
	addi  x8,x2,32
	sw x1,24(x2)
	sw x12,16(x8)
	sw x13,20(x8)
	sw x14,24(x8)
	sw x15,28(x8)
	sw x16,32(x8)
	sw x17,36(x8)
	la x30,outptr
	lw x29,0+16(x8)
	sw x29,0(x30)
	addi x30,x8,8+16
	sw x30,-8+16(x8)
	lw x12,4+16(x8)
	lw x13,-8+16(x8)
	jal x1,xvprintf
	la x30,outptr
	lw x30,0(x30)
	sb x0,0(x30)
	la x30,outptr
	sw x0,0(x30)
L.102:
	lw x1,24(x2)
	lw  x8,44(x2)
	addi  x2,x2,48
	jalr x0,x1,0

	.align 4
	.data
	.globl s
	.align	4
s:
	.word	L.105
	.globl fib
	.align 4
	.text
	.align	4
fib:
	addi x2,x2,-48
	sw  x8,44(x2)
	addi  x8,x2,32
	sw x24,0(x2)
	sw x25,4(x2)
	sw x26,8(x2)
	sw x27,12(x2)
	addi x26,x0,0
	li x27,1
	li x25,1
	jal x0,L.110
L.107:
	add x24,x26,x27
	addi x26,x27,0
	addi x27,x24,0
L.108:
	addi x25,x25,1
L.110:
	ble x25,x12,L.107
	addi x10,x26,0
L.106:
	lw x24,0(x2)
	lw x25,4(x2)
	lw x26,8(x2)
	lw x27,12(x2)
	lw  x8,44(x2)
	addi  x2,x2,48
	jalr x0,x1,0

	.globl main
	.align	4
main:
	addi x2,x2,-64
	sw  x8,60(x2)
	addi  x8,x2,48
	sw x1,24(x2)
	sw x25,28(x2)
	sw x26,32(x2)
	sw x27,36(x2)
	addi x27,x0,0
	addi x25,x0,0
	li x26,1
	addi x27,x0,0
L.112:
	la x12,L.116
	la x30,s
	lw x13,0(x30)
	addi x14,x27,0
	jal x1,printf
L.113:
	addi x27,x27,1
	li x30,10
	blt x27,x30,L.112
	addi x27,x0,0
L.117:
	addi x12,x27,0
	jal x1,fib
	addi x30,x10,0
	la x12,L.121
	addi x13,x27,0
	addi x14,x30,0
	jal x1,printf
L.118:
	addi x27,x27,1
	li x30,10
	blt x27,x30,L.117
	add x30,x26,x26
	add x30,x26,x30
	add x30,x26,x30
	add x30,x26,x30
	add x30,x26,x30
	add x25,x26,x30
	la x12,L.122
	addi x13,x25,0
	jal x1,printf
	jal x1,exit
L.123:
L.124:
	jal x0,L.123
	addi x10,x0,0
L.111:
	lw x1,24(x2)
	lw x25,28(x2)
	lw x26,32(x2)
	lw x27,36(x2)
	lw  x8,60(x2)
	addi  x2,x2,64
	jalr x0,x1,0

	.align 4
	.bss
	.align	4
outptr:
	.space	4
	.align 4
	.data
	.align	1
L.122:
	.byte	0x25
	.byte	0x64
	.byte	0xa
	.byte	0x0
	.align	1
L.121:
	.byte	0x25
	.byte	0x64
	.byte	0x29
	.byte	0x20
	.byte	0x66
	.byte	0x69
	.byte	0x62
	.byte	0x3a
	.byte	0x20
	.byte	0x25
	.byte	0x64
	.byte	0xa
	.byte	0x0
	.align	1
L.116:
	.byte	0x25
	.byte	0x73
	.byte	0x20
	.byte	0x25
	.byte	0x64
	.byte	0xa
	.byte	0x0
	.align	1
L.105:
	.byte	0x65
	.byte	0x73
	.byte	0x74
	.byte	0x61
	.byte	0x20
	.byte	0x65
	.byte	0x73
	.byte	0x20
	.byte	0x75
	.byte	0x6e
	.byte	0x61
	.byte	0x20
	.byte	0x70
	.byte	0x72
	.byte	0x75
	.byte	0x65
	.byte	0x62
	.byte	0x61
	.byte	0x0
	.align 4
