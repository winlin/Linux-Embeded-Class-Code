.global start
start:
	bl uart_init
	b mystart
