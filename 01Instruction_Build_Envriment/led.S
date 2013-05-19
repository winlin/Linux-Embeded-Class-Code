.global start
.global delay
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

	mov r0,#10
	bl running

	b .

delay:
	cmp r0,#0
	subne r0,r0,#1
	bne delay
	mov pc,lr
