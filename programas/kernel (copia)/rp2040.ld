
MEMORY
{
  flash (rx) : ORIGIN = 0x10000000, LENGTH = 1044480 
  ram  (rwx) : ORIGIN = 0x20000000, LENGTH = 264k  
  /*SCRATCH_X(rwx) : ORIGIN = 0x20040000, LENGTH = 4k
  SCRATCH_Y(rwx) : ORIGIN = 0x20041000, LENGTH = 4k*/
}

PROVIDE ( _EEPROM_start = 270528512 );
PROVIDE ( _FS_start     = 269479936 );
PROVIDE ( _FS_end       = 270528512 );

__top_flash = ORIGIN(flash) + LENGTH(flash);
__top_ram = ORIGIN(ram) + LENGTH(ram);
_min_stack_sz = 256;
PROVIDE(_stack_limit = __top_ram - _min_stack_sz);
PROVIDE(_stack_top = __top_ram - 0);


ENTRY(irq_handler_reset)

SECTIONS
{ 

  .text : ALIGN(4)
  {
    _text = .;
    FILL(0xff)
    KEEP(*(.boot))
    . = ALIGN(256);
    KEEP(*(.vectors))
    *(.text*)
    *(.rodata)
    *(.rodata.*)
    . = ALIGN(4);
     
  } > flash
  
 


  . = ALIGN(4);
  _etext = .;

   
  .uninit_RESERVED : ALIGN(4)
  {
    KEEP(*(.bss.$RESERVED*))
  } > ram

  .data : ALIGN(4)
  {
    FILL(0xff)
    _data = .;
    *(.ramfunc .ramfunc.*);
    *(vtable)
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > ram AT > flash

  /* .scratch_x : {
        __scratch_x_start__ = .;
        *(.scratch_x.*)
        . = ALIGN(4);
        __scratch_x_end__ = .;
    } > SCRATCH_X AT > flash
    __scratch_x_source__ = LOADADDR(.scratch_x);

    .scratch_y : {
        __scratch_y_start__ = .;
        *(.scratch_y.*)
        . = ALIGN(4);
        __scratch_y_end__ = .;
    } > SCRATCH_Y AT > flash
    __scratch_y_source__ = LOADADDR(.scratch_y);*/

  .bss : ALIGN(4)
  {
    _bss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    PROVIDE(_end = .);
  } > ram

  PROVIDE(_kernel_end = .);

  ._stack : {
        . += _min_stack_sz;
  } >ram
}
