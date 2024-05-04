
CROSS_COMPILE = arm-none-eabi-
CC              = $(CROSS_COMPILE)gcc
CFLAGS +=  -fpie -mthumb  -Werror -Os -mcpu=cortex-m0 -nostartfiles -ffreestanding 

OBJS += main.c



ELF = kern.elf
BIN = kern.bin

project:	$(OBJS)
	clear
	clear
	$(CC) $(CFLAGS) $(OBJS) -T link.ld -o $(ELF)
	#arm-none-eabi-gcc -O0 -nostartfiles exception.c init.c main.c serial.c -o kern.elf linker_script.ld -mthumb -mcpu=cortex-m0
	arm-none-eabi-objcopy -O binary  $(ELF) kern.bin
	arm-none-eabi-objdump -d $(ELF)
	arm-none-eabi-size -t kern.elf



clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.uf2
	rm -f makeuf2
	
