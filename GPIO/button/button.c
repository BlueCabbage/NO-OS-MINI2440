/*************************************************************************
    > File Name: button.c
    > Author: ZHAOCHAO
    > Mail: 479680168@qq.com 
    > Created Time: Fri 15 Jul 2016 11:05:27 PM PDT
 ************************************************************************/

#define	GPBCON		(*(volatile unsigned long *)0x56000010)
#define GPBDAT		(*(volatile unsigned long *)0x56000014)

#define GPGCON		(*(volatile unsigned long *)0x56000060)
#define GPGDAT		(*(volatile unsigned long *)0x56000064)


/*
 *   LED1-4: GPB5-8   KEY1-6: GPG0,3,5,6,7,11
 */

#define GPB5_OUT	(1<<(5*2))
#define GPB6_OUT	(1<<(6*2))
#define GPB7_OUT	(1<<(7*2))
#define GPB8_OUT	(1<<(8*2))

#define GPG1_IN		~(3<<(1*2))
#define GPG3_IN		~(3<<(3*2))
#define GPG5_IN		~(3<<(5*2))
#define GPG6_IN		~(3<<(6*2))
#define GPG7_IN		~(3<<(7*2))
#define GPG11_IN	~(3<<(11*2))

int main()
{
	unsigned long dwDat;
	
	// Set LED1-4 
	GPBCON = GPB5_OUT | GPB6_OUT | GPB7_OUT | GPB8_OUT;

	// Set Key1-2
	GPGCON = GPG1_IN | GPG3_IN | GPG5_IN | GPG6_IN;

	while (1) {
		// If Keyn = 0(Pressed), Let Ledn = 0(On)
		dwDat = GPGDAT;

		if (dwDat & (1 << 0))		// Key1 no pressed
			GPBDAT |= (1<<5);		// Led1 off
		else
			GPBDAT &= ~(1<<5);	    // Led1 on

		
		if (dwDat & (1 << 3))		// Key2 no pressed
			GPBDAT |= (1 << 6);
		else
			GPBDAT &= ~(1 << 6);	

		if (dwDat & (1 << 5))
			GPBDAT |= (1 << 7);
		else
			GPBDAT &= ~(1 << 7);

		if (dwDat & (1 << 6))
			GPBDAT |= (1 << 8);
		else
			GPBDAT &= ~(1 << 8);
		
	}


	return 0;

}



