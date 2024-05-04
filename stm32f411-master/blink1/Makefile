# Makefile for blink demo
#
# This is/was the first code I ever wrote to
# run on an STM32F411.

TOOLS = arm-none-eabi

# Assembling with gcc makes it want crt0 at link time.
#AS = $(TOOLS)-gcc
AS = $(TOOLS)-as
# Use the -g flag if you intend to use gdb
#CC = $(TOOLS)-gcc -mcpu=cortex-m3 -mthumb
#CC = $(TOOLS)-gcc -mcpu=cortex-m3 -mthumb -g
CC = $(TOOLS)-gcc -mcpu=cortex-m4 -mthumb -Os

# Optimizing for size works fine and reduces code size from 360 to 252 bytes.

#LD = $(TOOLS)-gcc
LD = $(TOOLS)-ld.bfd
OBJCOPY = $(TOOLS)-objcopy
DUMP = $(TOOLS)-objdump -d
GDB = $(TOOLS)-gdb

#OBJS = locore.o blink.o
OBJS = blink.o locore.o

all: blink.elf blink.dump

blink.dump:	blink.elf
	$(DUMP) blink.elf >blink.dump

blink.elf: 	$(OBJS)
	$(LD) -T blink.lds -o blink.elf $(OBJS)
	$(OBJCOPY) -O binary blink.elf blink.bin

	
locore.o:	locore.s
	$(AS) locore.s -o locore.o

blink.o:	blink.c
	$(CC) -c blink.c



clean:
	rm -f *.o blink.elf blink.dump

flash:
	st-flash write blink.bin 0x08000000
	st-flash reset