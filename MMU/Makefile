objs := head.o init.o leds.o

mmu.bin : $(objs)
	arm-none-eabi-ld -Tmmu.lds -o mmu_elf $^
	arm-none-eabi-objcopy -O binary -S mmu_elf $@
	arm-none-eabi-objdump -D -m arm mmu_elf > mmu_elf.dis
	arm-none-eabi-objdump -D -m arm -b binary mmu.bin > mmu_bin.dis

%.o : %.c
	arm-none-eabi-gcc -Wall -O2 -c -o $@ $<

%.o : %.S
	arm-none-eabi-gcc -Wall -O2 -c -o $@ $<
	

clean:
	rm -f mmu.bin mmu_elf  *.dis *.o
