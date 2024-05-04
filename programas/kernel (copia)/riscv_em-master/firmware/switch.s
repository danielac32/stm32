.global save_context
.global restore_context
.global set
.global save_context_task
.global restore_context_task
.global __strlen


#************************************************************************
__strlen:	 
mv a1,zero
mv a2,a0
init:
lb	a1, 0(a0)
addi a0,a0,1
bne a1,zero,init
sub a0,a0,a2
ret

#************************************************************************

set:
add sp,sp,a1
addi sp,sp,100
ret  
 
#************************************************************************
save_context_task:
sw    s11,68(a0)
sw    s10,64(a0)
sw    s9,60(a0)
sw    s8,56(a0)
sw    s7,52(a0)
sw    s6,48(a0)
sw    s5,44(a0)
sw    s4,40(a0)
sw    s3,36(a0)
sw    s2,32(a0)
sw    a7,28(a0)
sw    a6,24(a0)
sw    a5,20(a0)
sw    a4,16(a0)
sw    a3,12(a0)
sw    a2,8(a0)
sw    s1,4(a0)
sw    s0,0(a0)
li a0,0
ret


restore_context_task:
lw    s11,68(a0)
lw    s10,64(a0)
lw    s9,60(a0)
lw    s8,56(a0)
lw    s7,52(a0)
lw    s6,48(a0)
lw    s5,44(a0)
lw    s4,40(a0)
lw    s3,36(a0)
lw    s2,32(a0)
lw    a7,28(a0)
lw    a6,24(a0)
lw    a5,20(a0)
lw    a4,16(a0)
lw    a3,12(a0)
lw    a2,8(a0)
lw    s1,4(a0)
lw    s0,0(a0)
li a0,0
ret



#************************************************************************
save_context:
sub  sp,sp,a0
#sw    a0,72(sp)
sw    s11,68(sp)
sw    s10,64(sp)
sw    s9,60(sp)
sw    s8,56(sp)
sw    s7,52(sp)
sw    s6,48(sp)
sw    s5,44(sp)
sw    s4,40(sp)
sw    s3,36(sp)
sw    s2,32(sp)
sw    a7,28(sp)
sw    a6,24(sp)
sw    a5,20(sp)
sw    a4,16(sp)
sw    a3,12(sp)
sw    a2,8(sp)
sw    s1,4(sp)
sw    s0,0(sp)

ret

restore_context:
#lw    a0,72(sp)
lw    s11,68(sp)
lw    s10,64(sp)
lw    s9,60(sp)
lw    s8,56(sp)
lw    s7,52(sp)
lw    s6,48(sp)
lw    s5,44(sp)
lw    s4,40(sp)
lw    s3,36(sp)
lw    s2,32(sp)
lw    a7,28(sp)
lw    a6,24(sp)
lw    a5,20(sp)
lw    a4,16(sp)
lw    a3,12(sp)
lw    a2,8(sp)
lw    s1,4(sp)
lw    s0,0(sp)
add  sp,sp,a0
ret

#***********************************************************************




