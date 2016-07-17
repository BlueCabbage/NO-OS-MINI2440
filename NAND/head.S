.text
.global _start
_start:
	LDR		SP, =0X4096
	BL		disable_watch_dog
	BL		clock_init
	BL		memsetup
	BL		nand_c_init
	BL		copy_to_ram_from_nand
	LDR		SP, =0X34000000
	LDR		LR, =halt_loop
	LDR		PC, _start_armboot			@ CMD B and BL only can jump to the 32MB nearby here
										@ So use PC finish long jump

_start_armboot: 
	.word main

halt_loop:
	B		halt_loop

@
@/*
@ *  NAND FLASH init function
@ *  TACLS: TWRPH0 : TWRPHO : TWRPH1 = 1: 2: 1, BUSS_WIDTH_8
@ */
@#define S3C2440_NAND_BASE		0X4E000000
@#define NFCONF_OFFSET			0X0
@#define	NFCONT_OFFSET			0X4
@
@
@nand_asm_init:
@	LDR		R0, =S3C2440_NAND_BASE
@	LDR		R1, =0X001210
@	STR		R1, [R0, #NFCONF_OFFSET]
@	
@	MOV		R1, #0X3
@	STR		R1, [R0, #NFCONT_OFFSET]
@
@	MOV		PC, LR
@


