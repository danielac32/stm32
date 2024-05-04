|       signed/unsigned multiply
|       calling (left * right)
|       push left
|       ldd right
|       jsr [u|s]mul (same entry point)
|       result in d, stack is popped
.globl __smul,__umul
__smul:
__umul:
	pshs    d
        lda     2+2,s
        mul             ; left msb * right lsb
        pshs    b       ; save high order
        ldb     -1+3,s ; right lsb
        lda     3+3,s  ; left lsb
        mul
        pshs    d
        lda     3+5,s  ; left lsb
        ldb     -2+5,s ; right msb
        beq     small   ; is zero?
        mul             ; no, gotta do it too
        tfr     b,a
        clrb
        addd    ,s++   ; partial prod
        bra     big
small:  puls    d       ; aha! don't need third mul
big:    adda    ,s+
        pshs    d
        ldd     4,s    ; rearrange return address
        std     6,s
        puls    d
        leas    4,s
        rts
