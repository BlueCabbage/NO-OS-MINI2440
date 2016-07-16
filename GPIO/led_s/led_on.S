.text

.global _start

_start:
	LDR		R0,	=0X56000010		@ Set R0 = GPBCON
	MOV		R1, #0X00015400		@ Set [11 10] = 0b01
	STR		R1, [R0]			@ Set PB5 output mode
	LDR		R0, =0X56000014		@ Set RO = GPBDAT
	MOV		R1, #0x00000000     
	STR		R1, [R0]			@ Set GPB5 = 0

MAIN_LOOP:
	B		MAIN_LOOP
