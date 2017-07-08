#ifndef LED_KEY_H   /* Header Sentry */
#define LED_KEY_H

void SetupPortE(void);

void Delay1ms();	//delay 1ms

void Delay(WORD ms);	//delay specific ms

BYTE ScanKey(); //return the key number.

void LEDFlow(); //LED flow

void LEDFlowRev(); //LED reverse flow

void LEDFlash();	//All LED flash 

void LEDKeyTest();	//The program to test the key and LED function