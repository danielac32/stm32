__heap_size = 0x1000;
__stack_size = 0x1000;

ENTRY(_start)

SECTIONS
{
	. = 0x80000000;
	.text : ALIGN(16) {
		__TEXT_BEGIN__ = .;
	
 
		*(.text)

		__TEXT_END__ = .;
	}

	

	.data : ALIGN(16) {
		__DATA_BEGIN__ = .;
		*(.rodata)
		*(.rodata.*)
		*(.gnu.linkonce.r.*)
		*(.rodata1)
		*(.dynsbss)
		*(.gnu.linkonce.sb.*)
		*(.scommon)
		*(.gnu.linkonce.sb2.*)
		*(.sbss)
		*(.sbss.*)
		*(.sbss2)
		*(.sbss2.*)
		*(.dynbss)
		*(.data)
		*(.data.*)
		*(.got)
		*(.got.*)
		__DATA_END__ = .;
	}

	.bss : ALIGN( 16 ) {
		__BSS_BEGIN__ = .;
		*(.bss) /* Tricky: BSS needs to be allocated but not sent. GCC Will not populate these for calculating data size */
		*(.bss.*)
		__BSS_END__ = .;
	}
    PROVIDE( __global_pointer$ = . + 0x800 );


    
	.heap : ALIGN( 16 ) {
		_sheap = .;
		. = . + __heap_size;
		_eheap = .;
	}

	.stack : ALIGN( 16 ) {
		_estack = .;
		. = . + __stack_size;
		_sstack = .;
	}

	PROVIDE( SYSCON = 0x11100000 );
	PROVIDE( TIMERL = 0x1100bff8 );
	PROVIDE( TIMERH = 0x1100bffc );
}


