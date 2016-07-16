.text

.global	_start
_start:
	LDR		R0, =0X53000000		@ WATCHDOG ADDR
	MOV		R1, #0X0 
	STR		R1, [R0]			@ Disable WATCHDOG
	LDR		sp, =1024*4			@ Set stack, Size <= 4KB
	BL		main				@ Call main function at C source file

HALT_LOOP:
	B		HALT_LOOP
