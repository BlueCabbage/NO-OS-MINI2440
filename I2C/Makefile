CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-ld
AR      = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

INCLUDEDIR 	:= $(shell pwd)/include
CFLAGS 		:= -Wall -O2
CPPFLAGS   	:= -nostdinc -I$(INCLUDEDIR)

export 	CC LD OBJCOPY OBJDUMP INCLUDEDIR CFLAGS CPPFLAGS

objs := head.o init.o nand.o interrupt.o i2c.o m41t11.o serial.o main.o lib/libc.a

i2c.bin: $(objs)
	${LD} -Ti2c.lds -o i2c_elf $^
	${OBJCOPY} -O binary -S i2c_elf $@
	${OBJDUMP} -D -m arm i2c_elf > i2c.dis

.PHONY : lib/libc.a
lib/libc.a:
	cd lib; make; cd ..
	
%.o:%.c
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

%.o:%.S
	${CC} $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	make  clean -C lib
	rm -f i2c.bin i2c_elf i2c.dis *.o

