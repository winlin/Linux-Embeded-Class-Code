MODNAME = kernelspacekmalloc

ifeq ($(PLATFORM), PC)
KERNELDIR ?= /lib/modules/$(shell uname -r)/build
else
KERNELDIR ?= /opt/real6410/kernel/linux-2.6.28.6
endif

obj-m := $(MODNAME).o

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	@rm -rf *.o *.ko *.mod.* .$(MODNAME).* .tmp* Module* module*

