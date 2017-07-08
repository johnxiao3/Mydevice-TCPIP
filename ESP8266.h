#ifndef ESP8266_H     //Header sentry
#define ESP8266_H

void Serial0_Init();
void Serial0_SendChar(unsigned char source);
void Serial0_SendString(unsigned char temp[]);
void Serial0_SendString1(char* temp);
void Serial0_SendInt(int source);

void ABCshow();
void ABC1Show();