
#include <stdio.h>
#include <syscall.h>
#include <string.h>
#include <stdlib.h>
#include <shell.h>
#include <kernel.h>
#include <stm32f401xc.h>



int main(int argc, char const *argv[])
{

  #define NVIC_AIRCR_VECTKEY    (0x5FA << 16)   /*!< AIRCR Key for write access   */
  #define NVIC_SYSRESETREQ            2
  SCB->AIRCR  = (NVIC_AIRCR_VECTKEY | (SCB->AIRCR & (0x700)) | (1<<NVIC_SYSRESETREQ)); /* Keep priority group unchanged */                                                                         /* Ensure completion of memory access */              
  while(1);                                    
  return 0;
}