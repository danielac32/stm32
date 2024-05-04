#include <stdarg.h>
#include <kernel.h>
#include <queue.h>
#include <stdint.h>
#include <stdio.h>
//#include <process.h>
//#include <syscall.h>
#include <usbapi.h>
sem_t uart_tx;
sem_t uart_rx;
//static char out_buf[2000];
extern  void _doprnt(char *, va_list, int (*)(int));
extern void _fdoprnt(char *, va_list, int (*func) (int, int), int);

static int sprntf(int, int);

static int  sprntf(
          int       acpp,
          int       ac
        )
{
    char **cpp = (char **)acpp;
    char c = (char)ac;

    return (*(*cpp)++ = c);
}

int lib_putc(int ch)
{
    //while(usbd_ep_write(&udev, CDC_TXD_EP, &ch, 1)!=-1);
    //delay(40);
    do{
       //delay(30);
       asm("nop");
    }while(usbd_ep_write(&udev, CDC_TXD_EP, &ch, 1)==-1);
    return 0;
}

int32 sprintf(
      char      *str,       /* output string        */
      char      *fmt,       /* format string        */
      ...
    )
{
    va_list ap;
    char *s;

    s = str;
    va_start(ap, fmt);
    _fdoprnt(fmt, ap, sprntf, (int)&s);
    va_end(ap);
    *s++ = '\0';

    return ((int)str);
}

int32 printf(const char *fmt, ...){
    va_list ap; 
sem_wait(&uart_tx);
   va_start(ap, fmt);
   _doprnt((char *)fmt, ap, lib_putc);
   va_end(ap);
sem_signal(&uart_tx);
   return OK;
}


uint32 kprintf(const char *fmt, ...){
    va_list ap; 

   va_start(ap, fmt);
   _doprnt((char *)fmt, ap, lib_putc);
   va_end(ap);

   return OK;
}


#if 0
void hexDump2(uint32 offset, void *addr, int len) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = addr;

    
    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", offset+i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}

void hexDump(uint32 offset,char *desc, void *addr, int len) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = addr;

    // Output description if given.
    if (desc != NULL)
        printf("%x (%s):\n", addr, desc);
    else
        printf("%x:\n", addr);
    
    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", offset+i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}







void    panic (
     char   *msg            /* Message to display       */
    )
{
    disable();          /* Disable interrupts       */
    kprintf("\n\n\rpanic: %s\n\n", msg);
    SYSCON = 0x5555; 
}
#endif



 
/*
uint32 kprintf(const char *s, ...)
{
    int res = 0;
    va_list vl;
    //sem_wait(&uart_tx);
    
    va_start(vl, s);
    res = lib_vprintf(s, vl);
    va_end(vl);
    //sem_signal(&uart_tx);

    return res;
}*/


void hexDump2(uint32 offset, void *addr, int len) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = addr;

    
    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0){
                printf("  %s\n", buff);
                
            }

            // Output the offset.
            printf("  %04x ", offset+i);
            
        }

        // Now the hex code for the specific character.
        printf(" %02x", pc[i]);
        

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
    
}