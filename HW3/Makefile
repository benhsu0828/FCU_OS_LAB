obj-m += D1053020_1.o
 
KDIR = /usr/src/linux-headers-4.15.0-112-generic
 
 
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
 
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean