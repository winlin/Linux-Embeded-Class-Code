#!/bin/bash
arm-linux-gcc -c led.S
arm-linux-gcc -nostartfiles -nostdlib -T led.lds led.o
arm-linux-objcopy -O binary a.out led.bin
cp led.bin /tftpboot/
rm *.o