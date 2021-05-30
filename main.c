



#include "ports_init.h"


#include "test.h"



int main(void)
{
	char* s;
	char ss[4];
	char c;
	int i;
	
	Systic_init();
  portA_init();
	portB_init();
	portD_init();
	portE_init();
	portF_init();
	
	
    //----------------------------UART initialization------------------------//

    SYSCTL->RCGCUART |=  (1<<0);//4; //enable clock for UART2  port D
    SYSCTL->RCGCGPIO |=   (1<<0);  //  8;
    GPIOA->AFSEL |=        (1<<1)|(1<<0);     //0xC0;   //port 6,7
    GPIOA->PCTL |=     (1<<0)|(1<<4);    //0xC0;
    GPIOA->DEN |=       (1<<0)|(1<<1);          //0xC0;

    UART0->CTL &=  ~(1<<0);      
    //after calculationg baudrate
    UART0->IBRD =104;
    UART0->FBRD =11;

    UART0->LCRH =  (0x3<<5)|(1<<4);       
    UART0->CC= 0x0;
    UART0->CTL =	 (1<<0)|(1<<8)|(1<<9);			
		
		// configration of leds
		SYSCTL->RCGCGPIO |=0x20;
		GPIOF->DIR |= 0x0E;  //MAKE LEDS
		GPIOF->DEN |=0x0E;
		GPIOF->DATA &=~ 0x0E;  // LEDS OFF
		while(1)
		{
			
			Print_String("Type Ron to turn led on, And Roff to turn it off:  ");
			
			for(i=0;i<4;i++)
				ss[i]=Read_Char();
			
			
			Print_String("\n\r");
			
			
			if(ss[0]=='R'&&ss[1]=='o'&&ss[2]=='n')
				GPIOF->DATA |=0x02;
			
			else if(ss[0]=='R' && ss[1]=='o'&&ss[2]=='f'&&ss[3]=='f')
				GPIOF->DATA &=~0x02;
			if(ss[0]=='B'&&ss[1]=='o'&&ss[2]=='n')
				GPIOF->DATA |=0x04;
			
			else if(ss[0]=='B' && ss[1]=='o'&&ss[2]=='f'&&ss[3]=='f')
				GPIOF->DATA &=~0x04;
			
			if(ss[0]=='G'&&ss[1]=='o'&&ss[2]=='n')
				GPIOF->DATA |=0x08;
			
			else if(ss[0]=='G' && ss[1]=='o'&&ss[2]=='f'&&ss[3]=='f')
				GPIOF->DATA &=~0x08;
	}

   


}




