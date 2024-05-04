core:
	riscv32-unknown-elf-gcc -march=rv32i -mabi=ilp32 -Wl,-Bstatic,-T,sections.ld,--strip-debug -ffreestanding -nostartfiles -o kern.elf start.s kern.c
	riscv32-unknown-elf-objcopy -O binary kern.elf kern.bin


run:
	