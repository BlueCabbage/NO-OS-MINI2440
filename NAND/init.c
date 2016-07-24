/*************************************************************************
    > File Name: init.c
    > Author: ZHAOCHAO
    > Mail: 479680168@qq.com 
    > Created Time: Sat 16 Jul 2016 06:47:41 AM PDT
 ************************************************************************/
#define WTCON		(*(volatile unsigned long *)0x53000000)
//#define CLKDIVN		(*(volatile unsigned long *)0x4C000014)
//#define MPLLCON		(*(volatile unsigned long *)0x4c000004)

/* SDRAM Register */
//#define	S3C2440_MPLL_400MHZ		((0x7f<<12)|(0x02<<4)|(0x01))
#define MEM_CTL_BASE			0X48000000


void disable_watch_dog()
{
	WTCON = 0;
}


/* Set the 13 registers of SDRAM */
void memsetup(void)
{
	int i = 0;
	volatile unsigned long *p = (unsigned long *)MEM_CTL_BASE;

	unsigned long const mem_cfg_val[] = {
											0x22011110,		// BWSCON
											0X00000700,		// BANKCON0
											0X00000700,		// BANKCON1
											0X00000700,		// BANKCON2
											0X00000700,		// BANKCON3
											0X00000700,		// BANKCON4
											0X00000700,		// BANKCON5
											0X00018005,		// BANKCON6
											0X00018005,		// BANKCON7
											0X008C07A3,		// REFRESH
											0X000000B1,		// BANKSIZE
											0X00000030,		// MRSRB6
											0X00000030,		// MRSRB7
									};

	for (; i < 13; i ++) {
		p[i] = mem_cfg_val[i];
	}
}


