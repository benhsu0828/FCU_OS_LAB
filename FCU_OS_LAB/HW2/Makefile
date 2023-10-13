obj-m += hellok.o
 
KDIR = /usr/src/linux-headers-6.2.0-32-generic
 
 
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
 
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean