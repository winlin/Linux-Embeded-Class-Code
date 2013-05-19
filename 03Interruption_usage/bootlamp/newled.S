.global start
.equ GPN_BASE,0x7f008830

#define CON_OFFSET 0x0
#define DAT_OFFSET 0x4
#define PUD_OFFSET 0x8
#define	SVC_SP		0X2000   /* 8KB,ARM is full-sub-stack */
#define	SVC_SP_SIZE	128
#define IRQ_SP 		(SVC_SP-SVC_SP_SIZE)
#define IRQ_SP_SIZE	512
#define SYS_SP		(IRQ_SP-IRQ_SP_SIZE)
#define SYS_SP_SIZE	
initlize:
	/* 1.mode initlize */
	/* svc mode initlize*/
	msr cpsr_c, #0xd3
	mov sp, #SVC_SP
	/* irq mode initlize */
	msr cpsr_c, #0xd2
	mov sp, #IRQ_SP
	/* sys mode initlize */
	msr cpsr_c, #0xdf
	mov sp, #SYS_SP

	/* 2.clean bss */
	adr r0, _bss_start
	adr r1, _bss_end
	ldr r2, [r0]
	ldr r3, [r1]

	mov r0, #0
loop:
	cmp r2,r3
	strne r0, [r2],#4
	bne loop

	/* 3.device memory address mapping */
	/* start 0x70000000 size is 0x13 means*/
	ldr r0, =0x70000013
	mcr p15, 0, r0, c15 ,c2 ,4

	/* 4.stop the watchdog */
	ldr r0, =0x7e004000
	mov r1, #0
	str r1,[r0]
	
start:
	ldr r0,=GPN_BASE
	ldr r2,[r0,#CON_OFFSET]
	mov r1,#1
	orr r2,r2,r1,lsl #12
	bic r2,r2,r1,lsl #13
	str r2,[r0,#CON_OFFSET]

	mov r2,#0	
	str r2,[r0,#PUD_OFFSET]

running:
	ldr r2,[r0,#DAT_OFFSET]
	bic r2,r2,r1,lsl #6
	str r2,[r0,#DAT_OFFSET]           

	mov r3,#0xff00
loop1:
	cmp r3,#0
	sub r3,r3,#1
	bne loop1

	ldr r2,[r0,#DAT_OFFSET]
	orr r2,r2,r1,lsl #6
	str r2,[r0,#DAT_OFFSET]

	mov r3,#0xff00
loop2:
	cmp r3,#0
	sub r3,r3,#1
	bne loop2

	b running

	
_bss_start:	.word bss_start
_bss_end:	.word bss_end