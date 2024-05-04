
#include <os.h>


void __attribute__((naked))  svccall_handler_c(uint32_t *sp){
uint32_t mask=disable();

uint32_t svc_number = ((char *)sp[6])[-2];
uint32_t svc_arg1 = sp[0];
uint32_t svc_arg2 = sp[1];
uint32_t result = svc_arg1 + svc_arg2;

printf("svc Number: %d\n", svc_number);
switch(svc_number){
case OS_NULLPROCESS:
     printf("OS_NULLPROCESS\n");
break;
case OS_UPTIME:

break;
case OS_PUTC:
    {
      char *p;
      printf("prueba\n");
      printf("%d\n",svc_arg1 );
      printf("%d\n",svc_arg2 );
   
    }
break;
case OS_CLOSE:

break; /* Devices */
case OS_CONTROL:

break;
case OS_GETC:

break;
case OS_INIT:

break;
case OS_OPEN:

break;
case OS_READ:

break;
case OS_SEEK:

break;
case OS_WRITE:

break;

case OS_CREATE:

break; /* Processes */ 
case OS_CHPRIO:

break;
case OS_KILL:

break;/* Exit */
case OS_GETPRIO:

break;
case OS_READY:

break;
case OS_RECEIVE:

break;
case OS_RECVCLR:

break;
case OS_RECVTIME:

break;
case OS_RESCHED_CNTL:

break;
case OS_RESUME:

break;
case OS_SEND:

break;
case OS_SIGNAL:

break;
case OS_SIGNALN:

break;
case OS_SLEEP:

break;
case OS_SLEEPMS:

break;
case OS_SUSPEND:

break;
case OS_UNSLEEP:

break;
case OS_WAKE:

break;
case OS_WAKEUP:

break; 
case OS_YIELD:

break;

case OS_MKBUFPOOL:

break; /* Buffers */
case OS_FREEBUF:

break;
case OS_FREEMEM:

break;
case OS_GETBUF:

break;

case OS_KPRINTF:

break; /* Kernel */
case OS_KPUTC:

break;
case OS_GETDEV:

break;
case OS_GETMEM:

break;
case OS_GETSTK:

break;

case OS_PTCOUNT:

break; /* Ports */
case OS_PTCREATE:

break;
case OS_PTDELETE:

break;
case OS_PTINIT:

break;
case OS_PTRCV:

break;
case OS_PTRESET:

break;

case OS_SEMCOUNT:

break; /* Semaphores */
case OS_SEMCREATE:

break;
case OS_SEMRESET:

break;
case OS_SEMDELET:

break;
  }
  restore(mask);
  asm volatile(
               "ldr r0,=0xFFFFFFF9\n"
               "bx r0\n");
}
