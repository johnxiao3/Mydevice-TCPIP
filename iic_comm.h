#ifndef IIC_COMM_H   /* Header Sentry */
#define IIC_COMM_H

#define SDA PB0
#define SCL PB1

void I2CInit();
void I2CStart();
void I2CRestart();
void I2CStop();
void I2CAck();
void I2CNak();
unsigned char I2CSend(unsigned char Data);
unsigned char I2CRead();