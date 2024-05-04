

#include <xinu.h>
#include <string.h>
#include <stdlib.h>
#include <elf.h>
extern	void	start(void);	/* Start of Xinu code			*/
extern	void	*_end;		/* End of Xinu code			*/
extern	void meminit(void);
 
struct	memblk	memlist;





const uint8_t app[] __attribute__((aligned)) = {0x54,
0x49,0x4e,0x46,0x62,0x6c,0x69,0x6e,0x6b,0x2e,0x74,
0x69,0x6e,0x66,0x0,0x0,0x0,0x0,0x0,0x0,0x1,
0x0,0x2b,0x0,0x2,0x0,0x0,0x0,0x5,0x0,0x0,
0x0,0x81,0x46,0x0,0x0,0x80,0xb5,0x0,0xaf,0x16,
0x4b,0x59,0xf8,0x3,0x30,0x1b,0x68,0x1b,0x78,0x64,
0x2b,0x22,0xd1,0x4f,0xf4,0x64,0x72,0xd,0x21,0x3,
0x20,0x12,0x4b,0x59,0xf8,0x3,0x30,0x98,0x47,0x1,
0x22,0xd,0x21,0x4,0x20,0xe,0x4b,0x59,0xf8,0x3,
0x30,0x98,0x47,0xc8,0x21,0x9,0x20,0xb,0x4b,0x59,
0xf8,0x3,0x30,0x98,0x47,0x0,0x22,0xd,0x21,0x4,
0x20,0x8,0x4b,0x59,0xf8,0x3,0x30,0x98,0x47,0x64,
0x21,0x9,0x20,0x5,0x4b,0x59,0xf8,0x3,0x30,0x98,
0x47,0xe4,0xe7,0x0,0x23,0x18,0x46,0x80,0xbd,0x0,
0xbf,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xef,
0xf3,0x3,0x80,0x72,0xb6,0x70,0x47,0x80,0xf3,0x3,
0x88,0x62,0xb6,0x70,0x47,0x62,0xb6,0x70,0x47,0x72,
0xb6,0xfe,0xe7,0x0,0xdf,0xc0,0x46,0x70,0x47,0x64,
0xdf,0x70,0x47,0x0,0x0,0x64,0x61,0x6e,0x69,0x65,
0x6c,0x20,0x71,0x75,0x69,0x6e,0x74,0x65,0x72,0x6f,
0x0,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,
0x31,0x31,0x0,0x90,0x0,0x0,0x0,0xa0,0x0,0x0,
0x0,0x84,0x0,0x0,0x0,0x0,0x0,0x0,0x10,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,};





typedef struct {
    /** Should be 'TINF'. This is checked while loading the app and APP_INVALID
     *  is returned if check fails.*/
    uint8_t magic[4];
    /** This is the name of app (max 15 chars and one \0 at the end).
     *  Will be passed to Task create function */
    uint8_t app_name[16];
    /** Major version of app. (max 255) */
    uint8_t major_version;
    /** Minor version of app. (max 255) */
    uint8_t minor_version;
    /** Size of the text section ie code section in words. max (65K words) */
    uint16_t text_size;
    /** Size of the data section in words. max (65K words) */
    uint16_t data_size;
    /** Size of the bss section in words. max (65K words) */
    uint16_t bss_size;
    /** Number of GOT table entries */
    uint32_t got_entries;
    /** The binary data is appended here */
    uint32_t bin[];

}__attribute__((packed)) tinf_t;


/** Magic and DUMMY_SYS_ADDR check failed */
#define APP_INVALID -1
/** Unable to allocate memory */
#define APP_OOM -2

#define DEFAULT_STACK_SIZE 1024

int8_t LoadApp(const uint8_t* tinf_img);






void uartinit()
{
	struct	uart_csreg *uptr;	/* Address of UART's CSRs */
    init_sys();         /* Set MCU speed, enable preipherials, etc. */

    hw_cfg_pin(GPIOx(GPIO_A),     11,     GPIOCFG_MODE_ALT | 8 | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(GPIO_A),     12,     GPIOCFG_MODE_ALT | 8 | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

	/* Enable peripheral */
	uptr = (struct uart_csreg *)(0x40011400);
	uptr->cr1 &= ~(1 << UART_EN);
	
	/* Set baudrate 115200 */
	uptr->brr = UART_BAUDRATE(100 MHZ, 115200);
	
	uptr->cr1 |= (1 << UART_INTR_RX) | (1 << UART_TX_EN) | (1 << UART_RX_EN);
	uptr->cr1 |= (1 << UART_EN);
	
	
}
 
 
void SysTick_Handler(void){
	//*SCB_ICSR |= (1 << PENDSV_INTR);
}
 
 

/*
void write_sram(uint32 address, uint8 data_byte){
hw_set_pin(GPIOx(GPIO_A), 4, 0);
transfer(WRITE);
transfer((uint8)(address >> 16) & 0xff);
transfer((uint8)(address >> 8) & 0xff);
transfer((uint8)address);
transfer(data_byte);
hw_set_pin(GPIOx(GPIO_A), 4, 1);
}

uint8 read_sram(uint32 address){
uint8 read_byte;
hw_set_pin(GPIOx(GPIO_A), 4, 0);
transfer(READ);
transfer((uint8)(address >> 16) & 0xff);
transfer((uint8)(address >> 8) & 0xff);
transfer((uint8)address);
read_byte = transfer(0x00);
hw_set_pin(GPIOx(GPIO_A), 4, 1);
}

*/


FATFS FatFs;


void	nulluser()
{	
struct	memblk	*memptr;	/* Ptr to memory block		*/
uint32	free_mem;	 
 
	 
uartinit();
meminit();
free_mem = 0;
for (memptr = memlist.mnext; memptr != NULL;memptr = memptr->mnext) {
	free_mem += memptr->mlength;
}
kprintf("%10d bytes of free memory.  Free list:\n", free_mem);
for (memptr=memlist.mnext; memptr!=NULL;memptr = memptr->mnext) {
    kprintf("           [0x%08X to 0x%08X]\n",
	(uint32)memptr, ((uint32)memptr) + memptr->mlength - 1);
}

kprintf("%10d bytes of Xinu code.\n",
	(uint32)&etext - (uint32)&text);
kprintf("           [0x%08X to 0x%08X]\n",
	(uint32)&text, (uint32)&etext - 1);
kprintf("%10d bytes of data.\n",
	(uint32)&ebss - (uint32)&data);
kprintf("           [0x%08X to 0x%08X]\n\n",
	(uint32)&data, (uint32)&ebss - 1);



//enable();
//init_systick(50,1);

FRESULT res;
DIR dirs;
FILINFO Finfo;


if ((res = f_mount(&FatFs, "", 1)) != FR_OK) {
    kprintf("mount error! res=%d\n", res);
    return;//return 1;
}
kprintf("mount successfully\n");



char *p2 = malloc(100);
if ((res = f_opendir(&dirs,p2)) == FR_OK) {
     for (;;){
         res = f_readdir(&dirs, &Finfo);
         if(res != FR_OK || Finfo.fname[0] == 0)break;
         kprintf("%s\n", Finfo.fname);
     }
 }else{
    kprintf("error dir\n");
 }
free(p2);

 

int ress = LoadApp(app);
	kprintf("%d\n", ress);

for(;;){

}

}
 
 

 int8_t LoadApp(const uint8_t* tinf_img) {

    tinf_t* tinf = (tinf_t*)tinf_img;
    uint32_t app_stack[DEFAULT_STACK_SIZE];
    // Check if it is actually an APP
    // Check the magic, ie start of the header contains 'TINF'
    uint32_t* data_base = tinf->bin+tinf->text_size;

    if(memcmp(tinf->magic, "TINF", 4) == 0) {

        // Valid TINF Format app
        kprintf("Loading app: %s %d.%d\n", tinf->app_name, tinf->major_version, tinf->minor_version);
        kprintf("App text size: %d 32 bit word\n", tinf->text_size);
        kprintf("App data size: %d 32 bit word\n", tinf->data_size);
        kprintf("App bss size: %d 32 bit word\n", tinf->bss_size);
        kprintf("App GOT entries: %d\n", tinf->got_entries);

        // Allocate memory for data and bss section of the app on the heap
        uint32_t app_data_size = tinf->data_size+tinf->got_entries+tinf->bss_size;

        kprintf("Allocating app memory of %ld bytes\n", app_data_size*4);
        // TODO: Add the size of the stack actually required by the app, currently hardcoded to DEFAULT_STACK_SIZE words, change in the task create API also
        // TODO: Cannot dynamically allocate thread stack due to limitation in zephyr
        // Zephyr does not have an aligned allocater
        //uint32_t* app_data_base = k_malloc((app_data_size+DEFAULT_STACK_SIZE)*4);
        uint32_t* app_data_base = app_stack;
        uint32_t app_stack_size = DEFAULT_STACK_SIZE;
        //if(app_data_base == NULL) {
        //    return APP_OOM;
        //}

        // app_stack_base is the address of the base of the stack used by
        // the rtos task
        uint32_t* app_stack_base = app_data_base+app_data_size;

        // app_got_base is the value which will get loaded to r9
        // This is address of the base of GOT which will actually be used
        // by the app for global data accesses
        // This value is passed as the parameter to the app_main. The app will
        // copy the this value from r0 to r9 register
        uint32_t* app_got_base = app_data_base + tinf->data_size;
        kprintf("App data base: 0x%08X\n", app_data_base);

        // Layout in RAM:
        // low memory (eg: 0x200014a0)                          high memory (eg: 0x200014cc)
        // |<--------------------- app_data_size ---------------------->|
        // |<-- tinf->data_size -->|<-- got_entries -->|<-- bss_size -->|<-- stack_size -->|
        // +-----------------------+-------------------+----------------+------------------+
        // | .data                 | .got              | .bss           | APP STACK        |
        // +-----------------------+-------------------+----------------+------------------+
        // ^                       ^                                    ^                  Λ
        // app_data_base           app_got_base                         app_stack_base     |
        //                                                  stack starts pushing from here |
        // TODO: There is no APP STACK overflow protection right now since we cannot detect
        //       when the stack will start overwriting the bss section

        if(app_data_size != 0) {
            if(tinf->data_size > 0) {
                // Copy data section from flash to the RAM we allocated above
                memcpy(app_data_base, data_base, (tinf->data_size*4));
                kprintf("Data at data section (flash): 0x%08X\n", *(uint32_t*)(tinf->bin+(tinf->text_size)));
                kprintf("Data at data section (RAM): 0x%08X\n", *(uint32_t*)(app_data_base));

                // Replace the sys_struct address to the one on the mcu
                // The start of the app_data_base will point to start of the data section
                // This is where the sys_struct address was kept by the linker script
                //*app_data_base = (uintptr_t)&sys;
                //printf("App sys_struct: 0x%08X", *app_data_base);
            }

            // If there is any global data of the app then copy to RAM
            if(tinf->got_entries > 0) {
                // Copy the GOT from flash to RAM
                // app_got_base is the base of the GOT in the RAM
                // this is where GOT will be copied to
                kprintf("GOT in app stack: %p\n", app_got_base);

                // got_entries_base is the base of the GOT in the flash
                // tinf->got_entries number of entries from this location
                // needs to be copied into RAM
                uint32_t* got_entries_base = data_base + tinf->data_size;

                // While copying add the base address (app_data_base) in RAM to each element of the GOT
                // TODO: Add more explaination about this
                // Need to subtract the data_offset to get the location with respect to 0
                uint32_t data_offset = 0x10000000;
                for(uint8_t i = 0; i < tinf->got_entries; i++) {
                    if(*(got_entries_base+i) >= data_offset) {
                        // If the value is greater than data_offset then it is in the RAM section.
                        // So add the app_data_base to it.
                        *(app_got_base+i) = ((*(got_entries_base+i))-data_offset)+(uint32_t)((uintptr_t)app_data_base);
                    } else {
                        // else it is a relocation in the flash
                        // so add the flash->bin ie the base of the app in the flash to it
                        *(app_got_base+i) = (*(got_entries_base+i))+(uint32_t)((uintptr_t)tinf->bin);
                    }
                }
            }

            // If there is any bss in the app then set that much space to 0
            if(tinf->bss_size > 0) {
                // Set BSS section to 0
                memset(app_data_base+(tinf->data_size+tinf->got_entries), 0, tinf->bss_size*4);
                kprintf("Data at bss section (RAM): 0x%08X\n", *(uint32_t*)(app_data_base+(tinf->data_size)));
            }
            //uint32_t* app_stack_got = app_data_base + tinf->data_size;
            //uint32_t* got_entries_base = data_base + tinf->data_size;
            //for(uint8_t i = 0; i < tinf->got_entries; i++) {
            //printf("app_stack_got: 0x%08X: 0x%08X: 0x%08X and flash: 0x%08X", app_stack_got, *app_stack_got, *((uint32_t*)((uintptr_t)(*app_stack_got))), *got_entries_base++);
            //printf("app_stack_got: 0x%08X: 0x%08X and flash: 0x%08X", app_stack_got, *app_stack_got, *got_entries_base++);
            //app_stack_got++;
            //}
        }
        
        // OR'ed with 1 to set the thumb bit, TODO: Check if this is handled internally by zephyr
        int (*app_main)() = (void*)(((uintptr_t)(tinf->bin))|1);
        kprintf("App entry point: 0x%08X\n", app_main);
        // TODO: The following line gives segfault for some reason
        //printf("Data at app entry point: 0x%08X", *((uint32_t*)(((uintptr_t)app_main)&0xFFFFFFFE)));

        app_main(app_got_base);

         kprintf("got base 0x%08X\n", app_got_base);
        return 0;
    } else {
        // If check fails then it probably is not an app or it is corrupted
        // Invalid app
        return APP_INVALID;
    }
}

