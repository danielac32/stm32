
# Setup the toolchain.
ARMGNU	= arm-none-eabi
AS	= $(ARMGNU)-as
CC	= $(ARMGNU)-gcc
LD	= $(ARMGNU)-ld
STRIP=$(ARMGNU)-strip
OBJCOPY	= $(ARMGNU)-objcopy
OBJDUMP	= $(ARMGNU)-objdump


CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# Compiler flags
CFLAGS  = -mcpu=cortex-m4 -mno-unaligned-access -mthumb -fno-builtin -fno-stack-protector -nostdlib -c -Wall -O  ${INCLUDE} $(MCU) 
ASFLAGS =  -O  ${INCLUDE} $(MCU) 



SYSTEM_DIR = system
#USB_DIR = usb
FAT32_DIR = fat32
LIB_DIR = lib
TTY_DIR = device/tty
NAM_DIR = device/nam
SPI_DIR = device/spi
SHELL_DIR = shell
INCLUDE		=	-I include

# Create the source lists.
SYSTEM_GCC = $(wildcard $(SYSTEM_DIR)/*.c)
SYSTEM_ASM = $(wildcard $(SYSTEM_DIR)/*.S)
#USB_GCC = $(wildcard $(USB_DIR)/*.c)
LIB_GCC = $(wildcard $(LIB_DIR)/*.c)
FAT32_LIB_GCC = $(wildcard $(FAT32_DIR)/*.c)
#TTY_GCC = $(wildcard $(TTY_DIR)/*.c)
#NAM_GCC = $(wildcard $(NAM_DIR)/*.c)
#SPI_GCC = $(wildcard $(SPI_DIR)/*.c)
#SHELL_GCC = $(wildcard $(SHELL_DIR)/*.c)
 
# Build the object list.
OBJ = $(SYSTEM_GCC:.c=.o)
OBJ += $(SYSTEM_ASM:.S=.o)
#OBJ += $(USB_GCC:.c=.o)
OBJ += $(LIB_GCC:.c=.o)
OBJ += $(FAT32_LIB_GCC:.c=.o)
#OBJ += $(TTY_GCC:.c=.o)
#OBJ += $(NAM_GCC:.c=.o)
#OBJ += $(SPI_GCC:.c=.o)
#OBJ += $(SHELL_GCC:.c=.o)




all: xinu.elf
	arm-none-eabi-objdump -d xinu.elf > xinu.list
	arm-none-eabi-size xinu.elf
	arm-none-eabi-objcopy xinu.elf -O binary xinu.bin

xinu.elf : ld.script $(OBJ)
	$(LD) $(OBJ) -T ld.script -o xinu.elf



TO_ADDR =  0x08010000#0x800c000

dump:
	clear
	rm apps/*.o
	arm-none-eabi-gcc -O0 -c -o apps/entry.o  -mcpu=cortex-m4 -mthumb  apps/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o apps/main.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/user/dump/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/ctype.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/ctype_.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/fdoprnt.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/fdoprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/lib.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/lib.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/printf.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/printf.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/syscall.o  -mcpu=cortex-m4 -mthumb  apps/lib/syscall.s -fpie
	arm-none-eabi-gcc -O0 -c -o apps/syscallc.o  -mcpu=cortex-m4 -mthumb -I apps/include apps/lib/syscallc.c -fpie
	arm-none-eabi-ld apps/entry.o apps/main.o apps/ctype.o apps/fdoprnt.o apps/lib.o apps/printf.o apps/syscall.o apps/syscallc.o  -I apps/include  -nostartfiles -nostdlib -T apps/app_base.ld  -o apps/user/dump/dump0.elf 
	$(STRIP) -s -o apps/user/dump/dump.elf apps/user/dump/dump0.elf


reboot:
	clear
	arm-none-eabi-gcc -O0 -c -o apps/entry.o  -mcpu=cortex-m4 -mthumb  apps/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o apps/main.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/user/reboot/main.c -fpie
	arm-none-eabi-ld apps/entry.o apps/main.o   -I apps/include  -nostartfiles -nostdlib -T apps/app_base.ld  -o apps/user/reboot/reboot0.elf 
	$(STRIP) -s -o apps/user/reboot/reboot.elf apps/user/reboot/reboot0.elf


kill:
	clear
	rm apps/*.o
	arm-none-eabi-gcc -O0 -c -o apps/entry.o  -mcpu=cortex-m4 -mthumb  apps/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o apps/main.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/user/kill/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/ctype.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/ctype_.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/fdoprnt.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/fdoprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/lib.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/lib.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/printf.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/printf.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/syscall.o  -mcpu=cortex-m4 -mthumb  apps/lib/syscall.s -fpie
	arm-none-eabi-gcc -O0 -c -o apps/syscallc.o  -mcpu=cortex-m4 -mthumb -I apps/include apps/lib/syscallc.c -fpie
	arm-none-eabi-ld apps/entry.o apps/main.o apps/ctype.o apps/fdoprnt.o apps/lib.o apps/printf.o apps/syscall.o apps/syscallc.o  -I apps/include  -nostartfiles -nostdlib -T apps/app_base.ld  -o apps/user/kill/kill0.elf 
	$(STRIP) -s -o apps/user/kill/kill.elf apps/user/kill/kill0.elf


memstat:
	clear
	arm-none-eabi-gcc -O0 -c -o apps/entry.o  -mcpu=cortex-m4 -mthumb  apps/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o apps/main.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/user/memstat/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/ctype.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/ctype_.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/fdoprnt.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/fdoprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/lib.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/lib.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/printf.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/printf.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/syscall.o  -mcpu=cortex-m4 -mthumb  apps/lib/syscall.s -fpie
	arm-none-eabi-gcc -O0 -c -o apps/syscallc.o  -mcpu=cortex-m4 -mthumb -I apps/include apps/lib/syscallc.c -fpie
	arm-none-eabi-ld apps/entry.o apps/main.o apps/ctype.o apps/fdoprnt.o apps/lib.o apps/printf.o apps/syscall.o apps/syscallc.o  -I apps/include  -nostartfiles -nostdlib -T apps/app_base.ld  -o apps/user/memstat/memstat0.elf 
	$(STRIP) -s -o apps/user/memstat/memstat.elf apps/user/memstat/memstat0.elf
ps:
	clear
	#@rm apps/*.o
	arm-none-eabi-gcc -O0 -c -o apps/entry.o  -mcpu=cortex-m4 -mthumb  apps/lib/entry.s -fpic
	arm-none-eabi-gcc -O0 -c -o apps/main.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/user/ps/main.c -fpic
	arm-none-eabi-gcc -O0 -c -o apps/ctype.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/ctype_.c -fpic
	arm-none-eabi-gcc -O0 -c -o apps/fdoprnt.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/fdoprnt.c -fpic
	arm-none-eabi-gcc -O0 -c -o apps/lib.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/lib.c -fpic
	arm-none-eabi-gcc -O0 -c -o apps/printf.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/printf.c -fpic
	arm-none-eabi-gcc -O0 -c -o apps/syscall.o  -mcpu=cortex-m4 -mthumb  apps/lib/syscall.s -fpic
	arm-none-eabi-gcc -O0 -c -o apps/syscallc.o  -mcpu=cortex-m4 -mthumb -I apps/include apps/lib/syscallc.c -fpic
	arm-none-eabi-ld apps/entry.o apps/main.o apps/ctype.o apps/fdoprnt.o apps/lib.o apps/printf.o apps/syscall.o apps/syscallc.o  -I apps/include  -nostartfiles -nostdlib -T apps/app_base.ld  -o apps/user/ps/ps0.elf 
	$(STRIP) -s -o apps/user/ps/ps.elf apps/user/ps/ps0.elf

shell:
	clear
	#rm apps/*.o
	arm-none-eabi-gcc -O0 -c -o apps/entry.o  -mcpu=cortex-m4 -mthumb  apps/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o apps/main.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/user/shell/main.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/elf.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/user/shell/elf.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/lexan.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/user/shell/lexan.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/ctype.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/ctype_.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/fdoprnt.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/fdoprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/lib.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/lib.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/printf.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/printf.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/syscall.o  -mcpu=cortex-m4 -mthumb  apps/lib/syscall.s -fpie
	arm-none-eabi-gcc -O0 -c -o apps/syscallc.o  -mcpu=cortex-m4 -mthumb -I apps/include apps/lib/syscallc.c -fpie
	arm-none-eabi-ld apps/entry.o apps/main.o apps/elf.o apps/lexan.o apps/ctype.o apps/fdoprnt.o apps/lib.o apps/printf.o apps/syscall.o apps/syscallc.o  -I apps/include  -nostartfiles -nostdlib -T apps/app_base.ld  -o apps/user/shell/shell0.elf 
	$(STRIP) -s -o apps/user/shell/shell.elf apps/user/shell/shell0.elf



	#st-flash write apps/user/shell.elf $(TO_ADDR)
	#st-flash reset


test1:
	arm-none-eabi-gcc -O0 -c -o apps/entry.o  -mcpu=cortex-m4 -mthumb  apps/lib/entry.s -fpic
	arm-none-eabi-gcc -O0 -c -o apps/test1.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/user/test1.c -fpic
	arm-none-eabi-gcc -O0 -c -o apps/ctype.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/ctype_.c -fpic
	arm-none-eabi-gcc -O0 -c -o apps/fdoprnt.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/fdoprnt.c -fpic
	arm-none-eabi-gcc -O0 -c -o apps/lib.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/lib.c -fpic
	arm-none-eabi-gcc -O0 -c -o apps/printf.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/printf.c -fpic
	arm-none-eabi-gcc -O0 -c -o apps/syscall.o  -mcpu=cortex-m4 -mthumb  apps/lib/syscall.s -fpic
	arm-none-eabi-gcc -O0 -c -o apps/syscallc.o  -mcpu=cortex-m4 -mthumb -I apps/include apps/lib/syscallc.c -fpic
	arm-none-eabi-ld apps/entry.o apps/test1.o apps/ctype.o apps/fdoprnt.o apps/lib.o apps/printf.o apps/syscall.o apps/syscallc.o  -I apps/include  -nostartfiles -nostdlib -T apps/app_base.ld  -o apps/user/blink.elf 
	$(STRIP) -s -o apps/user/test1.elf apps/user/blink.elf

test2:
	arm-none-eabi-gcc -O0 -c -o apps/entry.o  -mcpu=cortex-m4 -mthumb  apps/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o apps/test2.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/user/test2.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/ctype.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/ctype_.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/fdoprnt.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/fdoprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/lib.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/lib.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/printf.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/printf.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/syscall.o  -mcpu=cortex-m4 -mthumb  apps/lib/syscall.s -fpie
	arm-none-eabi-gcc -O0 -c -o apps/syscallc.o  -mcpu=cortex-m4 -mthumb -I apps/include apps/lib/syscallc.c -fpie
	arm-none-eabi-ld  apps/entry.o apps/test2.o apps/ctype.o apps/fdoprnt.o apps/lib.o apps/printf.o apps/syscall.o apps/syscallc.o -I apps/include  -nostartfiles -nostdlib -T apps/app_base.ld  -o apps/user/getpin.elf 
	$(STRIP) -s -o apps/user/getpin2.elf apps/user/getpin.elf

test3:
	arm-none-eabi-gcc -O0 -c -o apps/test3.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/user/test3.c -fpic
	arm-none-eabi-gcc -O0 -c -o apps/ctype.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/ctype_.c -fpic
	arm-none-eabi-gcc -O0 -c -o apps/fdoprnt.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/fdoprnt.c -fpic
	arm-none-eabi-gcc -O0 -c -o apps/lib.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/lib.c -fpic
	arm-none-eabi-gcc -O0 -c -o apps/printf.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/printf.c -fpic
	arm-none-eabi-gcc -O0 -c -o apps/syscall.o  -mcpu=cortex-m4 -mthumb  apps/lib/syscall.s -fpic
	arm-none-eabi-gcc -O0 -c -o apps/syscallc.o  -mcpu=cortex-m4 -mthumb -I apps/include apps/lib/syscallc.c -fpic
	arm-none-eabi-ld apps/*.o -I apps/include  -nostartfiles -nostdlib -T apps/app_base.ld  -o apps/user/test3.elf 
	$(STRIP) -s -o apps/user/test3-strip.elf apps/user/test3.elf

test4:
	arm-none-eabi-gcc -O0 -c -o apps/entry.o  -mcpu=cortex-m4 -mthumb  apps/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o apps/test4.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/user/test4.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/ctype.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/ctype_.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/fdoprnt.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/fdoprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/lib.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/lib.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/printf.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/printf.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/syscall.o  -mcpu=cortex-m4 -mthumb  apps/lib/syscall.s -fpie
	arm-none-eabi-gcc -O0 -c -o apps/syscallc.o  -mcpu=cortex-m4 -mthumb -I apps/include apps/lib/syscallc.c -fpie
	arm-none-eabi-ld apps/test4.o apps/blink.o apps/ctype.o apps/fdoprnt.o apps/lib.o apps/printf.o apps/syscall.o apps/syscallc.o -I apps/include  -nostartfiles -nostdlib -T apps/app_base.ld  -o apps/user/test4.elf 
	$(STRIP) -s -o apps/user/test4-strip.elf apps/user/test4.elf
	arm-none-eabi-objdump -D apps/user/test4.elf


TO_ADDR =  0x08010000#0x800c000


blink:
	clear
	arm-none-eabi-gcc -O0 -c -o apps/entry.o  -mcpu=cortex-m4 -mthumb  apps/lib/entry.s -fpie
	arm-none-eabi-gcc -O0 -c -o apps/blink.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/user/blink.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/ctype.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/ctype_.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/fdoprnt.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/fdoprnt.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/lib.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/lib.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/printf.o  -mcpu=cortex-m4 -mthumb -I apps/include  apps/lib/printf.c -fpie
	arm-none-eabi-gcc -O0 -c -o apps/syscall.o  -mcpu=cortex-m4 -mthumb  apps/lib/syscall.s -fpie
	arm-none-eabi-gcc -O0 -c -o apps/syscallc.o  -mcpu=cortex-m4 -mthumb -I apps/include apps/lib/syscallc.c -fpie
	arm-none-eabi-ld apps/entry.o apps/blink.o apps/ctype.o apps/fdoprnt.o apps/lib.o apps/printf.o apps/syscall.o apps/syscallc.o -I apps/include -nostartfiles -nostdlib -T apps/app_base.ld  -o apps/user/blink.elf 
	$(STRIP) -s -o apps/user/blink1.elf apps/user/blink.elf
	arm-none-eabi-objdump -D apps/user/blink.elf
	#gcc -m32 -o apps/got2 apps/got2.c 
	#./apps/got2 apps/user/blink1.elf
	#gcc  -o apps/dump apps/dump.c
	#./apps/dump out.bin
	#st-flash write apps/user/blink-strip.elf $(TO_ADDR)
	#st-flash write out.bin $(TO_ADDR)

clean:
	@rm -f $(OBJ)
	@rm -f *.elf
	@rm -f *.bin
	@rm -f *.list
	@rm apps/*.o
	@rm apps/user/*.elf

flash:
	st-flash write xinu.bin 0x08000000
	#st-flash reset

reset:
	st-flash reset