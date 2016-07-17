objs := head.o init.o nand.o leds.o

all : nand.bin

nand.bin : $(objs)
	arm-none-eabi-ld -Tnand.lds -o nand_elf $^
	arm-none-eabi-objcopy -O binary -S nand_elf $@
	arm-none-eabi-objdump -D -m arm nand_elf > nand_elf.dis
	arm-none-eabi-objdump -D -m arm -b binary nand.bin > nand_bin.dis

%.o : %.c
	arm-none-eabi-gcc -Wall -c -O2 -o $@ $<

%.o : %.S
	arm-none-eabi-gcc -Wall -g -c -O2 -o $@ $<


clean:
	rm -f *.dis *.o nand.bin nand_elf

