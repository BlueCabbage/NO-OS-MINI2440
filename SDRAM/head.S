@ Set SDRAM, Copy the program to SDRAM, and then run from SDRAM

.equ	MEM_CTL_BASE,		0X48000000
.equ	SDRAM_BASE,			0X30000000

.text
.global	_start
_start:
	BL		disable_watch_dog				@ CLOSE WATCH DOG
	BL		memsetup
	BL		copy_steppingstone_to_sdram		@ Copy the code to SDRAM
	LDR		PC, =on_sdram

on_sdram:
	LDR		SP,	=0X34000000					@ Set stack
	BL		main 

halt_loop:
	B		halt_loop


disable_watch_dog:
	@ Write WATCHDOG Resiger 0
	
	MOV		R1, #0X53000000
	MOV		R2,	#0X0
	STR		R2, [R1]
	MOV		PC, lr							@ Return

copy_steppingstone_to_sdram:
	@ Copy all 4KB data of Steppingstone to SDRAM
	@ Steppingstone addr: 0x00000000,  DSRAM addr: 0x30000000

	MOV		R1, #0
	LDR		R2, =SDRAM_BASE
	MOV		R3, #4*1024

1:
	LDR		R4, [R1], #4					@ Read 4Byte data from Steppingstone and let the source addr += 4
	STR		R4, [R2], #4					@ Copy this 4Byte data to SDRAM and let the destination addr += 4
	CMP		R1, R3							@ Judge if the copy is complete
	BNE		1b								
	MOV		PC, lr							@ Return

memsetup:
	@ Set the memory management to use SDRAM and other Device
	MOV		R1, #MEM_CTL_BASE				@ The begin addr of 13 memory control register 
	ADRl	R2, mem_cfg_val					@ Save the begin addr
	ADD		R3, R1, #52						@ 13 * 4 = 52
1:
	LDR		R4, [R2], #4					@ Read the set value and let r2 += 4
	STR		R4, [R1], #4					@ Write this value to register and let r1 += 4
	CMP		R1, R3							@ Judge if commplete all 13 registers
	BNE		1b
	MOV		PC, lr							@ Return


.align 4
mem_cfg_val:
	@ The set value of 13 mem control register
	.long	0x22011110						@ BWSCON
	.long	0x00000700						@ BANKCON0
	.long	0X00000700						@ BANKCON1
	.long	0X00000700						@ BANKCON2
	.long	0x00000700						@ BANKCON3
	.long	0x00000700						@ BANKCON4
	.long	0x00000700						@ BANKCON5
	.long	0x00018005						@ BANKCON6
	.long	0x00018005						@ BANKCON7
	.long	0x008c07a3						@ REFRESH
	.long	0x000000B1						@ BANKSIZE
	.long	0x00000030						@ MRSRB6
	.long	0x00000030						@ MRSRB7

