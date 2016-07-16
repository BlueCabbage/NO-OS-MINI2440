/*************************************************************************
    > File Name: init.c
    > Author: ZHAOCHAO
    > Mail: 479680168@qq.com 
    > Created Time: Sat 16 Jul 2016 01:52:35 AM PDT
 ************************************************************************/

/*
 * init.c  initialization run at Steppingstone
 *		   until now the mmu have not run, use PA
 */

/* WATCHDOG Register */
#define	WTCON		(*(volatile unsigned long *)0x53000000)

/* Memory Control Register Begin Addr */
#define MEM_CTL_BASE	 0x48000000

// Close the watchdog
void disable_watch_dog(void)
{
	WTCON = 0; 
}

// setup sdram
void memsetup(void)
{
	unsigned long const mem_cfg_val[] = {
		0x22011110,		// BWSCON
		0x00000700,		// BANKCON0
		0x00000700,		// BANKCON1
		0x00000700,		// BANKCON2
		0x00000700,		// BANKCON3
		0X00000700,		// BANKCON4
		0X00000700,		// BANKCON5
		0X00018005,		// BANKCON6
		0X00018005,		// BANKCON7
		0X008C07A3,		// REFRESH
		0X000000B1,		// BANKSIZE
		0X00000030,		// MRSRB6
		0X00000030,		// MRSRB7
	};

	int i = 0;
	volatile unsigned long *p = (volatile unsigned long *)MEM_CTL_BASE;

	for ( ; i < 13; i ++ ) {
		p[i] = mem_cfg_val[i];
	}
}


/* 
 * Copy the 2th code to SDRAM	
 */
void copy_2th_to_sdram(void)
{
	unsigned int *pdwSrc  = (unsigned int *)2048;
	unsigned int *pdwDest = (unsigned int *)0x30004000;

	while (pdwSrc < (unsigned int *)4096) {
		*pdwDest = *pdwSrc;
		pdwDest ++;
		pdwSrc ++;
	}
}


/*
 * Set up the page table
 */
void create_page_table(void)
{
	/*
	 *	some define
	 */
	#define MMU_FULL_ACCESS		(3 << 10)	// the authority of get
	#define	MMU_DOMAIN			(0 << 5)    // domain
	#define	MMU_SPECIAL			(1 << 4)    // MUST BE 0x1b
	#define	MMU_CACHEABLE		(1 << 3)	// cacheable
	#define	MMU_BUFFERABLE		(1 << 2)	// bufferable
	#define MMU_SECTION			(2)			// Section describe symbol
	#define	MMU_SECDESC			(MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | MMU_SECTION)
	#define	MMU_SECDESC_WB		(MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SECTION | MMU_CACHEABLE | MMU_BUFFERABLE | MMU_SECTION)
	#define	MMU_SECTION_SIZE	0X00100000

		
	unsigned long virtualaddr, physicaladdr;
	unsigned long *mmu_tlb_base = (unsigned long *)0x30000000;

	/*
	 *	Steppingstone's begin addr = 0x0
	 */

	virtualaddr = 0;
	physicaladdr = 0;

	*(mmu_tlb_base + (virtualaddr >> 20)) = (physicaladdr & 0xFFF00000) | MMU_SECDESC_WB;

	/*
	 *	GPIO Register begin addr 0x56000000
	 *	GPBCON, GPBDAT = 0X56000000, 0X56000014
	 *  0XA0000000 -> 0X56000000
	 *  size: 1MB
	 */
	virtualaddr = 0xA0000000;
	physicaladdr = 0x56000000;
	*(mmu_tlb_base + (virtualaddr >> 20)) = (physicaladdr & 0xFFF00000) | MMU_SECDESC;


	/*
	 * SDRAM addr 0x30000000 - 0x33ffffff
	 * 0xB0000000 - 0xB3FFFFFF -> 0x30000000 - 0x33FFFFFF  
	 * size: 64MB, relate 64 section describe symbol 
	 */
	virtualaddr = 0xB0000000;
	physicaladdr = 0x30000000;

	while (virtualaddr < 0xB4000000) {
		*(mmu_tlb_base + (virtualaddr >> 20)) = (physicaladdr & 0xFFF00000) | MMU_SECDESC_WB;

		virtualaddr += 0x100000;
		physicaladdr += 0x100000;
	}

}


/*
 *	Run the MMU
 */
void mmu_init(void) 
{
	unsigned long ttb = 0x30000000;

	 __asm__(
	 		"MOV	R0, #0\n"
	 		"MCR	P15, 0, R0, C7, C7, 0\n"	// Disable ICaches and DCaches
	 		"MCR	P15, 0, R0, C7, C10, 4\n"   // Drain write buffer on v4
	 		"MCR	P15, 0, R0, C8, C7, 0\n"	// Disable command/data TLB
	 		"MOV	R4, %0\n"					// R4 = page table base addr
	 		"MCR	P15, 0, R4, C2, C0, 0\n"	// Set Page table base register
	 		"MVN	R0, #0\n" 
	 		"MCR	P15, 0, R0, C3, C0, 0\n"	// Domain get control register = 0XFFFFFFFF, DON't check authority
	 		/*
	 		 *  For control register, first read its value and then modified interesting bit , final wirte back
	 		 */
	 		"MRC	P15, 0, R0, C1, C0, 0\n"	// Read out control register's value

	  	
	 		/*	Control Register low 16bit's mean
	 		 *  R: the method of switch out the items of cache
	 		 *		0 = Random replacement;
	 		 *		1 = Round robin replacement;
	 		 *	V: Show the addr of error vectory
	 		 *		0 = Low addresses = 0x00000000;
	 		 *		1 = High addresses = 0xFFFF0000;
	 		 *	I: 
	 		 *		0 = Off ICaches;
	 		 *		1 = On  ICaches;
	 		 *	R,S: relate with Page table's dscribe symbol to comfirm the memory' get authority
	 		 *	B:
	 		 *		0 = CPU is small byte sequences;
	 		 *		1 = CPU is big byte sequcences;
	 		 *	C: 
	 		 *		0 = Off DCaches;
	 		 *		1 = On DCaches;
	 		 *	A: 
	 		 *		0 = Don't check align addr when get data
	 		 *		1 = Check align addr when get data
	 		 *	M:
	 		 *		0 = Off MMU
	 		 *		1 = On  MMU
		 	 */
	  	
	 	     /*
	 		  *	 First clear bit not used
	 		  */
	 											// .RVI ..RS B... .CAM 
	 		"BIC	R0, R0, #0X3000\n"			// ..11 .... .... .... clear bit V I
	 		"BIC	R0, R0, #0X0300\n"			// .... ..11 .... .... clear bit R S
	 		"BIC	R0, R0, #0X0087\n"			// .... .... 1... .111 clear bit B C A M

	 		/*
	 		 *  Set the needed bit
	 		 */
	 		"ORR	R0, R0, #0X0002\n"			// .... .... .... ..1. On align check
	 		"ORR	R0, R0, #0X0004\n"			// .... .... .... .1.. On DCaches 
	 		"ORR	R0, R0, #0X1000\n"			// ...1 .... .... .... On ICaches
	 		"ORR	R0, R0, #0X0001\n"			// .... .... .... ...1 Able MMU

	 		"MCR	P15, 0, R0, C1, C0, 0\n"	// Write the value to control register
	 		: /* No output */  
	 		: "r" (ttb) );
}


