#!/bin/bash
arm-linux-gcc -c protocol.h protocol.c start.S uart.c dm9000.c uartasm.S 
arm-linux-gcc -nostartfiles -nostdlib -T led.lds *.o
arm-linux-objcopy -O binary a.out a.bin
cp a.bin /tftpboot/

rm *.o
rm *.gch