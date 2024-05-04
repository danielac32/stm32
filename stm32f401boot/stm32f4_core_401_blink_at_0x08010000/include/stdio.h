/*
 * Copyright (C) 2013, 2014 F4OS Authors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <kernel.h>
#include <queue.h>
 
extern sem_t uart_tx;
extern sem_t uart_rx;
extern sem_t uart_usb;

extern uint32 kprintf(const char *, ...);
extern void    kpanic (
     char   *msg            /* Message to display       */
    );



int32 sprintf(
      char      *str,       /* output string        */
      char      *fmt,       /* format string        */
      ...
    );


int32 sprintf2(
      char      *str,       /* output string        */
      char      *fmt,       /* format string        */
      ...
    );


extern  int32   printf(const char *, ...);
extern void hexDump(uint32 offset, void *addr, int len);
extern void hexDump2(uint32 offset, void *addr, int len);
#endif
