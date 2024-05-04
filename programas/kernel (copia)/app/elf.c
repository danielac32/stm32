#include "elf.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include<stdlib.h>


static inline uintptr_t make_code_addr(uintptr_t addr) {
    return addr | 0x1; // Thumb mode
}

 bool is_elf_x(ELF32_hdr *hdr) {
    return hdr->e_ident[0] == EID0 &&
           hdr->e_ident[1] == 'E'  &&
           hdr->e_ident[2] == 'L'  &&
           hdr->e_ident[3] == 'F'  &&
           (hdr->e_type == ET_EXEC || hdr->e_type == ET_DYN);
}
 bool is_compatible(ELF32_hdr *hdr) {
    return hdr->e_ident[4] == EC_32      &&
           hdr->e_version  == EV_CURRENT &&
           hdr->e_machine  == EM_ARM;
}

// TODO load from something file-like
int load_elf(void *prg, exec_img *res) {
    // note: I am assuming my binary starts at 0x0 throughout this function
    const uintptr_t from_addr = (uintptr_t)prg; // TODO this should read a file one day
    ELF32_hdr *hdr = (ELF32_hdr*)prg;
    if (!is_elf_x(hdr)) {
        return E_NOT_ELF_X;
    }
    if (!is_compatible(hdr)) {
        return E_NOT_COMPATIBLE;
    }

    ELF32_shdr *sections = (ELF32_shdr*)((uintptr_t)hdr + hdr->e_shoff);
    char *strings = (char*)(from_addr + sections[hdr->e_shstrndx].sh_off);

    // pass 1: just find out the future executable image's size and allocate memory
    size_t size = 0;
    for (int sec_idx = 0; sec_idx < hdr->e_shnum; ++sec_idx) {
        ELF32_shdr *shdr = &sections[sec_idx];

        if (shdr->sh_flags & SHF_ALLOC && shdr->sh_size) { // non-empty allocatable sections
            // sh_size needs to be aligned according to the next section's addralign, as that will
            // start aligned and therefore any holes created by the alignment must be accounted for
            const int align = (sec_idx+1 < hdr->e_shnum) ? (&sections[sec_idx+1])->sh_addralign : 1;
            size += ALIGN(align, shdr->sh_size);
        }
    }
    const uintptr_t to_addr = (uintptr_t)malloc(size);
    res->start = (void*)to_addr; res->end = (void*)(to_addr + size);

    // pass 2: relocate GOT, prepare .bss, load .text and .data
    for (int sec_idx = 0; sec_idx < hdr->e_shnum; ++sec_idx) {
        ELF32_shdr *shdr = &sections[sec_idx];

        
       /* printf(" addr: %08x ", shdr->sh_addr);
	    printf(" size: %08x ", shdr->sh_size);
	    printf(" offset: %08x \n", shdr->sh_off);*/

        if (shdr->sh_flags & SHF_ALLOC && shdr->sh_size) { // non-empty allocatable sections
            uintptr_t daddr = to_addr + shdr->sh_addr; // poor man's relocation :D
            char *name = strings + shdr->sh_name;
            if (memcmp(name, ".got", 4) == 0) { // handle relocation -- copy adding offset
                // TODO assert that shdr->sh_entsize is 4 uint8_ts
                uint32_t *src  = (uint32_t*)(from_addr + shdr->sh_off);
                uint32_t *dest = (uint32_t*)daddr;

                while ((uintptr_t)dest < (uint32_t)(daddr + shdr->sh_size)) {
                    *dest++ = to_addr + *src++; // to_addr is also the offset (as start is at 0x0)
                }
            }
            else {
                switch (shdr->sh_type) {
                    case SHT_NOBITS: // .bss
                        memset((void*)daddr, 0, shdr->sh_size);
                        break;
                    case SHT_PROGBITS: // .data, .rodata, .text (should use FLAGS to set rwx perms)
                        memcpy((void*)daddr, (void*)(from_addr + shdr->sh_off), shdr->sh_size);
                        break;
                }
            }
        }
    }

    res->size=size;
    res->entry = (void*)make_code_addr(to_addr + hdr->e_entry);
    
    return 0;
}



#define DEBUG_ASCII 0x01
#define DEBUG_HEX   0x02


static void hexdump_print(uint8_t, uint8_t);
static void hexdump_print(
	 uint8_t	data,			/* Item to print		*/
	 uint8_t	mode			/* ASCII or hex mode		*/
	)
{
    switch (mode)
    {
    case DEBUG_ASCII:
        data = (' ' <= data && data <= '~') ? data : '.';
        printf( "%c", data);
        break;
    case DEBUG_HEX:
        printf( "%02x ", data);
        break;
    default:
        break;
    }
}


void	hexdump(
	 void	*buffer,		/* Addresss of memory area	*/
	 uint32_t	length,			/* Length in uint8_ts		*/
	 bool	canon			/* Print in ASCII or hex	*/
	)
{
    uint32_t m, n, remain;

    uint8_t *b = (uint8_t *)buffer;

    for (n = 0; n < length; n += 0x10) {
        printf( "%08x ", (uint32_t)buffer + n);

        remain = length - n;

        for (m = 0; m < remain && m < 0x10; m++) {
            if (m % 0x08 == 0) {
                printf( " ");
            }
            hexdump_print(b[n + m], DEBUG_HEX);
        }

        /* Pad the rest if needed */
        if (remain < 0x10) {
            for (m = 0; m < 0x10 - remain; m++) {
                if ((0 != m) && (0 == m % 0x08)) {
                    printf( " ");
                }
                printf( "   ");
            }
        }

        if (canon == true) {
            printf( " |");
            for (m = 0; m < remain && m < 0x10; m++) {
                hexdump_print(b[n + m], DEBUG_ASCII);
            }
            printf( "|");
        }
        printf( "\n");
    }
}




int main(int argc, char const *argv[])
{
	exec_img ximg;
	FILE *elffile;
    uint32_t lsize;
    char buff[512000];
	elffile=fopen(argv[1],"r");
	//elffile=fopen("rv32.elf","r");
	if(!elffile)exit(-1);
	fseek(elffile, 0, SEEK_END);
    lsize = ftell(elffile);
    rewind(elffile);
    //buff = (char *)malloc(lsize*sizeof(char));
  	fread(buff,sizeof(buff),1, elffile);
    //program_img *file=(void *)buff;
    int r = load_elf((void *)buff, &ximg);
    
    hexdump(ximg.start,ximg.size,true);
    printf("entry: %08p\n",ximg.entry );
    printf("sp: %08p\n",ximg.end );

	return 0;
}