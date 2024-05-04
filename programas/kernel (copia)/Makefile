
CROSS_COMPILE = arm-none-eabi-
CC              = $(CROSS_COMPILE)gcc
CFLAGS += -mthumb  -Werror -Os -mcpu=cortex-m0 -lgcc -lm -nostartfiles -ffreestanding -I include

OBJS += system/start.s
OBJS += system/*.c
OBJS += lib/*.c
OBJS += shell/*.c
OBJS += main/*.c
OBJS += device/tty/*.c
OBJS += device/gpio/*.c
OBJS += device/nam/*.c
OBJS += fatfs/*.c
OBJS += littlefs/*.c


ELF = kern.elf
BIN = kern.bin

project:	$(OBJS)
	clear
	clear
	$(CC) $(CFLAGS) $(OBJS) -T rp2040.ld -o $(ELF)
	#arm-none-eabi-gcc -O0 -nostartfiles exception.c init.c main.c serial.c -o kern.elf linker_script.ld -mthumb -mcpu=cortex-m0
	arm-none-eabi-objcopy -O binary  $(ELF) kern.bin
	arm-none-eabi-objdump -d $(ELF)
	g++ ./elf2uf2/main.cpp -o makeuf2
	./makeuf2 kern.elf main.uf2
	arm-none-eabi-size -t kern.elf



clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.uf2
	rm -f makeuf2
	
