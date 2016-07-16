.text
.global _start
_start:
	LDR		sp, =4096			@ Set stack 
	BL		disable_watch_dog
	BL		memsetup
	BL		copy_2th_to_sdram
	BL		create_page_table		@ Set the page table
	BL		mmu_init			@ Run the mmu
	LDR		SP, =0XB4000000			@ Reset the stack pointer
	LDR		PC, =0xB0004000			@ Jump to SDRAM to continue

halt_loop:
	B		halt_loop

