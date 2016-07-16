button_c.bin : crt0.S button.c
	arm-none-eabi-gcc -g -c -o crt0.o crt0.S
	arm-none-eabi-gcc -g -c -o button_c.o button.c
	arm-none-eabi-ld -Ttext 0x00000000 -g crt0.o button_c.o -o button_c_elf
	arm-none-eabi-objcopy -O binary -S button_c_elf button_c.bin
	arm-none-eabi-objdump -D -m arm button_c_elf > button_c_elf.dis
	arm-none-eabi-objdump -D -m arm -b binary button_c.bin > button_c_bin.dis

clean:
	rm -f  button_c.bin  led_on_c_elf *.dis *.o
