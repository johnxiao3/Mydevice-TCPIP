#include "Fx2.h"
#include "fx2regs.h"
#include "LED_Key.h"

#include "iic_comm.h"
#include "ssd1306.h"

#include "esp8266.h"
#include "MQTTPACKET.H"	
#include "string.h"


code unsigned char at_mode[14]={'A','T','+','C','W','M','O','D','E','=','1',0x0D,0x0A,'\0'}; 


code char Connect[133]=
{0x10,0x82,0x01,0x00,0x06,0x4d,0x51,0x49,0x73,0x64,0x70,0x03,0xc2,0x00,0x3c,0x00
,0x24,0x64,0x63,0x37,0x37,0x63,0x32,0x32,0x30,0x2d,0x34,0x64,0x38,0x33,0x2d,0x31
,0x31,0x65,0x37,0x2d,0x39,0x30,0x64,0x63,0x2d,0x63,0x64,0x36,0x33,0x39,0x63,0x61
,0x39,0x30,0x39,0x61,0x38,0x00,0x24,0x37,0x37,0x32,0x36,0x31,0x62,0x62,0x30,0x2d
,0x34,0x64,0x32,0x65,0x2d,0x31,0x31,0x65,0x37,0x2d,0x61,0x31,0x63,0x38,0x2d,0x30
,0x33,0x61,0x33,0x31,0x30,0x62,0x30,0x37,0x62,0x62,0x32,0x00,0x28,0x36,0x65,0x63
,0x64,0x63,0x37,0x34,0x31,0x39,0x39,0x64,0x34,0x62,0x34,0x33,0x64,0x37,0x63,0x30
,0x31,0x63,0x34,0x63,0x36,0x34,0x30,0x31,0x36,0x32,0x62,0x37,0x31,0x61,0x62,0x35
,0x30,0x61,0x37,0x37,0x63};	 

code char Subs[96]=
{0x82,0x5e,0x00,0x01,0x00,0x59,0x76,0x31,0x2f,0x37,0x37,0x32,0x36,0x31,0x62,0x62
,0x30,0x2d,0x34,0x64,0x32,0x65,0x2d,0x31,0x31,0x65,0x37,0x2d,0x61,0x31,0x63,0x38
,0x2d,0x30,0x33,0x61,0x33,0x31,0x30,0x62,0x30,0x37,0x62,0x62,0x32,0x2f,0x74,0x68
,0x69,0x6e,0x67,0x73,0x2f,0x64,0x63,0x37,0x37,0x63,0x32,0x32,0x30,0x2d,0x34,0x64
,0x38,0x33,0x2d,0x31,0x31,0x65,0x37,0x2d,0x39,0x30,0x64,0x63,0x2d,0x63,0x64,0x36
,0x33,0x39,0x63,0x61,0x39,0x30,0x39,0x61,0x38,0x2f,0x63,0x6d,0x64,0x2f,0x32,0x00};

code char Publish[93]=
{0x30,0x5d,0x00,0x5a,0x76,0x31,0x2f,0x37,0x37,0x32,0x36,0x31,0x62,0x62,0x30,0x2d
,0x34,0x64,0x32,0x65,0x2d,0x31,0x31,0x65,0x37,0x2d,0x61,0x31,0x63,0x38,0x2d,0x30
,0x33,0x61,0x33,0x31,0x30,0x62,0x30,0x37,0x62,0x62,0x32,0x2f,0x74,0x68,0x69,0x6e
,0x67,0x73,0x2f,0x64,0x63,0x37,0x37,0x63,0x32,0x32,0x30,0x2d,0x34,0x64,0x38,0x33
,0x2d,0x31,0x31,0x65,0x37,0x2d,0x39,0x30,0x64,0x63,0x2d,0x63,0x64,0x36,0x33,0x39
,0x63,0x61,0x39,0x30,0x39,0x61,0x38,0x2f,0x64,0x61,0x74,0x61,0x2f};


 
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
	MQTTHeader header = {0};
	MQTTConnectFlags flags = {0};

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
	ssd1306_printf("|   Mydevices   |  TCP IP Mode  |   By ZY Xiao  |     2017-7    ");
	Delay(5000);
		   
	Serial0_Init();
Restart:
	Serial0_SendString("+++");
	Delay(5000);
	
	ssd1306_initalize();
	ssd1306_clear();
	ssd1306_printf("|   Mydevices   |  TCP IP Mode  |  Reset Wifi   ");
	Serial0_SendString("AT+RST");
	Serial0_SendChar(0x0d);
	Serial0_SendChar(0x0a);
	Serial0_SendChar(0x00);
	Delay(10000);						
	ABCShow();
	Delay(10000);

Reconnect:
	ssd1306_initalize();
	ssd1306_clear();
	ssd1306_printf("|   Mydevices   |  TCP IP Mode  | Connect Server");
	count=0;
	Serial0_SendString("AT+CIPSTART=\"TCP\",\"192.168.2.3\",5000");
	Serial0_SendChar(0x0d);
	Serial0_SendChar(0x0a);
	Serial0_SendChar(0x00);
	Delay(10000);
	//temp_dataT[0]=count;ABCShow();Delay(10000);
	if(count==0x38) {count=0;goto Reconnect;}
	//temp_dataT[0]=count;ABCShow();Delay(10000);
	//if(count!=0x36) {count=0;goto Restart;}
	//if count =0x37 no ip maybe restart the modern	
	//count=0;
	ssd1306_initalize();
	ssd1306_clear();
	ssd1306_printf("| Connected |");
	Delay(5000);
	
	Serial0_SendString("AT+CIPMODE=1");
	Serial0_SendChar(0x0d);
	Serial0_SendChar(0x0a);
	Serial0_SendChar(0x00);
	Delay(5000);
	ABCShow();
	count=0;
	
	Serial0_SendString("AT+CIPSEND");
	Serial0_SendChar(0x0d);
	Serial0_SendChar(0x0a);
	Serial0_SendChar(0x00);
	Delay(10000); 
	if(count!=0x16) {count=0;goto Restart;}
	//temp_dataT[0]=count;ABC1Show();Delay(30000);
	count=0;

	Delay(10000);	

	//Serial0_SendString("Hahah");
	/*Serial0_SendString("AT+CWJAP=\"zhiyong_yang\",\"26996014\"");
	Serial0_SendChar(0x0d);
	Serial0_SendChar(0x0a);
	Serial0_SendChar(0x00);*/
		 
	LEDFlash();
	ssd1306_initalize();
	ssd1306_clear();
	ssd1306_printf("Waiting...");	
	while(1){
		Delay(5000);
		if (count==2)
		{
			//ABC1Show();
			if (temp_dataT[0]==0x01)
			{
				PD7=temp_dataT[1]&0x01;
				ssd1306_initalize();
				ssd1306_clear();
				if (temp_dataT[1])
					ssd1306_printf("Channel 1: ON");
				else
					ssd1306_printf("Channel 1: OFF");
			}
			if 	(temp_dataT[0]==0x00) 
			{
				Delay(10000);
				goto Restart;
			}	
		}
		count=0;
	}
	//LEDKeyTest();
}
