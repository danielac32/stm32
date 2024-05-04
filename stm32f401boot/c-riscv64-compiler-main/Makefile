CFLAGS=-std=c11 -g -fno-common

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

TEST_SRCS=$(wildcard test/*.c)
TESTS=$(TEST_SRCS:.c=.exe)

zhizhicc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJS): zhizhicc.h

test/%.exe: zhizhicc test/%.c
	$(CC) -o- -E -P -C test/$*.c | ./zhizhicc -o test/$*.s -
	riscv64-unknown-elf-gcc -o $@ test/$*.s -xc test/common

test: $(TESTS)
	for i in $^; do echo $$i; qemu-riscv64 ./$$i || exit 1; echo; done
	test/driver.sh

clean:
	rm -rf zhizhicc tmp* $(TESTS) test/*.s test/*.exe
	find * -type f '(' -name '*~' -o -name '*.o' ')' -exec rm {} ';'



asm:
	riscv64-unknown-elf-gcc  -ffreestanding -nostdlib -nostartfiles -lgcc -fno-builtin   entry.s  -o rv32 -T link.ld -march=rv32im -mabi=ilp32
	riscv64-unknown-elf-objcopy -O binary rv32 rv32.bin
	./mini-rv32ima -f rv32.bin
bare:
	riscv64-unknown-elf-gcc -I /usr/include -ffreestanding -nostdlib -nostartfiles -lgcc -fno-builtin   rv/baremetal.S rv/baremetal.c -o rv/rv32 -T rv/flatfile.lds -march=rv32im -mabi=ilp32
	riscv64-unknown-elf-objcopy -O binary rv/rv32 rv/rv32.bin
	./mini-rv32ima -f rv/rv32.bin

.PHONY: test clean asm
