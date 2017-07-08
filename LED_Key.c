#include "Fx2.h"
#include "fx2regs.h"

void SetupPortE(void)
{
	OEE = 0x0F;	
}

void Delay1ms()	//delay 1ms
{
    int j;
    int i;
    for(i=0;i<10;i++)
    {
        for(j=0;j<19;j++);
    }
}

void Delay(WORD ms)	//delay specific ms
{
    while(ms--)Delay1ms();
}	 
BYTE ScanKey() //return the key number.
{
	if ((IOE&0x40)==0x00)
		return 3;
	if ((IOE&0x20)==0x00)
		return 2;
	if ((IOE&0x10)==0x00)
		return 1;
	return 0;	
}
void LEDFlow() //LED flow
{
	IOE = ~0x01;
	Delay(500);
	IOE = ~0x02;
	Delay(500);
	IOE = ~0x04;
	Delay(500);
	IOE = ~0x08;
	Delay(500);	
}

void LEDFlowRev() //LED reverse flow
{
	IOE = ~0x08;
	Delay(500);
	IOE = ~0x04;
	Delay(500);
	IOE = ~0x02;
	Delay(500);
	IOE = ~0x01;
	Delay(500);	
}

void LEDFlash()	//All LED flash 
{
	IOE = ~0x0f;
	Delay(500);
	IOE = ~0x00;
	Delay(500);	
}

void LEDKeyTest()	//The program to test the key and LED function
{
	BYTE flag=1;
	SetupPortE();
    while (1)
    {
		switch(ScanKey()){
		case 3: flag=3;break;
		case 2: flag=2;break;
		case 1: flag=1;break;
		default: ;}

		switch(flag)
		{
			case 1:
				LEDFlow();
				break;	
			case 2:
				LEDFlash();
				break;
			case 3:
				LEDFlowRev();
				break;
		}	 
    }	
}


