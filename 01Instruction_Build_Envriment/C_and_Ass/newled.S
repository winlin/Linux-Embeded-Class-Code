.global start
.equ GPN_BASE,0x7f008830

#define CON_OFFSET 0x0
#define DAT_OFFSET 0x4
#define PUD_OFFSET 0x8

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

	mov r3,#0xff0000
loop1:
	cmp r3,#0
	sub r3,r3,#1
	bne loop1

	ldr r2,[r0,#DAT_OFFSET]
	orr r2,r2,r1,lsl #6
	str r2,[r0,#DAT_OFFSET]

	mov r3,#0xff0000
loop2:
	cmp r3,#0
	sub r3,r3,#1
	bne loop2

	b running
