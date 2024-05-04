/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <stdio.h>
#include <elf.h>

#define FROM_ADDR_2  0x08010000

shellcmd xsh_elf2(int nargs, char *args[])
{
    program_img prg1 = { .img = (void*)FROM_ADDR_2 };

    exec_img ximg;

    int r = load_elf(&prg1, &ximg);
    int (*p)(void)= (int *)ximg.entry;

    int re = p();
    printf("result %d\n", re);
    free(ximg.start);
	return 0;
}
