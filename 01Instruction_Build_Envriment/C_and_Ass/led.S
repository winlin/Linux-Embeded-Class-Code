.global start

start:
	ldr r0,=0x7f008830
	mov r1,#1
	mov r2,r1,lsl #12
	str r2,[r0]

	mov r2,#0
	str r2,[r0,#8]

running:
	mov r2,#0                
	str r2,[r0,#4]           

	mov r3,#0xff0000
loop1:
	cmp r3,#0
	sub r3,r3,#1
	bne loop1

	mov r2,r1,lsl #6
	str r2,[r0,#4]

	mov r3,#0xff0000
loop2:
	cmp r3,#0
	sub r3,r3,#1
	bne loop2

	b running
