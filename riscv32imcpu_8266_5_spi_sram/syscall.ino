#include "memory.h"
#include "cpu.h"
#include "syscall.h"


uint32_t temp_seek=0;


void syscall_handler(uint32_t code){
 //SYSCALL INTERNAL CPU
     switch(code){
       //files
      /* case SYSCALL_READ:
           {
                 if(filebin!=NULL){
                    riscv.wreg1=getc(filebin);  
                 }
           }
       break;
       case SYSCALL_READBUFF:
            {
                  uint32_t size=riscv.reg[10];//a0,
                  uint32_t addr=riscv.reg[11];//a1
                  uint16_t k=0;
                  char *p=(char *)calloc(size,sizeof(char));
                  if(filebin){
                     
                     fread(p, sizeof(char), size, filebin);

                     for(uint32_t i=addr;i<size+addr;i++){
                         write_vm(i,p[k]);
                         k++;
                     }
                  }
                  free(p);
            }
       break;
       case SYSCALL_WRITE:

       break;
       case SYSCALL_WRITEBUFF:

       break;
       case SYSCALL_OPEN:
            {
                  int i=0;
                  int j=0;
                  uint32_t point=riscv.reg[10];
                  strcpy(riscv.name,"");
                  while(read_vm(point)!='\0'){
                        riscv.name[i]=read_vm(point);
                        point++;
                        i++;
                  }
                  riscv.name[i]='\0';
                  i=strlen(riscv.name);
                  filebin = fopen(riscv.name, "rb");
                  if(filebin == NULL){
                     riscv.wreg1=1;
                  }else{
                     riscv.wreg1=0;
                  }
            }
       break;
       case SYSCALL_CLOSE:
            {
                  if(filebin!=NULL){
                     fclose(filebin);
                  }
            }
       break;
       case SYSCALL_SEEK:
            {
                  uint32_t point=riscv.reg[10];
                  if(filebin){
                     temp_seek=point;
                     fseek(filebin, point, SEEK_SET);
                  }
            }
       break;
       case SYSCALL_FSIZE:
            {     
                  if(filebin!=NULL){
                     fseek(filebin, 0, SEEK_END);
                     riscv.wreg1 = ftell(filebin);
                     rewind(filebin);
                  }
            }
       break;*/
       
      
      /* //MALLOC
       case SYSCALL_MALLOC:

       break;
       case SYSCALL_FREE:

       break;
       //task
       case SYSCALL_CREATETASK:

       break;
       case SYSCALL_DELETETASK:

       break;
       case SYSCALL_PID:

       break;
       case SYSCALL_GETPID:

       break;
       case SYSCALL_YIELD:
            
       break;
       case SYSCALL_WAIT:

       break;
       case SYSCALL_SLEEP:

       break;
       case SYSCALL_EXIT:

       break;
       case SYSCALL_KILL:
            
       break;*/
       //SERIAL
       case SYSCALL_PRINTSTR:
            {
                  uint32_t point=riscv.reg[10];
                  while(read_vm(point)!='\0'){
                       // printf("%c",read_vm(point));
                        Serial.write(read_vm(point));
                        point++;
                  }
            }
       break;
       case SYSCALL_PRINTCHAR:   
            {
                  uint32_t point=riscv.reg[10];
                  Serial.write(read_vm(point));//printf("%c",(char)point);
            }
       break;

       case SYSCALL_EXIT2:
            Serial.println("EXIT RISCV32\n");//printf("EXIT RISCV32\n");
            riscv.running=0;
       break;
    }
}
