
# Setup the toolchain.
ARMGNU	= arm-none-eabi
AS	= $(ARMGNU)-as
CC	= $(ARMGNU)-gcc
LD	= $(ARMGNU)-ld
OBJCOPY	= $(ARMGNU)-objcopy
OBJDUMP	= $(ARMGNU)-objdump
STRIP=$(ARMGNU)-strip

CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# Compiler flags
CFLAGS  = -mcpu=cortex-m4 -mno-unaligned-access -mthumb -fno-builtin -fno-stack-protector -nostdlib -c -Wall -O  ${INCLUDE} $(MCU) 
ASFLAGS =  -O  ${INCLUDE} $(MCU) 



SYSTEM_DIR = system
#USB_DIR = usb
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
TTY_GCC = $(wildcard $(TTY_DIR)/*.c)
NAM_GCC = $(wildcard $(NAM_DIR)/*.c)
SPI_GCC = $(wildcard $(SPI_DIR)/*.c)
SHELL_GCC = $(wildcard $(SHELL_DIR)/*.c)
 
# Build the object list.
OBJ = $(SYSTEM_GCC:.c=.o)
OBJ += $(SYSTEM_ASM:.S=.o)
#OBJ += $(USB_GCC:.c=.o)
OBJ += $(LIB_GCC:.c=.o)
OBJ += $(TTY_GCC:.c=.o)
OBJ += $(NAM_GCC:.c=.o)
OBJ += $(SPI_GCC:.c=.o)
OBJ += $(SHELL_GCC:.c=.o)


all: xinu.elf
	arm-none-eabi-objdump -d xinu.elf > xinu.list
	arm-none-eabi-size xinu.elf
	arm-none-eabi-objcopy xinu.elf -O binary xinu.bin

xinu.elf : ld.script $(OBJ)
	$(LD) $(OBJ) -T ld.script -o xinu.elf



clean:
	@rm -f $(OBJ)
	@rm -f *.elf
	@rm -f *.bin
	@rm -f *.list
	@rm -f *.o


flash:
	st-flash write xinu.bin 0x08000000
	st-flash reset


TO_ADDR =  0x800c000
TO_ADDR2 =  0x08010000
blink: 
	#arm-none-eabi-gcc -O0 -c -mcpu=cortex-m4 -mthumb   app/app_startup.s  -g -std=c11 -W -Wall -pedantic
	arm-none-eabi-gcc  -O0 -c -mcpu=cortex-m4 -mthumb  app/blink.c -fpic 
	arm-none-eabi-gcc -O0 -c -mcpu=cortex-m4 -mthumb  app/syscall.s -fpic 
	arm-none-eabi-gcc  -O0 -c -mcpu=cortex-m4 -mthumb  -I include  app/doprnt.c -fpic 

	arm-none-eabi-ld blink.o syscall.o doprnt.o -nostartfiles -nostdlib -T app/app_base.ld  -o blink.elf 

	arm-none-eabi-objdump -d blink.elf
	#arm-none-eabi-objcopy blink.elf -O binary blink.bin
	#gcc  -o dump dump.c
	#./dump blink.bin
	arm-none-eabi-size blink.elf
	#$(STRIP) -s -o blink-striped.elf blink.elf
	#python3 elf2tinf.py blink.elf blink.tinf
	#cat blink_tinf.h
	gcc  -o got2 app/got2.c 
	./got2 blink.elf
	st-flash write blink.elf $(TO_ADDR)

getpin: 
	#arm-none-eabi-gcc -O0 -c -mcpu=cortex-m4 -mthumb   app/app_startup.s  -g -std=c11 -W -Wall -pedantic
	arm-none-eabi-gcc  -O0 -c -mcpu=cortex-m4 -mthumb  app/getpin.c -fpic 
	arm-none-eabi-gcc -O0 -c -mcpu=cortex-m4 -mthumb  app/syscall.s -fpic 
	arm-none-eabi-gcc  -O0 -c -mcpu=cortex-m4 -mthumb  -I include  app/doprnt.c -fpic 

	arm-none-eabi-ld getpin.o syscall.o doprnt.o -nostartfiles -nostdlib -T app/app_base.ld  -o getpin.elf 

	arm-none-eabi-objdump -d getpin.elf
	#arm-none-eabi-objcopy getpin.elf -O binary getpin.bin
	#gcc  -o dump dump.c
	#./dump blink.bin
	arm-none-eabi-size getpin.elf
	#$(STRIP) -s -o getpin-striped.elf getpin.elf
	#python3 elf2tinf.py blink.elf blink.tinf
	#cat blink_tinf.h
	st-flash write getpin.elf $(TO_ADDR2)
test:
	#gcc  -o b16 app/b16.c
	#./b16
	#gcc  -o got app/got.c
	#./got
	gcc -m32 -o dump app/dump.c
	./dump blink.elf
	gcc  -o got2 app/got2.c 
	./got2 blink.elf
	arm-none-eabi-objdump -D -bbinary -marm blinknew.bin -Mforce-thumb 
view:
	arm-none-eabi-readelf -a blink.elf
	arm-none-eabi-objdump -D blink.elf

#arm-none-eabi-gcc -O0 -c -mcpu=cortex-m4 -mthumb   app/app_startup.s -fpic -mno-pic-data-is-text-relative 
#arm-none-eabi-gcc -O0  -c -mcpu=cortex-m4 -mthumb   app/blink.c -fpic -mno-pic-data-is-text-relative 
#arm-none-eabi-gcc -O0 -c -mcpu=cortex-m4 -mthumb   system/intr.S -fpic -mno-pic-data-is-text-relative 

#arm-none-eabi-ld app_startup.o blink.o intr.o  -T app/code_before_data.ld  -o blink.elf 