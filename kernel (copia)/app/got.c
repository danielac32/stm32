#include <stdio.h>
#include <stdint.h>

const uint8_t app[] __attribute__((aligned)) = {0x54,
0x49,0x4e,0x46,0x62,0x6c,0x69,0x6e,0x6b,0x2e,0x74,
0x69,0x6e,0x66,0x0,0x0,0x0,0x0,0x0,0x0,0x1,
0x0,0x21,0x0,0x1,0x0,0x0,0x0,0x5,0x0,0x0,
0x0,0x80,0xb5,0x0,0xaf,0x17,0x4b,0x59,0xf8,0x3,
0x30,0x1b,0x68,0x1b,0x78,0x64,0x2b,0x1,0xd1,0x13,
0x4b,0x22,0xe0,0x4f,0xf4,0x64,0x72,0xd,0x21,0x3,
0x20,0x12,0x4b,0x59,0xf8,0x3,0x30,0x98,0x47,0x1,
0x22,0xd,0x21,0x4,0x20,0xe,0x4b,0x59,0xf8,0x3,
0x30,0x98,0x47,0xc8,0x21,0x9,0x20,0xb,0x4b,0x59,
0xf8,0x3,0x30,0x98,0x47,0x0,0x22,0xd,0x21,0x4,
0x20,0x8,0x4b,0x59,0xf8,0x3,0x30,0x98,0x47,0x64,
0x21,0x9,0x20,0x5,0x4b,0x59,0xf8,0x3,0x30,0x98,
0x47,0xe4,0xe7,0x18,0x46,0x80,0xbd,0x3f,0x42,0xf,
0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xdf,0xc0,0x46,0x70,0x47,0x0,0x0,0x64,0x61,0x6e,
0x69,0x65,0x6c,0x20,0x71,0x75,0x69,0x6e,0x74,0x65,
0x72,0x6f,0x0,0x74,0x0,0x0,0x0,0x6c,0x0,0x0,
0x0,0x84,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,};




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

/**
* @brief    Load the app from flash to RAM and setup the FreeRTOS task
*
* @param    tinf_img pointer to the memory location where App starts
*
* @retval   APP_INVALID  if magic and DUMMY_SYS_ADDR check fails
* @retval   APP_OOM      if malloc fails
*/



#define DEFAULT_STACK_SIZE 1024


int8_t LoadApp(const uint8_t* tinf_img);


int main(int argc, char const *argv[])
{
	int res = LoadApp(app);
	printf("%d\n", res);
	return 0;
}




int8_t LoadApp(const uint8_t* tinf_img) {

    tinf_t* tinf = (tinf_t*)tinf_img;
    uint32_t app_stack[DEFAULT_STACK_SIZE];
    // Check if it is actually an APP
    // Check the magic, ie start of the header contains 'TINF'
    uint32_t* data_base = tinf->bin+tinf->text_size;

    if(memcmp(tinf->magic, "TINF", 4) == 0) {

        // Valid TINF Format app
        printf("Loading app: %s %hu.%hu\n", tinf->app_name, tinf->major_version, tinf->minor_version);
        printf("App text size: %hu 32 bit word\n", tinf->text_size);
        printf("App data size: %hu 32 bit word\n", tinf->data_size);
        printf("App bss size: %hu 32 bit word\n", tinf->bss_size);
        printf("App GOT entries: %ld\n", tinf->got_entries);

        // Allocate memory for data and bss section of the app on the heap
        uint32_t app_data_size = tinf->data_size+tinf->got_entries+tinf->bss_size;

        printf("Allocating app memory of %ld bytes\n", app_data_size*4);
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
        printf("App data base: 0x%08X\n", app_data_base);

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
                printf("Data at data section (flash): 0x%08X\n", *(uint32_t*)(tinf->bin+(tinf->text_size)));
                printf("Data at data section (RAM): 0x%08X\n", *(uint32_t*)(app_data_base));

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
                printf("GOT in app stack: %p\n", app_got_base);

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
                printf("Data at bss section (RAM): 0x%08X\n", *(uint32_t*)(app_data_base+(tinf->data_size)));
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
        printf("App entry point: 0x%08X\n", app_main);
        // TODO: The following line gives segfault for some reason
        //printf("Data at app entry point: 0x%08X", *((uint32_t*)(((uintptr_t)app_main)&0xFFFFFFFE)));

        //app_main(app_got_base);

         printf("got base 0x%08X\n", app_got_base);
        return 0;
    } else {
        // If check fails then it probably is not an app or it is corrupted
        // Invalid app
        return APP_INVALID;
    }
}

