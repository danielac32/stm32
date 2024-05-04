ENTRY(Reset_Handler)
__stack_size = 4096;

MEMORY
{
  ROM    (rx): ORIGIN = 0x08000000, LENGTH = 512K
  RAM   (rwx): ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
    .text :
    {
        __text_start = .;
        KEEP(*(.isr_vector))
        *(.text*)
        KEEP(*(.init))  KEEP(*(.fini))
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o).dtors)
        *(SORT(.dtors.*))
        *(.dtors)
        *(.rodata*)
        KEEP(*(.eh_frame*))
    } > ROM
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > ROM
    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > ROM
    __exidx_end = .;
    __etext = .;
    .data : AT (__etext)
    {
        __data_start__ = .;
        *(vtable)
        *(.data*)
        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);
        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);
        . = ALIGN(4);
        __data_end__ = .;
    } > RAM
    .bss (NOLOAD) :
    {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
    } > RAM

    .stack : ALIGN( 16 ) {
        _stack = .;
        . = . + __stack_size;
        _sstack = .;
        . = ALIGN(16);
    }>RAM


    end = .;
    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    PROVIDE(_estack = _sstack);
    PROVIDE(_sidata = __etext);
    PROVIDE(_sdata = __data_start__);
    PROVIDE(_edata = __data_end__);
    PROVIDE(_sbss = __bss_start__);
    PROVIDE(_ebss = __bss_end__);

    PROVIDE(text = __text_start);
    PROVIDE(etext = __exidx_start);
    PROVIDE(data = __data_start__);
    PROVIDE(edata = __data_end__);
    PROVIDE(bss = __bss_start__);
    PROVIDE(ebss = __bss_end__);
}

