
#include <os.h>

 

 
extern int fs();
extern void nokia();
extern void blink();
extern void terminal();




char *txt="1234567890\n";



char prog[]={
0x00,0xe0,0x00,0x00,0x05,0x4b,0x7b,0x44,0x1b,0x68,0x18,0x1c,0x04,0x4b,0x7b,0x44,
0x1b,0x68,0x19,0x1c,0x02,0xdf,0xc0,0x46,0xfd,0xe7,0xc0,0x46,0x22,0x00,0x00,0x00,
0x1e,0x00,0x00,0x00,0x64,0x61,0x6e,0x69,0x65,0x6c,0x00,0x61,0x1e,0x00,0x00,0x24,
0x00,0x00,0x00}; 


void ilde() {

   while (1) {
       PEND_SV();
  }
}




int main()
{
	disable();
    //meminit();
    //list();
     //mm_init();


    malloc_init(KMALLOC_START, KMALLOC_LENGTH);
    malloc_debug();
    systick_init();
    initialice();
    
    //write(CONSOLE,txt,strlen(txt));//test

    

    int pid=create((void *)blink,STACK_SIZE,1,"blink",0); 
    ready(pid);

    pid=create(fs,STACK_SIZE*3,1,"fs",0); 
    ready(pid);

    pid=create((void *)ilde,256,1,"ilde",0); 
    ready(pid);

    
    pid=create((void *)nokia,STACK_SIZE,1,"nokia",0); 
    ready(pid);
    
    pid=create((void *)terminal,STACK_SIZE,1,"shell",0); 
    ready(pid);


    systick_set(500);
    enable();
    PEND_SV();//rtos_start();
	return 0;
}