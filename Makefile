obj-m +=input.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	$(CC) user.c -o test.o
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm test.o
