objs := head.o init.o interrupt.o main.o

timer.bin: $(objs)
	arm-none-eabi-ld -Ttimer.lds -o timer_elf $^
	arm-none-eabi-objcopy -O binary -S timer_elf $@
	arm-none-eabi-objdump -D -m arm timer_elf > timer_elf.dis
	arm-none-eabi-objdump -D -m arm -b binary timer.bin > timer_bin.dis
	
%.o:%.c
	arm-none-eabi-gcc -Wall -O2 -c -o $@ $<

%.o:%.S
	arm-none-eabi-gcc -Wall -O2 -c -o $@ $<

clean:
	rm -f timer.bin timer_elf timer.dis *.o		

