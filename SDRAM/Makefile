sdram.bin : head.S button.c
	arm-none-eabi-gcc -c -o head.o head.S
	arm-none-eabi-gcc -c -o button_c.o button.c
	arm-none-eabi-ld -Ttext 0x30000000 head.o button_c.o -o sdram_elf
	arm-none-eabi-objcopy -O binary -S sdram_elf sdram.bin
	arm-none-eabi-objdump -D -m arm sdram_elf > sdram_elf.dis
	arm-none-eabi-objdump -D -m arm -b binary sdram.bin > sdram_bin.dis

clean:
	rm -f *.dis sdram_elf sdram.bin *.o
