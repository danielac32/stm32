	.align 4
	.text
	.align 4
	.data
	.align	4
lower_digits:
	.word	L.5
	.align	4
upper_digits:
	.word	L.6
	.align 4
	.text
	.align	4
strnlen:
	addi x2,x2,-32
	sw  x8,28(x2)
	addi  x8,x2,16
	sw x27,0(x2)
	addi x27,x12,0
	jal x0,L.11
L.8:
L.9:
	li x30,1
	add x27,x27,x30
L.11:
	lb x29,0(x27)
	beq x29,x0,L.12
	addi x29,x13,0
	addi x13,x29,-1
	bne x29,x0,L.8
L.12:
	addi x30,x27,0
	addi x29,x12,0
	sub x30,x30,x29
	addi x10,x30,0
L.7:
	lw x27,0(x2)
	lw  x8,28(x2)
	addi  x2,x2,32
	jalr x0,x1,0

	.align	4
ee_skip_atoi:
	addi x2,x2,-32
	sw  x8,28(x2)
	addi  x8,x2,16
	sw x27,0(x2)
	addi x27,x0,0
	jal x0,L.15
L.14:
	lw x30,0(x12)
	li x29,1
	add x29,x30,x29
	sw x29,0(x12)
	li x29,10
	mul x29,x29,x27
	lb x30,0(x30)
	add x30,x29,x30
	addi x27,x30,-48
L.15:
	lw x30,0(x12)
	lb x30,0(x30)
	li x29,48
	blt x30,x29,L.17
	li x29,57
	ble x30,x29,L.14
L.17:
	addi x10,x27,0
L.13:
	lw x27,0(x2)
	lw  x8,28(x2)
	addi  x2,x2,32
	jalr x0,x1,0

	.align	4
ee_number:
	addi x2,x2,-112
	sw  x8,108(x2)
	addi  x8,x2,32
	sw x24,0(x2)
	sw x25,4(x2)
	sw x26,8(x2)
	sw x27,12(x2)
	la x30,lower_digits
	lw x26,0(x30)
	andi x30,x17,64
	beq x30,x0,L.19
	la x30,upper_digits
	lw x26,0(x30)
L.19:
	andi x30,x17,16
	beq x30,x0,L.21
	andi x17,x17,-2
L.21:
	li x30,2
	blt x14,x30,L.25
	li x30,36
	ble x14,x30,L.23
L.25:
	addi x10,x0,0
	jal x0,L.18
L.23:
	andi x30,x17,1
	beq x30,x0,L.27
	li x24,48
	jal x0,L.28
L.27:
	li x24,32
L.28:
	addi x25,x24,0
	sb x0,-71+80(x8)
	andi x30,x17,2
	beq x30,x0,L.29
	bge x13,x0,L.31
	li x30,45
	sb x30,-71+80(x8)
	sub x13,x0,x13
	addi x15,x15,-1
	jal x0,L.32
L.31:
	andi x30,x17,4
	beq x30,x0,L.33
	li x30,43
	sb x30,-71+80(x8)
	addi x15,x15,-1
	jal x0,L.34
L.33:
	andi x30,x17,8
	beq x30,x0,L.35
	li x30,32
	sb x30,-71+80(x8)
	addi x15,x15,-1
L.35:
L.34:
L.32:
L.29:
	andi x30,x17,32
	beq x30,x0,L.37
	li x30,16
	bne x14,x30,L.39
	addi x15,x15,-2
	jal x0,L.40
L.39:
	li x30,8
	bne x14,x30,L.41
	addi x15,x15,-1
L.41:
L.40:
L.37:
	addi x27,x0,0
	bne x13,x0,L.46
	addi x30,x27,0
	addi x27,x30,1
	addi x29,x8,-70+80
	add x30,x30,x29
	li x29,48
	sb x29,0(x30)
	jal x0,L.44
L.45:
	addi x30,x27,0
	addi x27,x30,1
	addi x29,x8,-70+80
	add x30,x30,x29
	addi x29,x13,0
	addi x28,x14,0
	remu x29,x29,x28
	add x29,x29,x26
	lb x29,0(x29)
	sb x29,0(x30)
	addi x30,x13,0
	addi x29,x14,0
	divu x30,x30,x29
	addi x13,x30,0
L.46:
	bne x13,x0,L.45
L.44:
	ble x27,x16,L.48
	addi x16,x27,0
L.48:
	sub x15,x15,x16
	andi x30,x17,17
	bne x30,x0,L.50
	jal x0,L.53
L.52:
	addi x30,x12,0
	li x29,1
	add x12,x30,x29
	li x29,32
	sb x29,0(x30)
L.53:
	addi x30,x15,0
	addi x15,x30,-1
	bgt x30,x0,L.52
L.50:
	lb x30,-71+80(x8)
	beq x30,x0,L.55
	addi x30,x12,0
	li x29,1
	add x12,x30,x29
	lb x29,-71+80(x8)
	sb x29,0(x30)
L.55:
	andi x30,x17,32
	beq x30,x0,L.57
	li x30,8
	bne x14,x30,L.59
	addi x30,x12,0
	li x29,1
	add x12,x30,x29
	li x29,48
	sb x29,0(x30)
	jal x0,L.60
L.59:
	li x30,16
	bne x14,x30,L.61
	addi x30,x12,0
	li x29,1
	add x12,x30,x29
	li x29,48
	sb x29,0(x30)
	addi x30,x12,0
	li x29,1
	add x12,x30,x29
	la x29,lower_digits
	lw x29,0(x29)
	li x28,33
	add x29,x29,x28
	lb x29,0(x29)
	sb x29,0(x30)
L.61:
L.60:
L.57:
	andi x30,x17,16
	bne x30,x0,L.69
	jal x0,L.66
L.65:
	addi x30,x12,0
	li x29,1
	add x12,x30,x29
	sb x25,0(x30)
L.66:
	addi x30,x15,0
	addi x15,x30,-1
	bgt x30,x0,L.65
	jal x0,L.69
L.68:
	addi x30,x12,0
	li x29,1
	add x12,x30,x29
	li x29,48
	sb x29,0(x30)
L.69:
	addi x30,x16,0
	addi x16,x30,-1
	blt x27,x30,L.68
	jal x0,L.72
L.71:
	addi x30,x12,0
	li x29,1
	add x12,x30,x29
	addi x29,x8,-70+80
	add x29,x27,x29
	lb x29,0(x29)
	sb x29,0(x30)
L.72:
	addi x30,x27,0
	addi x27,x30,-1
	bgt x30,x0,L.71
	jal x0,L.75
L.74:
	addi x30,x12,0
	li x29,1
	add x12,x30,x29
	li x29,32
	sb x29,0(x30)
L.75:
	addi x30,x15,0
	addi x15,x30,-1
	bgt x30,x0,L.74
	addi x10,x12,0
L.18:
	lw x24,0(x2)
	lw x25,4(x2)
	lw x26,8(x2)
	lw x27,12(x2)
	lw  x8,108(x2)
	addi  x2,x2,112
	jalr x0,x1,0

	.align	4
eaddr:
	addi x2,x2,-48
	sw  x8,44(x2)
	addi  x8,x2,16
	sw x25,0(x2)
	sw x26,4(x2)
	sw x27,8(x2)
	la x30,lower_digits
	lw x25,0(x30)
	andi x30,x16,64
	beq x30,x0,L.78
	la x30,upper_digits
	lw x25,0(x30)
L.78:
	addi x26,x0,0
	addi x27,x0,0
L.80:
	beq x27,x0,L.84
	addi x30,x26,0
	addi x26,x30,1
	addi x29,x8,-28+32
	add x30,x30,x29
	li x29,58
	sb x29,0(x30)
L.84:
	addi x30,x26,0
	addi x26,x30,1
	addi x29,x8,-28+32
	add x30,x30,x29
	add x29,x27,x13
	lbu x29,0(x29)
	li x28,4
	sra x29,x29,x28
	add x29,x29,x25
	lb x29,0(x29)
	sb x29,0(x30)
	addi x30,x26,0
	addi x26,x30,1
	addi x29,x8,-28+32
	add x30,x30,x29
	add x29,x27,x13
	lbu x29,0(x29)
	andi x29,x29,15
	add x29,x29,x25
	lb x29,0(x29)
	sb x29,0(x30)
L.81:
	addi x27,x27,1
	li x30,6
	blt x27,x30,L.80
	andi x30,x16,16
	bne x30,x0,L.86
	jal x0,L.89
L.88:
	addi x30,x12,0
	li x29,1
	add x12,x30,x29
	li x29,32
	sb x29,0(x30)
L.89:
	addi x30,x14,0
	addi x14,x30,-1
	blt x26,x30,L.88
L.86:
	addi x27,x0,0
	jal x0,L.94
L.91:
	addi x30,x12,0
	li x29,1
	add x12,x30,x29
	addi x29,x8,-28+32
	add x29,x27,x29
	lb x29,0(x29)
	sb x29,0(x30)
L.92:
	addi x27,x27,1
L.94:
	blt x27,x26,L.91
	jal x0,L.96
L.95:
	addi x30,x12,0
	li x29,1
	add x12,x30,x29
	li x29,32
	sb x29,0(x30)
L.96:
	addi x30,x14,0
	addi x14,x30,-1
	blt x26,x30,L.95
	addi x10,x12,0
L.77:
	lw x25,0(x2)
	lw x26,4(x2)
	lw x27,8(x2)
	lw  x8,44(x2)
	addi  x2,x2,48
	jalr x0,x1,0

	.align	4
iaddr:
	addi x2,x2,-48
	sw  x8,44(x2)
	addi  x8,x2,16
	sw x25,0(x2)
	sw x26,4(x2)
	sw x27,8(x2)
	addi x25,x0,0
	addi x27,x0,0
L.99:
	beq x27,x0,L.103
	addi x30,x25,0
	addi x25,x30,1
	addi x29,x8,-28+32
	add x30,x30,x29
	li x29,46
	sb x29,0(x30)
L.103:
	add x30,x27,x13
	lbu x26,0(x30)
	bne x26,x0,L.105
	addi x30,x25,0
	addi x25,x30,1
	addi x29,x8,-28+32
	add x30,x30,x29
	la x29,lower_digits
	lw x29,0(x29)
	lb x29,0(x29)
	sb x29,0(x30)
	jal x0,L.106
L.105:
	li x30,100
	blt x26,x30,L.107
	addi x30,x25,0
	addi x25,x30,1
	addi x29,x8,-28+32
	add x30,x30,x29
	li x29,100
	div x29,x26,x29
	la x28,lower_digits
	lw x28,0(x28)
	add x29,x29,x28
	lb x29,0(x29)
	sb x29,0(x30)
	li x30,100
	rem x26,x26,x30
	addi x30,x25,0
	addi x25,x30,1
	addi x29,x8,-28+32
	add x30,x30,x29
	li x29,10
	div x29,x26,x29
	la x28,lower_digits
	lw x28,0(x28)
	add x29,x29,x28
	lb x29,0(x29)
	sb x29,0(x30)
	li x30,10
	rem x26,x26,x30
	jal x0,L.108
L.107:
	li x30,10
	blt x26,x30,L.109
	addi x30,x25,0
	addi x25,x30,1
	addi x29,x8,-28+32
	add x30,x30,x29
	li x29,10
	div x29,x26,x29
	la x28,lower_digits
	lw x28,0(x28)
	add x29,x29,x28
	lb x29,0(x29)
	sb x29,0(x30)
	li x30,10
	rem x26,x26,x30
L.109:
L.108:
	addi x30,x25,0
	addi x25,x30,1
	addi x29,x8,-28+32
	add x30,x30,x29
	la x29,lower_digits
	lw x29,0(x29)
	add x29,x26,x29
	lb x29,0(x29)
	sb x29,0(x30)
L.106:
L.100:
	addi x27,x27,1
	li x30,4
	blt x27,x30,L.99
	andi x30,x16,16
	bne x30,x0,L.111
	jal x0,L.114
L.113:
	addi x30,x12,0
	li x29,1
	add x12,x30,x29
	li x29,32
	sb x29,0(x30)
L.114:
	addi x30,x14,0
	addi x14,x30,-1
	blt x25,x30,L.113
L.111:
	addi x27,x0,0
	jal x0,L.119
L.116:
	addi x30,x12,0
	li x29,1
	add x12,x30,x29
	addi x29,x8,-28+32
	add x29,x27,x29
	lb x29,0(x29)
	sb x29,0(x30)
L.117:
	addi x27,x27,1
L.119:
	blt x27,x25,L.116
	jal x0,L.121
L.120:
	addi x30,x12,0
	li x29,1
	add x12,x30,x29
	li x29,32
	sb x29,0(x30)
L.121:
	addi x30,x14,0
	addi x14,x30,-1
	blt x25,x30,L.120
	addi x10,x12,0
L.98:
	lw x25,0(x2)
	lw x26,4(x2)
	lw x27,8(x2)
	lw  x8,44(x2)
	addi  x2,x2,48
	jalr x0,x1,0

	.align	4
ee_bufcpy:
	addi x2,x2,-32
	sw  x8,28(x2)
	addi  x8,x2,16
	sw x27,0(x2)
	add x27,x14,x13
	jal x0,L.125
L.124:
	addi x30,x12,0
	li x29,1
	add x12,x30,x29
	addi x28,x13,0
	add x13,x28,x29
	lb x29,0(x28)
	sb x29,0(x30)
L.125:
	addi x30,x13,0
	addi x29,x27,0
	bne x30,x29,L.124
L.123:
	lw x27,0(x2)
	lw  x8,28(x2)
	addi  x2,x2,32
	jalr x0,x1,0

	.align	4
parse_float:
	addi x2,x2,-176
	sw  x8,172(x2)
	addi  x8,x2,64
	sw x1,24(x2)
	sw x21,28(x2)
	sw x22,32(x2)
	sw x23,36(x2)
	sw x24,40(x2)
	sw x25,44(x2)
	sw x26,48(x2)
	sw x27,52(x2)
	sw x12,112(x8)
	mv x27,x13
	mv x26,x14
	mv x25,x15
	addi x24,x0,0
	sw x0,-16+112(x8)
	slli x30,x26,8*(4-1)
	srai x30,x30,8*(4-1)
	li x29,71
	beq x30,x29,L.130
	li x29,69
	bne x30,x29,L.128
L.130:
	li x30,1
	sw x30,-16+112(x8)
	slli x30,x26,8*(4-1)
	srai x30,x30,8*(4-1)
	addi x30,x30,32
	addi x26,x30,0
L.128:
	slli x30,x26,8*(4-1)
	srai x30,x30,8*(4-1)
	li x29,103
	bne x30,x29,L.131
	lw x12,0+112(x8)
	addi x13,x25,0
	addi x14,x8,-8+112
	addi x15,x8,-12+112
	addi x16,x8,-100+112
	jal x1,ecvtbuf
	addi x24,x10,0
	lw x30,-8+112(x8)
	addi x30,x30,-1
	sw x30,-20+112(x8)
	lw x30,-20+112(x8)
	li x29,-4
	blt x30,x29,L.135
	addi x29,x25,-1
	ble x30,x29,L.133
L.135:
	li x26,101
	addi x25,x25,-1
	jal x0,L.134
L.133:
	li x26,102
	lw x30,-8+112(x8)
	sub x25,x25,x30
L.134:
L.131:
	slli x30,x26,8*(4-1)
	srai x30,x30,8*(4-1)
	li x29,101
	bne x30,x29,L.136
	lw x12,0+112(x8)
	addi x13,x25,1
	addi x14,x8,-8+112
	addi x15,x8,-12+112
	addi x16,x8,-100+112
	jal x1,ecvtbuf
	addi x24,x10,0
	lw x30,-12+112(x8)
	beq x30,x0,L.138
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	li x29,45
	sb x29,0(x30)
L.138:
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	lb x29,0(x24)
	sb x29,0(x30)
	ble x25,x0,L.140
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	li x29,46
	sb x29,0(x30)
L.140:
	addi x12,x27,0
	li x30,1
	add x13,x24,x30
	addi x14,x25,0
	jal x1,ee_bufcpy
	add x27,x25,x27
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	lw x29,-16+112(x8)
	beq x29,x0,L.143
	li x21,69
	jal x0,L.144
L.143:
	li x21,101
L.144:
	addi x29,x21,0
	sb x29,0(x30)
	lw x30,-8+112(x8)
	bne x30,x0,L.145
	lw x12,0+112(x8)
	la x30,L.149
	lw x13,0(x30)
	jal x1,float32_ne
	bne x10,x0,L.147
	addi x23,x0,0
	jal x0,L.146
L.147:
	li x23,-1
	jal x0,L.146
L.145:
	lw x30,-8+112(x8)
	addi x23,x30,-1
L.146:
	bge x23,x0,L.150
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	li x29,45
	sb x29,0(x30)
	sub x23,x0,x23
	jal x0,L.151
L.150:
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	li x29,43
	sb x29,0(x30)
L.151:
	li x30,2
	add x30,x27,x30
	li x29,10
	rem x29,x23,x29
	addi x29,x29,48
	sb x29,0(x30)
	li x30,10
	div x23,x23,x30
	li x29,1
	add x29,x27,x29
	rem x30,x23,x30
	addi x30,x30,48
	sb x30,0(x29)
	li x30,10
	div x23,x23,x30
	rem x30,x23,x30
	addi x30,x30,48
	sb x30,0(x27)
	li x30,3
	add x27,x27,x30
	jal x0,L.137
L.136:
	slli x30,x26,8*(4-1)
	srai x30,x30,8*(4-1)
	li x29,102
	bne x30,x29,L.152
	lw x12,0+112(x8)
	addi x13,x25,0
	addi x14,x8,-8+112
	addi x15,x8,-12+112
	addi x16,x8,-100+112
	jal x1,fcvtbuf
	addi x24,x10,0
	lw x30,-12+112(x8)
	beq x30,x0,L.154
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	li x29,45
	sb x29,0(x30)
L.154:
	lb x30,0(x24)
	beq x30,x0,L.156
	lw x30,-8+112(x8)
	bgt x30,x0,L.158
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	li x29,48
	sb x29,0(x30)
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	li x29,46
	sb x29,0(x30)
	addi x22,x0,0
	jal x0,L.163
L.160:
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	li x29,48
	sb x29,0(x30)
L.161:
	addi x22,x22,1
L.163:
	lw x30,-8+112(x8)
	sub x30,x0,x30
	blt x22,x30,L.160
	jal x0,L.165
L.164:
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	addi x28,x24,0
	add x24,x28,x29
	lb x29,0(x28)
	sb x29,0(x30)
L.165:
	lb x30,0(x24)
	bne x30,x0,L.164
	jal x0,L.157
L.158:
	addi x22,x0,0
	jal x0,L.168
L.167:
	addi x30,x22,0
	addi x22,x30,1
	lw x29,-8+112(x8)
	bne x30,x29,L.170
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	li x29,46
	sb x29,0(x30)
L.170:
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	addi x28,x24,0
	add x24,x28,x29
	lb x29,0(x28)
	sb x29,0(x30)
L.168:
	lb x30,0(x24)
	bne x30,x0,L.167
	jal x0,L.157
L.156:
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	li x29,48
	sb x29,0(x30)
	ble x25,x0,L.172
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	li x29,46
	sb x29,0(x30)
	addi x22,x0,0
	jal x0,L.177
L.174:
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	li x29,48
	sb x29,0(x30)
L.175:
	addi x22,x22,1
L.177:
	blt x22,x25,L.174
L.172:
L.157:
L.152:
L.137:
	sb x0,0(x27)
L.127:
	lw x1,24(x2)
	lw x21,28(x2)
	lw x22,32(x2)
	lw x23,36(x2)
	lw x24,40(x2)
	lw x25,44(x2)
	lw x26,48(x2)
	lw x27,52(x2)
	lw  x8,172(x2)
	addi  x2,x2,176
	jalr x0,x1,0

	.align	4
decimal_point:
	addi x2,x2,-64
	sw  x8,60(x2)
	addi  x8,x2,48
	sw x1,24(x2)
	sw x26,28(x2)
	sw x27,32(x2)
	mv x27,x12
	jal x0,L.180
L.179:
	lb x30,0(x27)
	li x29,46
	bne x30,x29,L.182
	jal x0,L.178
L.182:
	lb x30,0(x27)
	li x29,101
	beq x30,x29,L.186
	li x29,69
	bne x30,x29,L.184
L.186:
	jal x0,L.181
L.184:
	li x30,1
	add x27,x27,x30
L.180:
	lb x30,0(x27)
	bne x30,x0,L.179
L.181:
	lb x30,0(x27)
	beq x30,x0,L.187
	addi x12,x27,0
	li x13,256
	jal x1,strnlen
	addi x26,x10,0
	jal x0,L.190
L.189:
	addi x30,x26,1
	add x30,x30,x27
	add x29,x26,x27
	lb x29,0(x29)
	sb x29,0(x30)
	addi x26,x26,-1
L.190:
	bgt x26,x0,L.189
	li x30,46
	sb x30,0(x27)
	jal x0,L.188
L.187:
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	li x29,46
	sb x29,0(x30)
	sb x0,0(x27)
L.188:
L.178:
	lw x1,24(x2)
	lw x26,28(x2)
	lw x27,32(x2)
	lw  x8,60(x2)
	addi  x2,x2,64
	jalr x0,x1,0

	.align	4
cropzeros:
	addi x2,x2,-32
	sw  x8,28(x2)
	addi  x8,x2,16
	sw x27,0(x2)
	jal x0,L.194
L.193:
	li x30,1
	add x12,x12,x30
L.194:
	lb x30,0(x12)
	beq x30,x0,L.196
	li x29,46
	bne x30,x29,L.193
L.196:
	addi x30,x12,0
	li x29,1
	add x12,x30,x29
	lb x30,0(x30)
	beq x30,x0,L.197
	jal x0,L.200
L.199:
	li x30,1
	add x12,x12,x30
L.200:
	lb x30,0(x12)
	beq x30,x0,L.203
	li x29,101
	beq x30,x29,L.203
	li x29,69
	bne x30,x29,L.199
L.203:
	addi x30,x12,0
	li x29,-1
	add x12,x30,x29
	addi x27,x30,0
	jal x0,L.205
L.204:
	li x30,-1
	add x12,x12,x30
L.205:
	lb x30,0(x12)
	li x29,48
	beq x30,x29,L.204
	lb x30,0(x12)
	li x29,46
	bne x30,x29,L.210
	li x30,-1
	add x12,x12,x30
	jal x0,L.210
L.209:
	li x30,1
	add x30,x12,x30
	addi x12,x30,0
	sb x0,0(x30)
L.210:
	addi x30,x12,0
	addi x29,x27,0
	bne x30,x29,L.209
L.197:
L.192:
	lw x27,0(x2)
	lw  x8,28(x2)
	addi  x2,x2,32
	jalr x0,x1,0

	.align	4
flt:
	addi x2,x2,-160
	sw  x8,156(x2)
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
	sw x13,100(x8)
	mv x26,x14
	mv x25,x15
	sw x16,112(x8)
	mv x24,x17
	lw x30,16+96(x8)
	sb x30,16+96(x8)
	andi x30,x24,16
	beq x30,x0,L.213
	andi x24,x24,-2
L.213:
	andi x30,x24,1
	beq x30,x0,L.216
	li x20,48
	jal x0,L.217
L.216:
	li x20,32
L.217:
	addi x21,x20,0
	sb x0,-85+96(x8)
	andi x30,x24,2
	beq x30,x0,L.218
	lw x12,4+96(x8)
	la x30,L.149
	lw x13,0(x30)
	jal x1,float32_ge
	bne x10,x0,L.220
	li x30,45
	sb x30,-85+96(x8)
	lw x12,4+96(x8)
	jal x1,float32_neg
	sw x10,4+96(x8)
	addi x26,x26,-1
	jal x0,L.221
L.220:
	andi x30,x24,4
	beq x30,x0,L.222
	li x30,43
	sb x30,-85+96(x8)
	addi x26,x26,-1
	jal x0,L.223
L.222:
	andi x30,x24,8
	beq x30,x0,L.224
	li x30,32
	sb x30,-85+96(x8)
	addi x26,x26,-1
L.224:
L.223:
L.221:
L.218:
	bge x25,x0,L.226
	li x25,6
L.226:
	lw x12,4+96(x8)
	addi x13,x8,-84+96
	lb x14,16+96(x8)
	addi x15,x25,0
	jal x1,parse_float
	andi x29,x24,32
	beq x29,x0,L.228
	bne x25,x0,L.228
	addi x12,x8,-84+96
	jal x1,decimal_point
L.228:
	lb x30,16+96(x8)
	li x29,103
	bne x30,x29,L.230
	andi x30,x24,32
	bne x30,x0,L.230
	addi x12,x8,-84+96
	jal x1,cropzeros
L.230:
	addi x12,x8,-84+96
	li x13,256
	jal x1,strnlen
	addi x22,x10,0
	sub x26,x26,x22
	andi x30,x24,17
	bne x30,x0,L.232
	jal x0,L.235
L.234:
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	li x29,32
	sb x29,0(x30)
L.235:
	addi x30,x26,0
	addi x26,x30,-1
	bgt x30,x0,L.234
L.232:
	lb x30,-85+96(x8)
	beq x30,x0,L.237
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	lb x29,-85+96(x8)
	sb x29,0(x30)
L.237:
	andi x30,x24,16
	bne x30,x0,L.239
	jal x0,L.242
L.241:
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	sb x21,0(x30)
L.242:
	addi x30,x26,0
	addi x26,x30,-1
	bgt x30,x0,L.241
L.239:
	addi x23,x0,0
	jal x0,L.247
L.244:
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	addi x29,x8,-84+96
	add x29,x23,x29
	lb x29,0(x29)
	sb x29,0(x30)
L.245:
	addi x23,x23,1
L.247:
	blt x23,x22,L.244
	jal x0,L.249
L.248:
	addi x30,x27,0
	li x29,1
	add x27,x30,x29
	li x29,32
	sb x29,0(x30)
L.249:
	addi x30,x26,0
	addi x26,x30,-1
	bgt x30,x0,L.248
	addi x10,x27,0
L.212:
	lw x1,24(x2)
	lw x20,28(x2)
	lw x21,32(x2)
	lw x22,36(x2)
	lw x23,40(x2)
	lw x24,44(x2)
	lw x25,48(x2)
	lw x26,52(x2)
	lw x27,56(x2)
	lw  x8,156(x2)
	addi  x2,x2,160
	jalr x0,x1,0

	.align	4
ee_vsprintf:
	addi x2,x2,-96
	sw  x8,92(x2)
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
	sw x12,16(x8)
	sw x13,20(x8)
	mv x27,x14
	lw x26,0+16(x8)
	jal x0,L.255
L.252:
	lw x30,4+16(x8)
	lb x30,0(x30)
	li x29,37
	beq x30,x29,L.256
	addi x30,x26,0
	li x29,1
	add x26,x30,x29
	lw x29,4+16(x8)
	lb x29,0(x29)
	sb x29,0(x30)
	jal x0,L.253
L.256:
	addi x24,x0,0
L.258:
	lw x30,4+16(x8)
	li x29,1
	add x30,x30,x29
	sw x30,4+16(x8)
	lw x30,4+16(x8)
	lb x30,0(x30)
	sw x30,-12+16(x8)
	lw x30,-12+16(x8)
	li x29,32
	beq x30,x29,L.264
	li x28,35
	beq x30,x28,L.265
	blt x30,x29,L.259
L.267:
	lw x30,-12+16(x8)
	li x29,43
	beq x30,x29,L.263
	li x29,45
	beq x30,x29,L.262
	li x29,48
	beq x30,x29,L.266
	jal x0,L.259
L.262:
	ori x24,x24,16
	jal x0,L.258
L.263:
	ori x24,x24,4
	jal x0,L.258
L.264:
	ori x24,x24,8
	jal x0,L.258
L.265:
	ori x24,x24,32
	jal x0,L.258
L.266:
	ori x24,x24,1
	jal x0,L.258
L.259:
	li x25,-1
	lw x30,4+16(x8)
	lb x30,0(x30)
	li x29,48
	blt x30,x29,L.268
	li x29,57
	bgt x30,x29,L.268
	addi x12,x8,4+16
	jal x1,ee_skip_atoi
	addi x25,x10,0
	jal x0,L.269
L.268:
	lw x30,4+16(x8)
	lb x30,0(x30)
	li x29,42
	bne x30,x29,L.270
	lw x30,4+16(x8)
	li x29,1
	add x30,x30,x29
	sw x30,4+16(x8)
	li x30,4
	add x30,x27,x30
	addi x27,x30,0
	li x29,-4
	add x30,x30,x29
	lw x25,0(x30)
	bge x25,x0,L.272
	sub x25,x0,x25
	ori x24,x24,16
L.272:
L.270:
L.269:
	li x23,-1
	lw x30,4+16(x8)
	lb x30,0(x30)
	li x29,46
	bne x30,x29,L.274
	lw x30,4+16(x8)
	li x29,1
	add x30,x30,x29
	sw x30,4+16(x8)
	lw x30,4+16(x8)
	lb x30,0(x30)
	li x29,48
	blt x30,x29,L.276
	li x29,57
	bgt x30,x29,L.276
	addi x12,x8,4+16
	jal x1,ee_skip_atoi
	addi x23,x10,0
	jal x0,L.277
L.276:
	lw x30,4+16(x8)
	lb x30,0(x30)
	li x29,42
	bne x30,x29,L.278
	lw x30,4+16(x8)
	li x29,1
	add x30,x30,x29
	sw x30,4+16(x8)
	li x30,4
	add x30,x27,x30
	addi x27,x30,0
	li x29,-4
	add x30,x30,x29
	lw x23,0(x30)
L.278:
L.277:
	bge x23,x0,L.280
	addi x23,x0,0
L.280:
L.274:
	li x21,-1
	lw x30,4+16(x8)
	lb x30,0(x30)
	li x29,108
	beq x30,x29,L.284
	li x29,76
	bne x30,x29,L.282
L.284:
	lw x30,4+16(x8)
	lb x21,0(x30)
	li x29,1
	add x30,x30,x29
	sw x30,4+16(x8)
L.282:
	li x20,10
	lw x30,4+16(x8)
	lb x30,0(x30)
	sw x30,-16+16(x8)
	lw x30,-16+16(x8)
	li x29,88
	beq x30,x29,L.321
	bgt x30,x29,L.331
L.330:
	lw x30,-16+16(x8)
	li x29,65
	beq x30,x29,L.316
	jal x0,L.285
L.331:
	lw x30,-16+16(x8)
	li x29,97
	blt x30,x29,L.285
	li x29,105
	bgt x30,x29,L.332
	li x29,2
	sll x30,x30,x29
	la x29,L.333-388
	add x30,x30,x29
	lw x30,0(x30)
	jalr x0,x30,0
	.align 4
	.data
	.align	4
L.333:
	.word	L.317
	.word	L.285
	.word	L.288
	.word	L.323
	.word	L.285
	.word	L.325
	.word	L.285
	.word	L.285
	.word	L.323
	.align 4
	.text
L.332:
	lw x30,-16+16(x8)
	li x29,111
	blt x30,x29,L.285
	li x29,120
	bgt x30,x29,L.285
	li x29,2
	sll x30,x30,x29
	la x29,L.335-444
	add x30,x30,x29
	lw x30,0(x30)
	jalr x0,x30,0
	.align 4
	.data
	.align	4
L.335:
	.word	L.320
	.word	L.313
	.word	L.285
	.word	L.285
	.word	L.297
	.word	L.285
	.word	L.286
	.word	L.285
	.word	L.285
	.word	L.322
	.align 4
	.text
L.288:
	andi x30,x24,16
	bne x30,x0,L.289
	jal x0,L.292
L.291:
	addi x30,x26,0
	li x29,1
	add x26,x30,x29
	li x29,32
	sb x29,0(x30)
L.292:
	addi x30,x25,-1
	addi x25,x30,0
	bgt x30,x0,L.291
L.289:
	addi x30,x26,0
	li x29,1
	add x26,x30,x29
	li x29,4
	add x29,x27,x29
	addi x27,x29,0
	li x28,-4
	add x29,x29,x28
	lw x29,0(x29)
	slli x29,x29,8*(4-1)
	srli x29,x29,8*(4-1)
	sb x29,0(x30)
	jal x0,L.295
L.294:
	addi x30,x26,0
	li x29,1
	add x26,x30,x29
	li x29,32
	sb x29,0(x30)
L.295:
	addi x30,x25,-1
	addi x25,x30,0
	bgt x30,x0,L.294
	jal x0,L.253
L.297:
	li x30,4
	add x30,x27,x30
	addi x27,x30,0
	li x29,-4
	add x30,x30,x29
	lw x30,0(x30)
	sw x30,-8+16(x8)
	lw x30,-8+16(x8)
	bne x30,x0,L.298
	la x30,L.300
	sw x30,-8+16(x8)
L.298:
	lw x12,-8+16(x8)
	addi x13,x23,0
	jal x1,strnlen
	addi x22,x10,0
	andi x30,x24,16
	bne x30,x0,L.301
	jal x0,L.304
L.303:
	addi x30,x26,0
	li x29,1
	add x26,x30,x29
	li x29,32
	sb x29,0(x30)
L.304:
	addi x30,x25,0
	addi x25,x30,-1
	blt x22,x30,L.303
L.301:
	addi x19,x0,0
	jal x0,L.309
L.306:
	addi x30,x26,0
	li x29,1
	add x26,x30,x29
	lw x28,-8+16(x8)
	add x29,x28,x29
	sw x29,-8+16(x8)
	lb x29,0(x28)
	sb x29,0(x30)
L.307:
	addi x19,x19,1
L.309:
	blt x19,x22,L.306
	jal x0,L.311
L.310:
	addi x30,x26,0
	li x29,1
	add x26,x30,x29
	li x29,32
	sb x29,0(x30)
L.311:
	addi x30,x25,0
	addi x25,x30,-1
	blt x22,x30,L.310
	jal x0,L.253
L.313:
	li x30,-1
	bne x25,x30,L.314
	li x25,8
	ori x24,x24,1
L.314:
	addi x12,x26,0
	li x30,4
	add x30,x27,x30
	addi x27,x30,0
	li x29,-4
	add x30,x30,x29
	lw x30,0(x30)
	addi x13,x30,0
	li x14,16
	addi x15,x25,0
	addi x16,x23,0
	addi x17,x24,0
	jal x1,ee_number
	addi x26,x10,0
	jal x0,L.253
L.316:
	ori x24,x24,64
L.317:
	li x30,108
	bne x21,x30,L.318
	addi x12,x26,0
	li x30,4
	add x30,x27,x30
	addi x27,x30,0
	li x29,-4
	add x30,x30,x29
	lw x13,0(x30)
	addi x14,x25,0
	addi x15,x23,0
	addi x16,x24,0
	jal x1,eaddr
	addi x26,x10,0
	jal x0,L.253
L.318:
	addi x12,x26,0
	li x30,4
	add x30,x27,x30
	addi x27,x30,0
	li x29,-4
	add x30,x30,x29
	lw x13,0(x30)
	addi x14,x25,0
	addi x15,x23,0
	addi x16,x24,0
	jal x1,iaddr
	addi x26,x10,0
	jal x0,L.253
L.320:
	li x20,8
	jal x0,L.286
L.321:
	ori x24,x24,64
L.322:
	li x20,16
	jal x0,L.286
L.323:
	ori x24,x24,2
	jal x0,L.286
L.325:
	addi x12,x26,0
	li x30,4
	add x30,x27,x30
	addi x27,x30,0
	li x29,-4
	add x30,x30,x29
	lw x13,0(x30)
	addi x14,x25,0
	addi x15,x23,0
	lw x30,4+16(x8)
	lb x16,0(x30)
	ori x17,x24,2
	jal x1,flt
	addi x26,x10,0
	jal x0,L.253
L.285:
	lw x30,4+16(x8)
	lb x30,0(x30)
	li x29,37
	beq x30,x29,L.326
	addi x30,x26,0
	li x29,1
	add x26,x30,x29
	li x29,37
	sb x29,0(x30)
L.326:
	lw x30,4+16(x8)
	lb x30,0(x30)
	beq x30,x0,L.328
	addi x30,x26,0
	li x29,1
	add x26,x30,x29
	lw x29,4+16(x8)
	lb x29,0(x29)
	sb x29,0(x30)
	jal x0,L.253
L.328:
	lw x30,4+16(x8)
	li x29,-1
	add x30,x30,x29
	sw x30,4+16(x8)
	jal x0,L.253
L.286:
	li x30,108
	bne x21,x30,L.337
	li x30,4
	add x30,x27,x30
	addi x27,x30,0
	li x29,-4
	add x30,x30,x29
	lw x18,0(x30)
	jal x0,L.338
L.337:
	andi x30,x24,2
	beq x30,x0,L.339
	li x30,4
	add x30,x27,x30
	addi x27,x30,0
	li x29,-4
	add x30,x30,x29
	lw x30,0(x30)
	addi x18,x30,0
	jal x0,L.340
L.339:
	li x30,4
	add x30,x27,x30
	addi x27,x30,0
	li x29,-4
	add x30,x30,x29
	lw x18,0(x30)
L.340:
L.338:
	addi x12,x26,0
	addi x13,x18,0
	addi x14,x20,0
	addi x15,x25,0
	addi x16,x23,0
	addi x17,x24,0
	jal x1,ee_number
	addi x26,x10,0
L.253:
	lw x30,4+16(x8)
	li x29,1
	add x30,x30,x29
	sw x30,4+16(x8)
L.255:
	lw x30,4+16(x8)
	lb x30,0(x30)
	bne x30,x0,L.252
	sb x0,0(x26)
	addi x30,x26,0
	lw x29,0+16(x8)
	sub x30,x30,x29
	addi x10,x30,0
L.251:
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
	lw  x8,92(x2)
	addi  x2,x2,96
	jalr x0,x1,0

	.globl uart_send_char
	.align	4
uart_send_char:
	addi x2,x2,-32
	sw  x8,28(x2)
	addi  x8,x2,16
	li x30,0x20000000
	sw x30,-8+16(x8)
	lw x30,-8+16(x8)
	slli x29,x12,8*(4-1)
	srai x29,x29,8*(4-1)
	sw x29,0(x30)
L.341:
	lw  x8,28(x2)
	addi  x2,x2,32
	jalr x0,x1,0

	.globl ee_printf
	.align	4
ee_printf:
	addi x2,x2,-1264
	sw  x8,1260(x2)
	addi  x8,x2,48
	sw x1,24(x2)
	sw x26,28(x2)
	sw x27,32(x2)
	sw x12,1216(x8)
	sw x13,1220(x8)
	sw x14,1224(x8)
	sw x15,1228(x8)
	sw x16,1232(x8)
	sw x17,1236(x8)
	addi x26,x0,0
	addi x30,x8,4+1216
	sw x30,-1208+1216(x8)
	addi x12,x8,-1204+1216
	lw x13,0+1216(x8)
	lw x14,-1208+1216(x8)
	jal x1,ee_vsprintf
	addi x27,x8,-1204+1216
	jal x0,L.346
L.345:
	lb x12,0(x27)
	jal x1,uart_send_char
	li x30,1
	add x26,x26,x30
	add x27,x27,x30
L.346:
	lb x30,0(x27)
	bne x30,x0,L.345
	addi x10,x26,0
L.342:
	lw x1,24(x2)
	lw x26,28(x2)
	lw x27,32(x2)
	lw  x8,1260(x2)
	addi  x2,x2,1264
	jalr x0,x1,0

	.align 4
	.data
	.align	1
L.300:
	.byte	0x3c
	.byte	0x4e
	.byte	0x55
	.byte	0x4c
	.byte	0x4c
	.byte	0x3e
	.byte	0x0
	.align	4
L.149:
	.word	0x0
	.align	1
L.6:
	.byte	0x30
	.byte	0x31
	.byte	0x32
	.byte	0x33
	.byte	0x34
	.byte	0x35
	.byte	0x36
	.byte	0x37
	.byte	0x38
	.byte	0x39
	.byte	0x41
	.byte	0x42
	.byte	0x43
	.byte	0x44
	.byte	0x45
	.byte	0x46
	.byte	0x47
	.byte	0x48
	.byte	0x49
	.byte	0x4a
	.byte	0x4b
	.byte	0x4c
	.byte	0x4d
	.byte	0x4e
	.byte	0x4f
	.byte	0x50
	.byte	0x51
	.byte	0x52
	.byte	0x53
	.byte	0x54
	.byte	0x55
	.byte	0x56
	.byte	0x57
	.byte	0x58
	.byte	0x59
	.byte	0x5a
	.byte	0x0
	.align	1
L.5:
	.byte	0x30
	.byte	0x31
	.byte	0x32
	.byte	0x33
	.byte	0x34
	.byte	0x35
	.byte	0x36
	.byte	0x37
	.byte	0x38
	.byte	0x39
	.byte	0x61
	.byte	0x62
	.byte	0x63
	.byte	0x64
	.byte	0x65
	.byte	0x66
	.byte	0x67
	.byte	0x68
	.byte	0x69
	.byte	0x6a
	.byte	0x6b
	.byte	0x6c
	.byte	0x6d
	.byte	0x6e
	.byte	0x6f
	.byte	0x70
	.byte	0x71
	.byte	0x72
	.byte	0x73
	.byte	0x74
	.byte	0x75
	.byte	0x76
	.byte	0x77
	.byte	0x78
	.byte	0x79
	.byte	0x7a
	.byte	0x0
	.align 4
