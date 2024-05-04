/* printf.c - printf*/

#include <kernel.h>
#include <stdio.h>
#include <stdarg.h>
#include <syscall.h>
#include <stdlib.h>

extern	void	_doprnt(char *, va_list, int (*)(int));

int _printf(
      const char        *fmt,
      ...
    )
{
    va_list ap;

    va_start(ap, fmt);
    _doprnt((char *)fmt, ap, syscall_putc);
    va_end(ap);

    return 0;
}


 