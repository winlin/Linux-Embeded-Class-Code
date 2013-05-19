MODULENAME = leddm
TESTPRE	= test

CC = arm-linux-gcc
KERNELDIR ?= /opt/real6410/kernel/linux-2.6.28.6

ifeq ($(DEBUG), no)
DEBUG_FLAGS = -O2
else
DEBUG_FLAGS = -O -g -DLEDDMDBG
endif

EXTRA_CFLAGS += $(DEBUG_FLAGS)

obj-m := $(MODULENAME).o

all: test driver

test:
	@echo "Compiling test program..."
	$(CC) $(TESTPRE)$(MODULENAME).c -o $(TESTPRE)$(MODULENAME)
	@echo

driver:
	@echo "Compiling device driver..."
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	@echo

clean:
	@rm -f $(TESTPRE)$(MODULENAME) a.out
	@rm -rf *.o *.ko *.mod.* .$(MODULENAME)* .tmp* modul* Modul*

