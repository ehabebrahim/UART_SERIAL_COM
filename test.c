/*
 * ports_init.c
 *
 *  Created on: May 26, 2021
 *      Author: ehab
 */
//#include <TM4C123.h>
#include "test.h"

char Read_Char(void)
{
	char c;
	while((UART0->FR &(1<<4))!=0);  //WAIT UNTILL RX BUFFER NOT FULL
	c=UART0->DR;
	return c;
}
void Print_Char(char c)
{
	while((UART0->FR &0x20)!=0);  //WAIT UNTILL TX BUFFER NOT FULL
	UART0->DR = c;
}

void Print_String(char * s)
{
	while(*s)
		Print_Char(*(s++));
}
