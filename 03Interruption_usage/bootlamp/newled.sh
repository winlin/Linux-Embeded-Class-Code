#!/bin/bash
arm-linux-gcc -c newled.S
arm-linux-gcc -nostartfiles -nostdlib -T newled.lds newled.o
arm-linux-objcopy -O binary a.out newled.bin
cp newled.bin /tftpboot/
rm *.o