/*************************************************************************
    > File Name: serial.h
    > Author: ZHAOCHAO
    > Mail: 479680168@qq.com 
    > Created Time: Sat 23 Jul 2016 11:04:26 PM PDT
 ************************************************************************/

void uart0_init(void);
void putc(unsigned char c);
unsigned char getc(void);
int isDigit(unsigned char c);
int isLetter(unsigned char c);
