#define UINTM0  0x7f005038
#define UINTSP0  0x7f005034
#define UINTP0  0x7f005030

#define VIC1ENABLE  0x71300010

void enable_irq(void)
{
	volatile unsigned int* p=(unsigned int*)UINTM0;
	*p=0xe;
	p=(unsigned int*)UINTSP0;
	*p=0xf;
	p=(unsigned int*)UINTP0;
	*p=0xf;
	
	p=(unsigned int*)VIC1ENABLE;
	*p=(1<<5);

	/* clear  CPSR[7] */
	clear_ibit();
	return;
}

void irq_test(void)
{
	volatile unsigned int* p;
	p=(unsigned int*)UINTSP0;
	*p=1;
	p=(unsigned int*)UINTP0;
	*p=1;
	//unsigned char input=get_c();
	//put_c(input);
	//put_c('\n');
	return;
}
