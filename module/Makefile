# `make -C /lib/modules/$(uname -r)/build M=$PWD modules` - 效果等同

obj-m += append_hello_mod.o

KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(CURDIR) modules

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) clean
