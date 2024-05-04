

#define reg_uart_data (*(volatile unsigned int*)0x300000)
void putc_( char ch){
     reg_uart_data=ch;
     /*register uint32_t a7 asm("a7")=24;
     register uint32_t a0 asm("a0")=ch;   //syscall event cpu                      
     asm("ecall"); */
} 

void print( char *str){
  while(*str){
    putc_(*str++);
  }
}

int main(int argc, char const *argv[])
{
	print("hola riscv32\n");
	
	return 0;
}