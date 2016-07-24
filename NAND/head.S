@ Set SDRAM, copy the code to SDRAM, and then run in SDRAM
.text
.global _start
.extern main
_start:									
										@ function disable_watch_dog, memsetup, init_nand, nand_read_ll define in init.c
	LDR		SP, =0X4096
	BL		disable_watch_dog
	BL		memsetup
@	BL		nand_init
										@ Copy code(compiled from main.c) from Nand flash memory at 4096, length 1024 to SDRAM
	LDR		R0, =0X30000000				@ Destination addr 0x30000000(SDRAM addr)
	MOV		R1, #4096					@ Src addr 4096
	MOV		R2, #1024					@ Src length 1024 Bytes


	LDR		SP, =0X34000000
	LDR		LR, =halt_loop				@ Return addr
	LDR		PC, =main					@ Command B and BL only have +-32MB Range form current addr to jump

	BL		nand_read

	LDR		SP, =0X34000000
	LDR		LR, =halt_loop				@ Return addr
	LDR		PC, =main					@ Command B and BL only have +-32MB Range form current addr to jump

halt_loop:
	B		halt_loop
