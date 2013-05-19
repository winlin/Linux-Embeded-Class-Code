#! /bin/sh

module="firstchardevdriver"
device="firstchardevdriver"

/sbin/insmod ./$module.ko

major=$(awk "\$2==\"$module\" {print \$1}" /proc/devices)

rm -rf /dev/${device}
mknod /dev/${device} c $major 0

