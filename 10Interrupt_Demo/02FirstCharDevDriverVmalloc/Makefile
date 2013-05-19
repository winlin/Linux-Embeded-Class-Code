MODNAME = firstchardevdriver
CROSS_COMPILE=arm-linux-
TESTPRE=test
ifeq ($(PLATFORM),PC)
KERNELDIR ?= /lib/modules/$(shell uname -r)/build
else
CC = $(CROSS_COMPILE)gcc 
KERNELDIR ?= /opt/real6410/kernel/linux-2.6.28.6
endif

obj-m := $(MODNAME).o

default: test driver
test:
	@echo "Compiling test program..."
	$(CC) $(TESTPRE)$(MODNAME).c -o $(TESTPRE)$(MODNAME)
	@echo
driver:
	@echo "Compiling device driver..."
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	@echo
clean:
	@echo "Clean test program..."
	@rm -rf $(TESTPRE)$(MODNAME)
	@echo
	@echo "Clean device driver..."
	@rm -rf *.o *.ko *.mod.* .$(MODNAME).* .tmp* Module* module*
	@echo
