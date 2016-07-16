/*************************************************************************
    > File Name: led_on.c
    > Author: ZHAOCHAO
    > Mail: 479680168@qq.com 
    > Created Time: Fri 15 Jul 2016 10:36:09 PM PDT
 ************************************************************************/

#define GPBCON		(*(volatile unsigned long *)0x56000010)
#define GPBDAT		(*(volatile unsigned long *)0x56000014)

int main()
{
	GPBCON = 0X00005400;	// Set GPB5 output mode [11 10] = 0xb01
	GPBDAT = 0X00000000;	// Set GPB5 = 0;

	return 0;
}
