export CROSS_COMPILE=m68k-uclinux-
export CROSS_LD=$(CROSS_COMPILE)ld
export CROSS_CC=$(CROSS_COMPILE)gcc
# Do not use the Fedora gcc 5.3.1. It miscompiles stuff badly.
export CROSS_CCOPTS=-c -Os -fno-strict-aliasing -fomit-frame-pointer -fno-stack-protector -fno-PIC -fno-builtin -Wall -m68000 -I$(ROOT_DIR)/cpu-68000 -I$(ROOT_DIR)/platform-$(TARGET) -I$(ROOT_DIR)/include
export CROSS_AS=$(CROSS_CC) $(CROSS_CCOPTS) #-Wa,-M
export CROSS_CC_SEG1=
export CROSS_CC_SEG2=
export CROSS_CC_SEG3=
# Fixme: we should split discard off
export CROSS_CC_SEGDISC=
export CROSS_CC_VIDEO=
export ASOPTS=
export ASMEXT = .S
export BINEXT = .o
export BITS=32