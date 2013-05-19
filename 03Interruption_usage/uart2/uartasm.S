.global uart_init
#define GPA_BASE	0x7f008000
#define GPACON_OFFSET	0x0
#define GPAPUD_OFFSET	0X8
#define UART0_BASE	0X7F005000
#define ULCON0_OFFSET	0x0
#define UCON0_OFFSET	0x4
#define UFCON0_OFFSET   0x8
#define UBRDIV0		0X7f005028
#define UDIVLOST0	0X7f00502c

uart_init:
	/* inilize GPA*/
	ldr r0,=GPA_BASE
	mov r1,#0x22
	str r1,[r0,#GPACON_OFFSET]
	mov r1,#0
	str r1,[r0,#GPAPUD_OFFSET]

	/* choose UART0 */
	ldr r0,=UART0_BASE
	mov r1,#0x3
	str r1,[r0,#ULCON0_OFFSET]
	/* set UCON0 */
	ldr r1,[r0,#UCON0_OFFSET]
	bic r1,r1,#0xf
	orr r1,r1,#0x5
	orr r1,r1,#0x800       /* PCLK setted */
	str r1,[r0,#UCON0_OFFSET]

	mov r1,#0x0
	str r1,[r0,#UFCON0_OFFSET]

	/* set DIV_VAL */
	ldr r0, =UBRDIV0
	mov r1, #0x23
	str r1, [r0]
	/* set  DIV_SOLT */
	ldr r0, =UDIVLOST0
	mov r1, #0x0080
	str r1,[r0]

	mov pc,lr
	
	
	
	
	