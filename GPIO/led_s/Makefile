led_on.bin : led_on.S
	arm-none-eabi-gcc -g -c -o led_on.o led_on.S
	arm-none-eabi-ld  -Ttext 0x00000000 -g led_on.o -o led_on_elf
	arm-none-eabi-objcopy -O binary -S led_on_elf led_on.bin
	arm-none-eabi-objdump -D -b binary -m arm led_on.bin > dis_led_on_bin
	arm-none-eabi-objdump -D led_on_elf > dis_led_on_elf

clean:
	rm -f led_on.bin led_on_elf *.o

