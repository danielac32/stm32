#include <stdarg.h>
#include <kernel.h>
#include <stdint.h>
#include <stdio.h>
#include <interrupt.h>
#include <string.h>
#include <uart.h>
//#include <usbapi.h>


extern  void _doprnt(char *, va_list, int (*)(int));

 
int lib_putc(int ch)
{
    
  /*uint32 mask;
  mask = irq_disable();

  if (c == '\n') {
      USART6->DR = 0x0D;
      while(!(USART6->SR &  USART_SR_TXE));
  }
  USART6->DR = c;
  while(!(USART6->SR &  USART_SR_TXE));
  irq_restore(mask);*/

  uint32 q = irq_disable();//
  /*while(1){
      int len = usbd_ep_write(&udev, CDC_TXD_EP, &ch, 1);
      if (len < 0){
          continue;
      }else break;
  }*/
  putchar(ch);
  irq_restore(q);//

  return OK;
}



uint32 kprintf(const char *fmt, ...){

   va_list ap; 
 
   va_start(ap, fmt);
   _doprnt((char *)fmt, ap, lib_putc);
   va_end(ap);
 
   return OK;
}


void hexDump(uint32 offset, void *addr, int len) 
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
                kprintf("  %s\n", buff);
                
            }

            // Output the offset.
            kprintf("  %04x ", offset+i);
            
        }

        // Now the hex code for the specific character.
        kprintf(" %02x", pc[i]);
        

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        kprintf ("   ");
        
        i++;
    }

    // And print the final ASCII bit.
    kprintf ("  %s\n", buff);
    
}
