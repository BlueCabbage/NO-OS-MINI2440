objs := head.o init.o interrupt.o main.o

int.bin : $(objs)
	arm-none-eabi-ld -Ttext 0x00000000 -o int_elf $^
	arm-none-eabi-objcopy -O binary -S int_elf $@
	arm-none-eabi-objdump -D -m arm int_elf > int_elf.dis
	arm-none-eabi-objdump -D -m arm -b binary $@ > int_bin.dis

%.o : %.c
	arm-none-eabi-gcc -Wall -O2 -c -o $@ $<

%.o : %.S
	arm-none-eabi-gcc -Wall -O2 -c -o $@ $<


clean:
	rm -f int.bin int_elf *.o *.dis 

