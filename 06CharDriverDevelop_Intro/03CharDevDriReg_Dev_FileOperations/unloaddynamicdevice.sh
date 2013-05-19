#! /bin/bash

module="firstchardevdriver"
device="firstchardevdriver"

/sbin/rmmod $module
rm -f /dev/${device}
