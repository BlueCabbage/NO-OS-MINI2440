/*************************************************************************
    > File Name: interrupt.c
    > Author: ZHAOCHAO
    > Mail: 479680168@qq.com 
    > Created Time: Fri 22 Jul 2016 09:16:26 PM PDT
 ************************************************************************/
#include "s3c24xx.h"

void Timer0_Handle(void)
{
    /*
     * 每次中断令4个LED改变状态
     */
    if(INTOFFSET == 10)
    {
        GPBDAT = ~(GPBDAT & (0xf << 5));
    }
    //清中断
    SRCPND = 1 << INTOFFSET;
    INTPND = INTPND;     
}
