.global start
#define	SVC_SP		0X2000   /* 8KB,ARM is full-sub-stack */
#define	SVC_SP_SIZE	128
#define IRQ_SP 		(SVC_SP-SVC_SP_SIZE)
#define IRQ_SP_SIZE	512
#define SYS_SP		(IRQ_SP-IRQ_SP_SIZE)
	
start:
	/* mode initlize */
	/* svc mode initlize*/
	msr cpsr_c, #0xd3
	mov sp, #SVC_SP
	/* irq mode initlize */
	msr cpsr_c, #0xd2
	mov sp, #IRQ_SP
	/* sys mode initlize */
	msr cpsr_c, #0xdf
	mov sp, #SYS_SP
	
	/* device memory address mapping */
	/* start 0x70000000 size is 0x13 means*/
	ldr r0, =0x70000013
	mcr p15, 0, r0, c15 ,c2 ,4

	/* stop the watchdog */
	ldr r0, =0x7e004000
	mov r1, #0
	str r1,[r0]

	/* set the clock */
	ldr r0, =0x7e00f900
	ldr r1, [r0]
	orr r1, r1, #0x40    /* DOUT_apll:1 */
	str r1,[r0]

	ldr r1, [r0]
	orr r1, r1, #0x80
	str r1, [r0]
	/* check mode chang sucess */
wait:
	ldr r1, [r0]
	and r1, r1, #0xf00
	cmp r1, #0
	beq wait
	
	/* clear MPLL_SEL and APLL_SEL to */
	/* makesure change the APLL and MPLL */
	ldr r0, =0x7e00f01c    /* CLK_SRC P145 */
	ldr r1, [r0]
	bic r1, r1, #0x7
	str r1, [r0]

	ldr r0, =0x7e00f020    /* CLK_DIV0 P147 */
	ldr r1, [r0]
	bic r1, r1, #0xff00
	bic r1, r1, #0xff
	orr r1, r1, #0x3300
	str r1, [r0]
	
        /* set APLL_LOCK/MPLL_LOCK/EPALL_LOCK,*/
	/* as the default value is the max value, */
	/* so wo can ignore this step P141/P144*/

	/* set APLL_CON and others P141*/
	ldr r0, =0x7e00f00c
	ldr r1, =((1<<31)|(266<<16)|(3<<8)|(1<<0))
	str r1, [r0]

	/* set MPLL_SEL and APLL_SEL */
	ldr r0, =0x7e00f01c    /* CLK_SRC P145 */
	ldr r1, [r0]
	orr r1, r1, #0x7
	str r1, [r0]
	
	/* clean bss */
	adr r0, _bss_start
	adr r1, _bss_end
	ldr r2, [r0]
	ldr r3, [r1]
	mov r0, #0
loop:
	cmp r2,r3
	strne r0, [r2],#4
	bne loop

	
	bl  uart_init
	bl  dm9000_init
	b   mystart

_bss_start:	.word bss_start
_bss_end:	.word bss_end