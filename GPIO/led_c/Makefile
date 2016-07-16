led_on_c.bin : crt0.S led_on.c
	arm-none-eabi-gcc -g -c -o crt0.o crt0.S
	arm-none-eabi-gcc -g -c -o led_on_c.o led_on.c
	arm-none-eabi-ld -Ttext 0x00000000 -g crt0.o led_on_c.o -o led_on_c_elf
	arm-none-eabi-objcopy -O binary -S led_on_c_elf led_on_c.bin
	arm-none-eabi-objdump -D -m arm led_on_c_elf > led_on_c_elf.dis
	arm-none-eabi-objdump -D -m arm -b binary led_on_c.bin > led_on_c_bin.dis

clean:
	rm -f  led_on_c.bin  led_on_c_elf *.dis *.o
