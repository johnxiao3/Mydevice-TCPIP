#include "Fx2.h"
#include "fx2regs.h"
#include "LED_Key.h"

#include "iic_comm.h"
#include "ssd1306.h"

#include "esp8266.h"
#include "MQTTPACKET.H"	
#include "string.h"



BYTE ScanKey();

 
extern unsigned char temp_dataT[70];
extern unsigned char count;
unsigned char Lig=1;
unsigned char CountineCheck=0;

void main(void)
{  
	/*****************************************
 	* Write to slave device with
 	* slave address e.g. say 0x20
 	*****************************************/
	//unsigned char ack;
	//MQTTClient_connectOptions pdata a;

	int len=0;
	int i=0;
	int Light1=0;

	OED=0xff;
	OEB=0xff;
	OEE=0x0f;
	Delay(1000);
	PD5=1;	//PD5 is the display
	PD7=0;   //PD7 is channel 1; 0 is off; 1 is on;
	LEDFlash();
	
	I2CInit();

	ssd1306_initalize();
	ssd1306_clear();
	ssd1306_printf("|   Mydevices   |  UDP    Mode  |   By ZY Xiao  |     2017-7    ");
	Delay(5000);
		   
	Serial0_Init();
Restart:
	Serial0_SendString("+++");
	Delay(5000);
	
	ssd1306_initalize();
	ssd1306_clear();
	ssd1306_printf("|   Mydevices   |  UDP    Mode  |  Reset Wifi   ");
	Serial0_SendString("AT+RST");
	Serial0_SendChar(0x0d);
	Serial0_SendChar(0x0a);
	Serial0_SendChar(0x00);
	Delay(10000);						
	ABCShow();
	Delay(5000);

	//ssd1306_initalize();
	//ssd1306_clear();
	//ssd1306_printf(" Connecting...");
Reconnect:
	count=0;
	/*Serial0_SendString("AT+GMR");
	Serial0_SendChar(0x0d);
	Serial0_SendChar(0x0a);
	Serial0_SendChar(0x00);
	Delay(5000);
	ABCShow();
	Delay(1000);	  */

	

	/*Serial0_SendString("AT+CWQAP");
	Serial0_SendChar(0x0d);
	Serial0_SendChar(0x0a);
	Serial0_SendChar(0x00);
	Delay(5000);
	ABCShow();
	Delay(1000);

	Serial0_SendString("AT+CWMODE=3");
	Serial0_SendChar(0x0d);
	Serial0_SendChar(0x0a);
	Serial0_SendChar(0x00);
	Delay(5000);
	ABCShow();
	Delay(1000);

	Serial0_SendString("AT+CWJAP=\"zhiyong_yang\",\"26996014\"");
	Serial0_SendChar(0x0d);
	Serial0_SendChar(0x0a);
	Serial0_SendChar(0x00);
	Delay(20000);
	ABCShow();
	Delay(30000);
	ABCShow();	  */

	//Serial0_SendString("AT+CWMODE_CUR=3");
	//Serial0_SendChar(0x0d);
	//Serial0_SendChar(0x0a);
	//Serial0_SendChar(0x00);
	//Delay(5000);
	//ssd1306_initalize();
	//ssd1306_clear();
	//ABCShow();
	//ssd1306_printf("| Connected |");
	//Delay(1000);
	
	

	//Serial0_SendString("AT+CIPSERVER=1,80");
	//Serial0_SendChar(0x0d);
	//Serial0_SendChar(0x0a);
	//Serial0_SendChar(0x00);
	//Delay(5000);
	//ABCShow();
	//Delay(1000);
	/*Serial0_SendString("AT+CIFSR");
	Serial0_SendChar(0x0d);
	Serial0_SendChar(0x0a);
	Serial0_SendChar(0x00);
	Delay(3000);
	ABCShow();
	Delay(10000);	  */

	Serial0_SendString("AT+CIPMUX=1");
	Serial0_SendChar(0x0d);
	Serial0_SendChar(0x0a);
	Serial0_SendChar(0x00);
	Delay(5000);
	ABCShow();
	Delay(1000);


	Serial0_SendString("AT+CIPSTART=0,\"UDP\",\"192.168.2.3\",4445,4445,2");
	Serial0_SendChar(0x0d);
	Serial0_SendChar(0x0a);
	Serial0_SendChar(0x00);
	Delay(3000);
	ABCShow();
	Delay(1000);

	while(1){
		for(i=0;i<100;i++)
		{
			 if (ScanKey())
			 {
			 	PD7=~PD7;
				Light1=~Light1;
				if (Light1)
					ssd1306_printf("Channel 1: ON");
				else
					ssd1306_printf("Channel 1: OFF");
				Delay(500);
			 }
			 
		}	
		if (count==0x0f)
		{
			PD7=temp_dataT[count-1]&0x01;
			ssd1306_initalize();
			ssd1306_clear();
			if (temp_dataT[count-1]-0x30)
				ssd1306_printf("Channel 1: ON");
			else
				ssd1306_printf("Channel 1: OFF");
			Light1=temp_dataT[count-1]-0x30;
			count=0;
			//temp_dataT[0]=count;ABC1Show();Delay(10000);
		}
	}
	//LEDKeyTest();
}
