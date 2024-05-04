#include <stdarg.h>
#include <kernel.h>
#include <stdint.h>
#include <stdio.h>
#include <interrupt.h>
#include <string.h>
//#include <uart.h>
#include <syscall.h>

#include <usb_cdc_conf.h>

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
  while(1){
      int len = usbd_ep_write(&udev, CDC_TXD_EP, &ch, 1);
      if (len < 0){
          continue;
      }else break;
  }
  //putchar(ch);
  irq_restore(q);//

  return OK;
}

int lib_puts(char *str, int size)
{
 
  /*uint32 q = irq_disable();//
  while(1){
      int len = usbd_ep_write(&udev, CDC_TXD_EP, str, size);
      if (len < 0){
          continue;
      }else break;
  }
  //putchar(ch);
  irq_restore(q);//*/

  syscall(XINU_PUTS,str,size);

  return OK;
}



uint32 kprintf(const char *fmt, ...){

   va_list ap; 
 
   va_start(ap, fmt);
   _doprnt((char *)fmt, ap, lib_putc);
   va_end(ap);
 
   return OK;
}
