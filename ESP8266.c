#include "ESP8266.h"

#include "Fx2.h"
#include "fx2regs.h"
#include "string.h"

#include "ssd1306.h"
				
void Delay(WORD ms);

unsigned char temp_dataT[70];
unsigned char count=0;
void Serial0_Init()	 // serial UART 0 with Timer 1 in mode 2 or high speed baud rate generator
{
	CPUCS = 0x0a; //24Mhz
	UART230=0x01;// Enable high-speed baud rate generator for Serial port 0
	PCON 	&= ~0x80;// Disable doubling of the baud rate for Serial port 0 (115,2K instead 230K)
	SCON0 = 0x50; //Set Serial Mode = 1, Recieve enable bit = 1	
	ES0 = 1;     //ÔÊÐí´®¿Ú0ÖÐ¶Ï
    TR0 = 1;
	EA = 1; 
	//TMOD=0x20;  //timer1 mode2
	//CKCON|=0x10;//T1M=1
	//T2CON = 0x34; //
	//RCAP2H = 0xff; //Upper 8 bit of 16 bit counter to FF
	//RCAP2L = 0xfa; //value of the lower 8 bits of timer set to baud rate
}

void Serial0_SendString(char* temp)
{
	unsigned char i;
	for(i=0;i<strlen(temp);i++)
	{
		Serial0_SendChar(*(temp+i));
	}
}

void Serial0_SendString1(char* temp)
{
	unsigned char i;
	for(i=0;i<strlen(temp);i++)
	{
		Serial0_SendChar(temp[i]);
	}
}

void ABC1Show()
{
	unsigned char h,l,i;
	ssd1306_clear();	
	writeCommand(SSD1306_SETLOWCOLUMN | 0x0);  // low col = 0
    writeCommand(SSD1306_SETHIGHCOLUMN | 0x0);  // hi col = 0
    writeCommand(SSD1306_SETSTARTLINE | 0x0); // line #0   */
	writeCommand(0xb0);
	
	for (i=0;i<count;i++)
	{
		h= (temp_dataT[i]>>4)&0x0f;
		l=	temp_dataT[i]&0x0f;
		if (h<0x0a)
			ssd1306_printchar(h+0x30);
		else
			ssd1306_printchar(h+0x57);
		if (l<0x0a)
			ssd1306_printchar(l+0x30);
		else
			ssd1306_printchar(l+0x57);	
	}  			
	count=0;
}

void ABCshow()
{
	unsigned char h,l,i;
	ssd1306_clear();	
	writeCommand(SSD1306_SETLOWCOLUMN | 0x0);  // low col = 0
    writeCommand(SSD1306_SETHIGHCOLUMN | 0x0);  // hi col = 0
    writeCommand(SSD1306_SETSTARTLINE | 0x0); // line #0   */
	writeCommand(0xb0);
	//ssd1306_printf(temp_dataT);
	for (i=0;i<count;i++)
	{
		if ((temp_dataT[i]==0x0d) ||(temp_dataT[i]==0x0a) || (temp_dataT[i]==0x0b))
				ssd1306_printchar(0x20);
		else	
				ssd1306_printchar(temp_dataT[i]);	
	}
		/*
	for (i=0;i<count;i++)
	{
		h= (temp_dataT[i]>>4)&0x0f;
		l=	temp_dataT[i]&0x0f;
		if (h<0x0a)
			ssd1306_printchar(h+0x30);
		else
			ssd1306_printchar(h+0x57);
		if (l<0x0a)
			ssd1306_printchar(l+0x30);
		else
			ssd1306_printchar(l+0x57);	
	}  				  */
	count=0;
}
void Serial0_SendInt(int source)
{
	SBUF0 = (unsigned char)(source / 256);
	while(TI == 0);
	TI = 0;
	SBUF0 = (unsigned char)(source % 256);
	while(TI == 0);
	TI = 0;	
}

void Serial0_SendChar(char source)
{
	SBUF0 = source;
	while(TI == 0);
	TI = 0;
}
void ISR_USART0(void) interrupt 4
{        
	BYTE temp_data,h,l;
	if(RI == 1)
	{
		RI = 0;	
		//temp_data=SBUF0;
		if (count <70)
		{
			temp_dataT[count]=SBUF0;
			count++;
			if(temp_dataT[count-1]==0x0a);
		}
		else
		{
			count=0;
		}
			//ABCShow();
		/*if(temp_dataT[count-1]==0xff)
			PD7=1;
		if(temp_dataT[count-1]==0xfe)
			PD7=0;			  */	
		//	ssd1306_printchar(temp_data);
		
	}
}    
										

