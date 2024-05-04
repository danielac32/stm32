
#include <stdio.h>
#include <syscall.h>
#include <string.h>
#include <stdlib.h>
#include <shell.h>
#include <kernel.h>
#include <elf.h>

static void hexDump2(uint32 offset, void *addr, int len) 
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

int main(int nargs, char *args[]){
    void *fptr;

    if ((fptr = syscall_fopen(args[1],"r")) == NULL){
       	printf("Error! opening file");
        return -1;
    }
    uint8 *buff;
    int part=0;
    syscall_fseek(fptr, 0, SEEK_END);
    int lsize = syscall_ftell(fptr);
    printf("size: %d\n",lsize );
    #define SIZEBUF 64
    buff=syscall_malloc(SIZEBUF);

    syscall_fseek(fptr, 0, SEEK_SET);
    while(!syscall_feof(fptr)){
    	 syscall_fread(buff,SIZEBUF,1, fptr);
    	 hexDump2(part, buff, SIZEBUF);
    	 memset(buff,0,SIZEBUF);
    	 part += SIZEBUF;
    }
    syscall_free(buff);
    syscall_fclose(fptr);
    return 0;
}