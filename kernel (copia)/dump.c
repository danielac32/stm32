
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>



void hexDump2(char *desc, void *addr, int len) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = addr;

    // Output description if given.
    if (desc != NULL)
        printf("%p (%s):\n", addr, desc);
    else
        printf("%p:\n", addr);
    


    printf("unsigned char mem[]={\n");
    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf (" %s\n");//printf ("  %s\n", buff);

            // Output the offset.
            //printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf (" 0x%02x,", pc[i]);

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
    //printf ("  %s\n", buff);
    printf("};\n");
}

void hexDump(char *desc, void *addr, int len) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = addr;

    // Output description if given.
    if (desc != NULL)
        printf("%p (%s):\n", addr, desc);
    else
        printf("%p:\n", addr);
    
    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", i);
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

int main(int argc, char const *argv[])
{
	FILE* fpSrc = fopen(argv[1], "r");
    unsigned long lsize = 0;
    char *p;
    fseek(fpSrc, 0, SEEK_END);
    lsize = ftell(fpSrc);
    rewind(fpSrc);
    p=(char*)malloc(lsize);
    fread(p, sizeof(char), lsize, fpSrc);

    fclose(fpSrc);

    printf("dump...%d\n",lsize );
    //hexDump (0, p, lsize);
    hexDump2 (0, p, lsize);
    free(p);
	return 0;
}