
#include <stdio.h>
#include <syscall.h>
#include <string.h>
#include <stdlib.h>
#include <shell.h>
#include <kernel.h>

#define KMALLOC_STATE_FREE 0xa1a1a1a1
#define KMALLOC_STATE_USED 0xbfbfbfbf

struct kmalloc_chunk {
    int state;
    int length;
    //char pid;
    struct kmalloc_chunk *next;
    struct kmalloc_chunk *prev;
};



int main(int argc, char const *argv[])
{


  //int mode = atoi(argv[0]);
  //if (mode == 1){
      struct kmalloc_chunk *c;
      struct kmalloc_chunk *buff=(struct kmalloc_chunk*)__syscall(XINU_KMALLOC_CHUNK);
      //printf("state ptr      prev     next     length  ptr(%x)\n",KMALLOC_START);
      printf("\n");
      for(c = buff; c; c = c->next) {
        if(c->state == KMALLOC_STATE_FREE) {
          printf("Free");
        } else if(c->state == KMALLOC_STATE_USED) {
          printf("Used");
        } else {
          printf("kmalloc list corrupted at %x!\n", c);
          return;
        }
        //printf("  %x         %d       %x       %x       %d\n", c,c, c->prev, c->next, c->length);
        printf("\t%x ", c);
        printf("\t%d bytes", c->length);
        printf("\n");
      }
  //}else{
    //  printf("free %d\n",syscall_heap_free());
  //}
	

 /**/



    


	return 0;
}