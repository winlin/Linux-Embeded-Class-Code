.global uart_init
#define GPA_BASE	0x7f008000
#define GPACON_OFFSET	0x0
#define GPAPUD_OFFSET	0X8
#define UART0_BASE	0X7F005000
#define ULCON0_OFFSET	0x0
#define UCON0_OFFSET	0x4
#define UFCON0_OFFSET   0x8	

uart_init:
	ldr r0,=GPA_BASE
	mov r1,#0x22
	str r1,[r0,#GPACON_OFFSET]
	mov r1,#0
	str r1,[r0,#GPAPUD_OFFSET]

	ldr r0,=UART0_BASE
	mov r1,#0x3
	str r1,[r0,#ULCON0_OFFSET]

	ldr r1,[r0,#UCON0_OFFSET]
	bic r1,r1,#0xf
	orr r1,r1,#0x5
	str r1,[r0,#UCON0_OFFSET]

	mov r1,#0x0
	str r1,[r0,#UFCON0_OFFSET]

	mov pc,lr
	
	
	
	
	