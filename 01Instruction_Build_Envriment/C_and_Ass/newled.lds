ENTRY(start)

SECTIONS
{
	. = 0xc0008000;
	.text :{
		newled.o(.text)
		*(.text)
	}
	.data :{
		*(.data)
	}
	.bss :{
		*(.bss)
	}
}
