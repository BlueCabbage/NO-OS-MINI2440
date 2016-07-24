objs := head.o init.o serial.o main.o

uart.bin : $(objs)
	arm-none-eabi-ld 		-Tuart.lds 	-o uart_elf $^
	arm-none-eabi-objcopy 	-O binary -S uart_elf 	$@
	arm-none-eabi-objdump  	-D -m arm uart_elf > uart_elf.dis
	arm-none-eabi-objdump 	-D -m arm -b binary $@ > uart_bin.dis

%.o : %.c
	arm-none-eabi-gcc 	-Wall -g -O2 -c -o $@ $<

%.o : %.S
	arm-none-eabi-gcc 	-Wall -g -O2 -c -o $@ $<


clean:
	rm -f uart.bin uart_elf *.dis *.o
