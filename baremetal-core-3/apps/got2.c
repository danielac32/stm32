#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct {
    uint8_t app_name[16];
    uint32_t size;
    uint32_t entry;
    uint8_t bin[10000];
}tinf_t;

void hexDump(uint32_t offset,char *desc, void *addr, int len) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = addr;


    printf("(%s):\n", desc);

    
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




typedef struct {
    void *img;
} program_img;

typedef struct {
    void *start, *end;
    void *entry;
} exec_img;

#define E_NOT_ELF_X       1
#define E_NOT_COMPATIBLE  2




typedef uint32_t elf32_addr;  // unsigned address
typedef uint32_t elf32_off;   // unsigned 4-byte integer
typedef uint16_t elf32_half;  // unsigned 2-byte integer
typedef uint32_t elf32_word;  // unsigned 4-byte integer
typedef int32_t  elf32_sword; // signed 4-byte integer
typedef uint8_t  elf32_byte;  // unsigned byte

typedef struct {
    elf32_byte e_ident[16]; // machine-independent identification info
    elf32_half e_type;      // object file type
    elf32_half e_machine;   // architecture
    elf32_word e_version;   // object file standard version
    elf32_addr e_entry;     // entry point
    elf32_off  e_phoff;     // program header table's offset
    elf32_off  e_shoff;     // section header table's offset
    elf32_word e_flags;     // processor-specific flags
    elf32_half e_ehsize;    // ELF header size
    elf32_half e_phentsize; // program header entry size (all program headers are the same size)
    elf32_half e_phnum;     // number of entries in the program header table
    elf32_half e_shentsize; // section header entry size (all section headers are the same size)
    elf32_half e_shnum;     // number of entries in the section header table
    elf32_half e_shstrndx;  // string table section index
} ELF32_hdr;
#define EID0        0x7f // ELF format magic number
#define EV_CURRENT  1    // ELF format version
#define EC_32       1    // 32-bit arch
#define EM_ARM      0x28 // for ARM CPUs
#define ET_EXEC     2    // type: normal executable
#define ET_DYN      3    // type: PIE (or a shared library, but shared libraries are useless now)

typedef struct {
    elf32_word sh_name;      // index into the section name string table
    elf32_word sh_type;
    elf32_word sh_flags;
    elf32_addr sh_addr;      // address in the memory image that this should appear at
    elf32_off  sh_off;       // offset from the file start to the section
    elf32_word sh_size;      // section size (in file, unless it is type SH_NOBITS - like .bss)
    elf32_word sh_link;      // type-dependent
    elf32_word sh_info;      // type-dependent
    elf32_word sh_addralign; // alignment constraints (sh_addr needs to be a multiple of this)
    elf32_word sh_entsize;   // if it is a table of fixed-size entries, then this is that size
} ELF32_shdr;

enum sh_type {
    SHT_NULL     =  0, // UNDEF
    SHT_PROGBITS =  1, // program info
    SHT_SYMTAB   =  2, // symbol table
    SHT_STRTAB   =  3, // string table
    SHT_RELA     =  4, // relocation (with addend)
    SHT_HASH     =  5, // symbol hash table
    SHT_DYNAMIC  =  6, // info for dynamic linking
    SHT_NOTE     =  7,
    SHT_NOBITS   =  8, // not present in the file
    SHT_REL      =  9, // relocation (no addend)
    SHT_SHLIB    = 10, // no specified semantics
    SHT_DYNSYM   = 11, // dynamic symbols table
};
enum sh_flags {
    SHF_WRITE = 0x1,
    SHF_ALLOC = 0x2,
    SHF_EXEC  = 0x4,
};

//int load_elf(program_img *prg/*, exec_img *res*/,char *out);

#define ALIGN(to, x)  ((x+(to)-1) & ~((to)-1))

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
int load_elf(void *prg) {
    // note: I am assuming my binary starts at 0x0 throughout this function
    uintptr_t from_addr = (uintptr_t)prg; // TODO this should read a file one day
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
            int align = (sec_idx+1 < hdr->e_shnum) ? (&sections[sec_idx+1])->sh_addralign : 1;
            size += ALIGN(align, shdr->sh_size);
        }
    }
    void* to_addr = (void *)malloc(size);
    //res->start = (void*)to_addr; 
    //res->end = (void*)(to_addr + size);//stack
  
    // pass 2: relocate GOT, prepare .bss, load .text and .data
    for (int sec_idx = 0; sec_idx < hdr->e_shnum; ++sec_idx) {
        ELF32_shdr *shdr = &sections[sec_idx];
        if (shdr->sh_flags & SHF_ALLOC && shdr->sh_size) { // non-empty allocatable sections
            uintptr_t daddr = to_addr + shdr->sh_addr; // poor man's relocation :D
            char *name = strings + shdr->sh_name;


            int32_t idx;

            printf(" %-20s ",name);
            printf(" addr: %08x ", shdr->sh_addr);
            printf(" size: %08x ", shdr->sh_size);
            printf(" offset: %08x \n", shdr->sh_off);



            if(memcmp(name, ".got", 4) == 0) { // handle relocation -- copy adding offset
                // TODO assert that shdr->sh_entsize is 4 bytes
                //hexDump (shdr->sh_addr,"got after relocation",from_addr + shdr->sh_off, shdr->sh_size);
                uint32_t *src  = (uint32_t*)(from_addr + shdr->sh_off);
                uint32_t *dest = (uint32_t*)daddr;
                while ((uintptr_t)dest < (uint32_t)(daddr + shdr->sh_size)) {
                    *dest++ = to_addr + *src++; // to_addr is also the offset (as start is at 0x0)
                }
            }else {
                switch (shdr->sh_type) {
                    case SHT_NOBITS: // .bss
                        memset((void*)daddr, 0, shdr->sh_size);
                        break;
                    case SHT_PROGBITS: // .data, .rodata, .text (should use FLAGS to set rwx perms)
                        memcpy((void*)daddr, (void*)(from_addr + shdr->sh_off), shdr->sh_size);
                        break;
                }
            }
            //hexDump (shdr->sh_addr,name,daddr, shdr->sh_size);
        }
    }

    //hexDump (0,"bin",to_addr, size);

  
    //res->entry = (void*)make_code_addr(to_addr + hdr->e_entry);
    //res->entry = (void*)((to_addr + hdr->e_entry) | 0x1);

    //printf("%x\n",res->start - to_addr);
    //printf("%x\n",res->end - to_addr);
    //printf("%x %x\n",res->entry,hdr->e_entry);
    



    FILE *fptr;
    //hexDump(0,NULL, to_addr, size);
    if ((fptr = fopen("out.bin","w+")) == NULL){
       printf("Error! opening file");
       return -1;
    }


    fwrite(to_addr, size, 1, fptr); 
    fclose(fptr);

   /* FILE *fp;
    tinf_t image;


    strcat(image.app_name,"test.bin");
    image.size=size;
    image.entry=hdr->e_entry;
    memcpy(image.bin,to_addr,size);
    fp = fopen(out,"w+");

    fwrite(&image.bin , 1 ,size , fp );
    fclose(fp);*/
    //printf("size: %d entry: %d name: %s\n",image.size,image.entry,image.app_name );
    return 0;
}




int main(int argc, char const *argv[])
{
	//program_img prg1 = { .img = (void*)FROM_ADDR_1 };

    //exec_img *ximg;

    //int r = load_elf(&prg1, ximg);
    

    FILE *fp;
	/*fp = fopen("data.txt","w+");

	fprintf(fp, "daniel quintero\n");


    fclose(fp);*/


    fp = fopen(argv[1],"r");
    fseek(fp, 0, SEEK_END);
    uint32_t lsize = ftell(fp);
    char *p = (char *)malloc(lsize);
    rewind(fp);
	fread(p, sizeof(char), lsize, fp);
	fclose(fp);
    
    //exec_img *ximg;
    //program_img prg1 = { .img = (void*)p };

    int r = load_elf(p);


	return 0;
}