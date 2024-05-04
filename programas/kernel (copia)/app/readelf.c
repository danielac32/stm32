


#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<stdint.h>

typedef uint32_t	Elf32_Addr;
typedef uint16_t	Elf32_Half;
typedef uint32_t	Elf32_Off;
typedef int32_t	Elf32_Sword;
typedef uint32_t	Elf32_Word;

#define EI_NIDENT	16

typedef struct elf32_hdr{
  unsigned char	e_ident[EI_NIDENT];
  Elf32_Half	e_type;
  Elf32_Half	e_machine;
  Elf32_Word	e_version;
  Elf32_Addr	e_entry;  /* Entry point */
  Elf32_Off	e_phoff;
  Elf32_Off	e_shoff;
  Elf32_Word	e_flags;
  Elf32_Half	e_ehsize;
  Elf32_Half	e_phentsize;
  Elf32_Half	e_phnum;
  Elf32_Half	e_shentsize;
  Elf32_Half	e_shnum;
  Elf32_Half	e_shstrndx;
} Elf32_Ehdr;

typedef struct elf32_shdr {
  Elf32_Word	sh_name;
  Elf32_Word	sh_type;
  Elf32_Word	sh_flags;
  Elf32_Addr	sh_addr;
  Elf32_Off	sh_offset;
  Elf32_Word	sh_size;
  Elf32_Word	sh_link;
  Elf32_Word	sh_info;
  Elf32_Word	sh_addralign;
  Elf32_Word	sh_entsize;
} Elf32_Shdr;


typedef struct elf32_sym{
  Elf32_Word	st_name;
  Elf32_Addr	st_value;
  Elf32_Word	st_size;
  unsigned char	st_info;
  unsigned char	st_other;
  Elf32_Half	st_shndx;
} Elf32_Sym;

typedef struct elf32_phdr{
  Elf32_Word	p_type;
  Elf32_Off	p_offset;
  Elf32_Addr	p_vaddr;
  Elf32_Addr	p_paddr;
  Elf32_Word	p_filesz;
  Elf32_Word	p_memsz;
  Elf32_Word	p_flags;
  Elf32_Word	p_align;
} Elf32_Phdr;

/* sh_type */
#define SHT_NULL	0
#define SHT_PROGBITS	1
#define SHT_SYMTAB	2
#define SHT_STRTAB	3
#define SHT_RELA	4
#define SHT_HASH	5
#define SHT_DYNAMIC	6
#define SHT_NOTE	7
#define SHT_NOBITS	8
#define SHT_REL		9
#define SHT_SHLIB	10
#define SHT_DYNSYM	11
#define SHT_NUM		12
#define SHT_LOPROC	0x70000000
#define SHT_HIPROC	0x7fffffff
#define SHT_LOUSER	0x80000000
#define SHT_HIUSER	0xffffffff

/* sh_flags */
#define SHF_WRITE		0x1
#define SHF_ALLOC		0x2
#define SHF_EXECINSTR		0x4
#define SHF_RELA_LIVEPATCH	0x00100000
#define SHF_RO_AFTER_INIT	0x00200000
#define SHF_MASKPROC		0xf0000000

/* special section indexes */
#define SHN_UNDEF	0
#define SHN_LORESERVE	0xff00
#define SHN_LOPROC	0xff00
#define SHN_HIPROC	0xff1f
#define SHN_LIVEPATCH	0xff20
#define SHN_ABS		0xfff1
#define SHN_COMMON	0xfff2
#define SHN_HIRESERVE	0xffff

#define ELF_ST_BIND(x)		((x) >> 4)
#define ELF_ST_TYPE(x)		(((unsigned int) x) & 0xf)
#define ELF32_ST_BIND(x)	ELF_ST_BIND(x)
#define ELF32_ST_TYPE(x)	ELF_ST_TYPE(x)

struct MEM{
	uint32_t addr;
	uint32_t offset;
	uint32_t size;
	uint8_t *data;
};
struct MEM rodata;
struct MEM data;
struct MEM sdata;
struct MEM text;
struct MEM bss;


void read_ELF_file_header(FILE* fp,Elf32_Ehdr *filehdr)
{
	//rewind(fp);	
	fread(filehdr,sizeof(Elf32_Ehdr),1,fp);
}
int is_ELF(Elf32_Ehdr *hdr)
{
	if(strncmp(hdr->e_ident,"\177ELF",4)==0)
		return 1;
	else
		return -1;
}
int read_ELF_program_header(int32_t sect_num,Elf32_Phdr *prg_hdr,FILE *fp)
{
	int32_t numsect;
	Elf32_Ehdr elfhdr;
	off_t prg_hdr_off;
	fseek(fp,(off_t)0,SEEK_SET);
	read_ELF_file_header(fp,&elfhdr);
	numsect=elfhdr.e_shnum;
	if((numsect<sect_num)||(sect_num<0))
		return -1;
	prg_hdr_off=elfhdr.e_phoff;
	prg_hdr_off+=elfhdr.e_phentsize*sect_num;
	fseek(fp,(off_t)prg_hdr_off,SEEK_SET);
	fread(prg_hdr,sizeof(Elf32_Phdr),1,fp);
	return 1;
}

int read_ELF_section_header(int32_t sect_num,Elf32_Shdr *sect_hdr,FILE *fp)
{
	int32_t numsect;
	Elf32_Ehdr elfhdr;
	off_t sect_hdr_off;
	fseek(fp,(off_t)0,SEEK_SET);
	read_ELF_file_header(fp,&elfhdr);	
	numsect=elfhdr.e_shnum;
	if ((numsect<sect_num)||(sect_num<0))
		return -1;
	sect_hdr_off=elfhdr.e_shoff;
	//rewind(fp);
	sect_hdr_off+=elfhdr.e_shentsize*sect_num;
	fseek(fp,(off_t)sect_hdr_off,SEEK_SET);
	fread(sect_hdr,sizeof(Elf32_Shdr),1,fp);
	return 1;	
}

void process_sect_hdr(Elf32_Shdr *sect_hdr,char *sect_name_buff,int i)
{
	int32_t idx;
	idx=sect_hdr->sh_name;
	printf(" %-20s ",sect_name_buff+idx);
	printf(" addr: %08lx ", sect_hdr->sh_addr);
	printf(" size: %08lx ", sect_hdr->sh_size);
	printf(" offset: %08lx \n", sect_hdr->sh_offset);
	if(i==1){//text
	   text.addr=sect_hdr->sh_addr;
	   text.offset=sect_hdr->sh_offset;
	   text.size=sect_hdr->sh_size;
	}else if (i==2){//rodata
	   rodata.addr=sect_hdr->sh_addr;
	   rodata.offset=sect_hdr->sh_offset;
	   rodata.size=sect_hdr->sh_size;
	}else if (i==3){//data
	   data.addr=sect_hdr->sh_addr;
	   data.offset=sect_hdr->sh_offset;
	   data.size=sect_hdr->sh_size;
	}else if (i==4){//sdata
	   sdata.addr=sect_hdr->sh_addr;
	   sdata.offset=sect_hdr->sh_offset;
	   sdata.size=sect_hdr->sh_size;
	}else if (i==5){//bss
	   bss.addr=sect_hdr->sh_addr;
	   bss.offset=sect_hdr->sh_offset;
	   bss.size=sect_hdr->sh_size;
	}
}
void display_sections(FILE *fp)
{
	Elf32_Ehdr ehdr;
	Elf32_Shdr sec_hdr;
	int num_hdrs,i;
	char *buff=NULL;	
	fseek(fp,(off_t)0,SEEK_SET);
	read_ELF_file_header(fp, &ehdr);
	if(is_ELF(&ehdr)==-1)
	{
		printf("Not an ELF file\n");
		exit(0);
	}
	num_hdrs=ehdr.e_shnum;
	if(read_ELF_section_header(ehdr.e_shstrndx,&sec_hdr,fp)==-1)
	{
		printf("Error: reading section string table sect_num= %d\n",ehdr.e_shstrndx);
		exit(0);
	}
	//we read the shstrtab
	// read content from the file
	buff=(char*)malloc(sec_hdr.sh_size);
	if (!buff)
	{
		printf("Malloc failed to allocate buffer for shstrtab\n");
		exit(0);
	}
	//seek to the offset in the file,
	fseek(fp,(off_t)sec_hdr.sh_offset,SEEK_SET);
	fread(buff,sec_hdr.sh_size,1,fp);
	printf("There are [%d] sections\n",num_hdrs);	
	for(i=0;i<num_hdrs;i++)
	{
		if(read_ELF_section_header(i,&sec_hdr,fp)==-1)
		{
			printf("Wrong Section to read\n");
		}
		else
		{
			printf("[section %3d] ",i);
			process_sect_hdr(&sec_hdr,buff,i);
			
		}
	}
	if(buff)
		free(buff);
}
int process_symtab(int32_t sect_num,FILE *fp)
{
	Elf32_Shdr sect_hdr;	
	Elf32_Sym mysym;
	char *name_buf=NULL;
	uint32_t num_sym,link,i,idx;
	off_t sym_data_offset;
	uint32_t sym_data_size;
	if(read_ELF_section_header(sect_num,&sect_hdr,fp)==-1)
	{
		return -1;
	}
	//we have to check to which strtab it is linked
	link=sect_hdr.sh_link;
	sym_data_offset=sect_hdr.sh_offset;
	sym_data_size=sect_hdr.sh_size;
	num_sym=sym_data_size/sizeof(Elf32_Sym);	
    
	//read the coresponding strtab
	if(read_ELF_section_header(link,&sect_hdr,fp)==-1)
	{
		return -1;
	}
	//get the size of strtab in file and allocate a buffer 
	name_buf=(char*)malloc(sect_hdr.sh_size);
	if(!name_buf)
		return -1;
	//get the offset of strtab in file and seek to it
	fseek(fp,sect_hdr.sh_offset,SEEK_SET);
	//read all data from the section to the buffer.
	fread(name_buf,sect_hdr.sh_size,1,fp);	
	//so we have the namebuf now seek to symtab data
	fseek(fp,sym_data_offset,SEEK_SET);
	printf(" %lx ",sym_data_offset);
	printf("[%d] symbols\n",num_sym);

	for(i=0;i<num_sym;i++)
	{
		//memcpy((char *)&mysym,(char *)sym_buf,sizeof(Elf32_Sym));
		fread(&mysym,sizeof(Elf32_Sym),1,fp);
	 
		idx=mysym.st_name;
		 	
		printf("[%3d] %s ",i,name_buf+idx); 
    	printf(" %lx ", mysym.st_value);
    	
		switch(ELF32_ST_TYPE(mysym.st_info))
		{
			case 0: printf(" NOTYPE ");
				break;
			case 1: printf(" OBJECT ");
				break;
			case 2: printf(" FUNC ");
				break;
			case 3: printf(" SECTION ");
				break;
			case 4: printf(" FILE ");
				break;
			case 5: printf(" COMMON ");
				break;
		}
	
		printf("\n");
				
	}
	if(name_buf)
		free(name_buf);
	
}
void dump_symbols(FILE *fp)
{
	Elf32_Ehdr ehdr;
	Elf32_Shdr sec_hdr;
	int num_hdrs,i;
	fseek(fp,(off_t)0,SEEK_SET);
	read_ELF_file_header(fp, &ehdr);
	num_hdrs=ehdr.e_shnum;
    printf("-->%lx\n",num_hdrs);
    
	for(i=0;i<num_hdrs;i++)
	{
		if(read_ELF_section_header(i,&sec_hdr,fp)==-1)
		{
			printf("Wrong Section to read\n");
		}
		else
		{
			if((sec_hdr.sh_type==SHT_SYMTAB)||(sec_hdr.sh_type==SHT_DYNSYM))			
			{
				printf("\n[section %3d] contains ",i);
				process_symtab(i,fp);
				
			}		
		}
	}
	
}



	
	
 
int main(int argc, char *argv[])
{
	FILE *elffile;
    uint8_t buff[512000];
    uint32_t i,lsize;
    char j=0;
    char k=0;
    uint32_t l=0;
    uint8_t *p;
    
	elffile=fopen(argv[1],"r");
	//elffile=fopen("rv32.elf","r");
	if(!elffile)exit(-1);
	fseek(elffile, 0, SEEK_END);
    lsize = ftell(elffile);
    rewind(elffile);
  	fread(buff,sizeof(buff),1, elffile);
  	
  	
	display_sections(elffile);
//	dump_symbols(elffile);
    printf("**************************************************\n");
     j=0;
     k=0;
     for(l=0;l<lsize;l++){//0x18a3
         if(!(j%4)){
            printf(" ");
         }
         if(!(k%16)){
            printf("\n");
         }
         printf("%02x", buff[l]);
         j++;
         k++;
     }
     printf("\n");
     printf("\n");
    printf("**************************************************\n");
    printf("text addr: %lx \n",text.addr);
    printf("text offset: %lx \n",text.offset);
    printf("text size: %lx \n",text.size);
    
	 text.data=&buff[text.offset];
     j=0;
     k=0;
     for(l=0;l<text.size;l++){//0x18a3
         if(!(j%4)){
            printf(" ");
         }
         if(!(k%16)){
            printf("\n");
         }
         printf("%02x", text.data[l]);
         j++;
         k++;
     }
     printf("\n");
     printf("\n");
	
    printf("**************************************************\n");
    printf("rodata addr: %lx \n",rodata.addr);
    printf("rodata offset: %lx \n",rodata.offset);
    printf("rodata size: %lx \n",rodata.size);
    rodata.data=&buff[rodata.offset];
    printf("%x\n",buff[rodata.offset]);
     j=0;
     k=0;
     for(l=0;l<rodata.size;l++){//0x18a3
         if(!(j%4)){
            printf(" ");
         }
         if(!(k%16)){
            printf("\n");
         }
         printf("%02x", rodata.data[l]);
         j++;
         k++;
     }
     printf("\n");
     printf("\n");
	
    printf("**************************************************\n");
    printf("data addr: %lx \n",data.addr);
    printf("data offset: %lx \n",data.offset);
    printf("data size: %lx \n",data.size);
	data.data=&buff[data.offset];
     j=0;
     k=0;
     for(l=0;l<data.size;l++){//0x18a3
         if(!(j%4)){
            printf(" ");
         }
         if(!(k%16)){
            printf("\n");
         }
         printf("%02x", data.data[l]);
         j++;
         k++;
     }
     printf("\n");
     printf("\n");
	
    printf("**************************************************\n");
	printf("sdata addr: %lx \n",sdata.addr);
    printf("sdata offset: %lx \n",sdata.offset);
    printf("sdata size: %lx \n",sdata.size);
    sdata.data=&buff[sdata.offset];
     j=0;
     k=0;
     for(l=0;l<sdata.size;l++){//0x18a3
         if(!(j%4)){
            printf(" ");
         }
         if(!(k%16)){
            printf("\n");
         }
         printf("%02x", sdata.data[l]);
         j++;
         k++;
     }
     printf("\n");
     printf("\n");
	
    printf("**************************************************\n");
    printf("bss addr: %lx \n",bss.addr);
    printf("bss offset: %lx \n",bss.offset);
    printf("bss size: %lx \n",bss.size);
    bss.data=&buff[bss.offset];
     j=0;
     k=0;
     for(l=0;l<bss.size;l++){//0x18a3
         if(!(j%4)){
            printf(" ");
         }
         if(!(k%16)){
            printf("\n");
         }
         printf("%02x", bss.data[l]);
         j++;
         k++;
     }
     printf("\n");
     printf("\n");
	
    printf("**************************************************\n");
	fclose(elffile);
	
    FILE *write_ptr;
    write_ptr = fopen("rom.bin","wb");  // w for write, b for binary
    fwrite(text.data,text.size,1,write_ptr);
	fclose(write_ptr);
	
	write_ptr = fopen("ram.bin","wb");  // w for write, b for binary
	
    fwrite(rodata.data,rodata.size,1,write_ptr);
    
    fwrite(data.data,data.size,1,write_ptr);
    
    fwrite(sdata.data,sdata.size,1,write_ptr);
    
    fwrite(bss.data,bss.size,1,write_ptr);
	fclose(write_ptr);
	//getch();
}
