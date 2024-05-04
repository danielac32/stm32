
# Setup the toolchain.
ARMGNU	= arm-none-eabi
AS	= $(ARMGNU)-as
CC	= $(ARMGNU)-gcc
LD	= $(ARMGNU)-ld
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
#TTY_GCC = $(wildcard $(TTY_DIR)/*.c)
#NAM_GCC = $(wildcard $(NAM_DIR)/*.c)
#SPI_GCC = $(wildcard $(SPI_DIR)/*.c)
#SHELL_GCC = $(wildcard $(SHELL_DIR)/*.c)
 
# Build the object list.
OBJ = $(SYSTEM_GCC:.c=.o)
OBJ += $(SYSTEM_ASM:.S=.o)
#OBJ += $(USB_GCC:.c=.o)
OBJ += $(LIB_GCC:.c=.o)
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

clean:
	@rm -f $(OBJ)
	@rm -f *.elf
	@rm -f *.bin
	@rm -f *.list

flash:
	st-flash write xinu.bin 0x08000000

