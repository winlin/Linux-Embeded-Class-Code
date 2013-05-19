ENTRY (start)
SECTIONS
{
	. = 0xc0008000;
	.text :{
		start.o(.text)
		*(.text)
	}
	.data :{
		*(.data)
	}
	.bss :{
		*(.bss)
	}
}
