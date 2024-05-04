BINUTILSDIR=../../binutils/bin
LCCDIR=../../lcc/bin
SRCDIR=./src/$1
RESDIR=./result/$1

rm -f -r result
mkdir result 
mkdir ${RESDIR}

${LCCDIR}/lcc  -Wo-lccdir=${LCCDIR} -S -target=riscv32 pointer.c -o main.s
${LCCDIR}/lcc  -Wo-lccdir=${LCCDIR} -S -target=riscv32 io.c -o io.s
${BINUTILSDIR}/as -o ${RESDIR}/asm.o asm.s
${BINUTILSDIR}/as -o ${RESDIR}/start.o start.s
${BINUTILSDIR}/as -o ${RESDIR}/io.o io.s
${BINUTILSDIR}/as -o ${RESDIR}/main.o main.s
${BINUTILSDIR}/ld -h -o ${RESDIR}/bin.bin  ${RESDIR}/start.o ${RESDIR}/io.o ${RESDIR}/main.o 

${BINUTILSDIR}/ld -h -o ${RESDIR}/test.bin  ${RESDIR}/asm.o
gcc -m32 -o dump dump.c
./dump result/bin.bin
cp result/bin.bin /home/daniel/Escritorio/res/riscv32_2/riscv32/emulator_riscv32im
cp result/test.bin /home/daniel/Escritorio/res/riscv32_2/riscv32/emulator_riscv32im
gcc -m32 -o vm cpu.c 
gcc -m32 -o rv32 riscv.c 
./rv32 result/bin.bin