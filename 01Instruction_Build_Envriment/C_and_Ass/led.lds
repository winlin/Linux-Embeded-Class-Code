ENTRY(start)

SECTIONS
{
	. = 0xc0008000;
	.text :{
		led.o(.text)
		*(.text)
	}
	.data :{
		*(.data)
	}
	.bss :{
		*(.bss)
	}
}
