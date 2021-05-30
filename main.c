

/* GPS
 * TX-->PD6
 * RX-->PD7
 * Vcc--> 3.3V
 */

#include "ports_init.h"
#include "LCD_display.h"
#include "measure_distance.h"
#include "test.h"

char* saatAyarla(char str[]);
void readGPSModule();

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
	LCD_INIT();
	
    //----------------------------UART initialization------------------------//

    SYSCTL->RCGCUART |=  (1<<0);//4; //enable clock for UART2  port D
    SYSCTL->RCGCGPIO |=   (1<<0);  //  8;
    GPIOA->AFSEL |=        (1<<1)|(1<<0);     //0xC0;   //port 6,7
    GPIOA->PCTL |=     (1<<0)|(1<<4);    //0xC0;
    GPIOA->DEN |=       (1<<0)|(1<<1);          //0xC0;

    UART0->CTL &=  ~(1<<0);      //~(0x20);
    //after calculationg baudrate
    UART0->IBRD =104;
    UART0->FBRD =11;

    UART0->LCRH =  (0x3<<5)|(1<<4);       //0x60; //desired serial parameter
    UART0->CC= 0x0;
    UART0->CTL =	 (1<<0)|(1<<8)|(1<<9);			//0x301;  //enable UART by setting UARTEN bi in the UARTCTL REG
		
		// configration of leds
		SYSCTL->RCGCGPIO |=0x20;
		GPIOF->DIR |= 0x0E;  //MAKE LEDS
		GPIOF->DEN |=0x0E;
		GPIOF->DATA &=~ 0x0E;  // LEDS OFF
		while(1)
		{
			//Print_Char('A');
			//Read_Char();
				Print_String("Type Ron to turn led on, And Roff to turn it off:  ");
			
				//s=Read_String('\r');
			for(i=0;i<4;i++)
			{
				ss[i]=Read_Char();
			}
			//ss[0]=Read_Char();
				Print_String("\n\r");
			//Print_Char(ss[0]);
		//		for(i=0;i<4;i++)
		//	{
		//		ss[i] = *(s++);
			//}
			
				if(ss[0]=='R'&&ss[1]=='o'&&ss[2]=='n')
				{
					GPIOF->DATA |=0x02;
				}
				else if(ss[0]=='R' && ss[1]=='o'&&ss[2]=='f'&&ss[3]=='f')
					GPIOF->DATA &=~0x02;
			if(ss[0]=='B'&&ss[1]=='o'&&ss[2]=='n')
				{
					GPIOF->DATA |=0x04;
				}
				else if(ss[0]=='B' && ss[1]=='o'&&ss[2]=='f'&&ss[3]=='f')
					GPIOF->DATA &=~0x04;
				if(ss[0]=='G'&&ss[1]=='o'&&ss[2]=='n')
				{
					GPIOF->DATA |=0x08;
				}
				else if(ss[0]=='G' && ss[1]=='o'&&ss[2]=='f'&&ss[3]=='f')
					GPIOF->DATA &=~0x08;
	}

   /* while(1)
    {
		 portA_init();
		 portB_init();
			LCD_CMD(0x38);
			Systic_Wait_10ms(1);
			LCD_CMD(0x01);  //clear display
			Systic_Wait_10ms(1);
			LCD_CMD(0x10);    //cursour display shift
			Systic_Wait_10ms(1);
			LCD_CMD(0x0C);   // display on
			Systic_Wait_10ms(1);
			LCD_WRITE('A');
			Systic_Wait_10ms(1);
			
        //readGPSModule();
    }*/
		LCD_CMD(0x01);
		LCD_CMD(0x80);
		Delay_Milli(500);/*
		LCD_WRITE('H');
		Delay_Milli(1);
		LCD_WRITE('E');
		Delay_Milli(1);
		LCD_WRITE('L');
		Delay_Milli(1);
		LCD_WRITE('O');
		Delay_Milli(1);
		LCD_WRITE('O');
		Delay_Milli(1);
		LCD_WRITE(' ');
		Delay_Milli(1);
		LCD_WRITE('W');
		Delay_Milli(1);
		LCD_WRITE('O');
		Delay_Milli(1);
		LCD_WRITE('R');
		Delay_Milli(1);
		LCD_WRITE('L');
		Delay_Milli(1);
		LCD_WRITE('D');
		Delay_Milli(1);*/
		LCD_WRITE(4);
		Delay_Milli(1);
		
		


}



void readGPSModule(void){
    char c0,GPSValues[100],latitudeResult[10],longitudeResult[10],parseValue[12][20],*token,tarih[9],*saat,guncelSaat[9];
    double latitude=0.0,longitude=0.0,seconds=0.0,result=0.0,minutes=0.0;
    const char comma[2] = ",";
    int index=0,degrees,i=0,j=0;

    while((UART2->FR &0x20)!=0);
    c0= UART2->DR;

    //gelen data $GPRMC mi?
    if(c0=='$'){
				char c1=UART2->DR;
        while((UART2->FR & 0x20)!=0);
        
        if(c1=='G'){
						char c2=UART2->DR;
            while((UART2->FR & 0x20)!=0);
            
            if(c2=='P'){
								char c3=UART2->DR;
                while((UART2->FR & 0x20)!=0);
                
                if(c3=='R'){
										char c4=UART2->DR;
                    while((UART2->FR & 0x20)!=0);
                    
                    if(c4=='M'){
											char c5=UART2->DR;
                        while((UART2->FR & 0x20)!=0);
                        
                        if(c5=='C'){
														char c6=UART2->DR;
                            while((UART2->FR & 0x20)!=0);
                            
                            if(c6==','){
																char c7=UART2->DR;
                                while((UART2->FR & 0x20)!=0);
                                

                                //GET  gps values as array and check sum
                                while(c7!='*'){
                                    GPSValues[index]=c7;
                                    while((UART2->FR & 0x20)!=0);
                                    c7=UART2->DR;
                                    index++;}


                                //GPSValues arrayindeki verileri virgul e gore ayirma
                                index=0;
                                token = strtok(GPSValues, comma);
                                while( token != NULL ) {
                                    strcpy(parseValue[index], token);
                                    token = strtok(NULL, comma);
                                    index++;}


                                //parseValue[1] = A ise veri gecerli - V ise gecerli degil
                                if(strcmp(parseValue[1],"A")==0){
                                    latitude=atof(parseValue[2]);
                                    longitude=atof(parseValue[4]);


                                    //latitude calculation
                                    degrees=latitude/100;
                                    minutes=latitude-(double)(degrees*100);
                                    seconds=minutes/60.00;
                                    result=degrees+seconds;
                                    sprintf(latitudeResult,"%f", result);


                                    //longitude calculation
                                    degrees=longitude/100;
                                    minutes=longitude-(double)(degrees*100);
                                    seconds=minutes/60.00;
                                    result=degrees+seconds;
                                    sprintf(longitudeResult, "%f", result);


                                    //printf("https://www.google.com/maps/place/%s+%s \n",latitudeResult,longitudeResult);
                                    //tarih duzeltme
                                    for(i=0;i<6;i++){
                                        tarih[j]=parseValue[index-2][i];
                                        if(i==1 || i==3){
                                            j++;
                                            tarih[j]='/';}
                                        j++;}
                                    tarih[8]='\0';


                                    //saat düzeltme +3 UTC ayarlama
                                    saat=saatAyarla(parseValue[0]);
                                    j=0;
                                    for(i=0;i<6;i++){
                                        guncelSaat[j]=saat[i];
                                        if(i==1 || i==3){
                                            j++;
                                            guncelSaat[j]=':';}
                                        j++;}
                                    guncelSaat[8]='\0';




                                   // printf("Saat  = %s\n",guncelSaat);
                                    //printf("Tarih = %s\n",tarih);
                                    //printf("Enlem = %s\n",latitudeResult);
                                    //printf("Boylam= %s\n\n\n",longitudeResult);
                                    //SysCtlDelay(SysCtlClockGet()/6);
																				}
                                else{
                                    //printf("  GPS Verileri Okunuyor\n\n\n");
																	}

                                //printf("");
                        }}}}}}}
}


char* saatAyarla(char str[]){
    int num10 = str[0]-'0';
    int num1 = str[1]-'0';
    int saatVerisi=num10*10+num1;

    saatVerisi=saatVerisi+3;
    if(saatVerisi>24){
        saatVerisi=saatVerisi%24;
        if(saatVerisi<10){
            str[0]='0';
            str[1]=saatVerisi+'0';}
        else{
            str[0]=(saatVerisi/10)+'0';
            str[1]=(saatVerisi%10)+'0';}}
    else{
        str[0]=(saatVerisi/10)+'0';
        str[1]=(saatVerisi%10)+'0';}
    return str;
}
