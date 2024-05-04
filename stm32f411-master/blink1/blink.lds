


MEMORY
{
	RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 64K
    FLASH (rx)      : ORIGIN = 0x8000000, LENGTH = 256K
} 


SECTIONS
{
	/* First we define the .text section in the final program */
	.text : 
	{
	
		*(.vectors)
		*(.text)
		

		_etext = .;	
		
	} > FLASH	/* Link this code into flash */

	.data : AT (_etext)
	{
		_sdata = .;
		*(.data)
		_edata = .;
	} > RAM
	
 
	.bss :
	{
		_sbss = .;
		*(.bss)
		_ebss = .;
	} > RAM
}