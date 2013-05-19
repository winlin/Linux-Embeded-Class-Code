#! /bin/bash

module="firstchardevdriver"
device="firstchardevdriver"

/sbin/insmod /lib/modules/2.6.28.6/kernel/drivers/$module.ko

major=$(awk "\$2==\"$module\" {print \$1}" /proc/devices)

rm -rf /dev/${device}
mknod /dev/${device} c $major 0