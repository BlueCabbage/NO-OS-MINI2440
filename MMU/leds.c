/*************************************************************************
    > File Name: leds.c
    > Author: ZHAOCHAO
    > Mail: 479680168@qq.com 
    > Created Time: Sat 16 Jul 2016 03:09:18 AM PDT
 ************************************************************************/

#define	GPBCON		(*(volatile unsigned long *)0XA0000010)			// Physical Addr = 0x56000010
#define GPBDAT		(*(volatile unsigned long *)0xA0000014)			// PA = 0x560000014

#define GPB5_OUT	(1 << (5*2))
#define GPB6_OUT	(1 << (6*2))
#define GPB7_OUT	(1 << (7*2))
#define GPB8_OUT	(1 << (8*2))


/* 
 *	the mean of wait function add "static inlin":
 *		when compile leds.c, wait embed in main function, the result of compule is only have
 *		one main function.
 *		so when the link occur, main function's addr is decided by the link file 
 *		and in the link file mmu.lds, it make the load addr of leds.o is 0xB0004000
 *		so that, In heal.S the "LDR	PC, 0XB0004000" is to jump to run the main function
 */
static inline void wait(unsigned long dly) 
{
	for (; dly > 0; dly --);
}


int main(void) 
{
	unsigned long i = 5;

	// Set the pin of led at output mode
	GPBCON = GPB5_OUT | GPB6_OUT | GPB7_OUT | GPB8_OUT;

//	while (1) {
//		wait(3000000);
//		GPBDAT = (~(i << 1));			// Turn on the led based on the value of i
//		if ( ++i == 9 )
//			i = 5;
//	}
	
	GPBDAT = (~(5 << 5));
	while(1);

	return 0;

}


