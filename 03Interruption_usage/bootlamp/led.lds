ENTRY(start)

SECTIONS
{
	. = 0x0;
	.text :{
		led.o(.text)
		*(.text)
	}
	.data :{
		*(.data)
	}
	bss_start = .;
	.bss :{
		*(.bss)
	}
	bss_end = .;
}
