obj-m += ecc-test.o 

KERNEL_SRC ?= /usr/src/linux-headers-`uname -r`

ecc-test-y  := ecc.o

MY_CFLAGS += -g -DDEBUG
ccflags-y += ${MY_CFLAGS}

SRC := $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC)

modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules_install

clean:
	rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c *.mod .*.o.d tests/*.o tests/*~ tests/core tests/.depend tests/.*.cmd tests/*.mod.c tests/*.mod tests/.*.o.d
	rm -f Module.markers Module.symvers modules.order
	rm -rf .tmp_versions Modules.symvers
