#ifndef SSD1306_H  /* Header Sentry */
#define SSD1306_H

#define SSD1306_CMD_DISPLAY_OFF 0xAE//--turn off the OLED
#define SSD1306_CMD_DISPLAY_ON 0xAF//--turn on oled panel 

#define BLACK 0
#define WHITE 1

//common parameters
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define SSD1306_FBSIZE 1024 //128x8
#define SSD1306_MAXROW 8

#define SSD1306_SETCONTRAST               0x81
#define SSD1306_DISPLAYALLON_RESUME       0xA4
#define SSD1306_DISPLAYALLON              0xA5
#define SSD1306_NORMALDISPLAY             0xA6
#define SSD1306_INVERTDISPLAY             0xA7
#define SSD1306_DISPLAYOFF                0xAE
#define SSD1306_DISPLAYON                 0xAF
#define SSD1306_SETDISPLAYOFFSET          0xD3
#define SSD1306_SETCOMPINS                0xDA
#define SSD1306_SETVCOMDETECT             0xDB
#define SSD1306_SETDISPLAYCLOCKDIV        0xD5
#define SSD1306_SETPRECHARGE              0xD9
#define SSD1306_SETMULTIPLEX              0xA8
#define SSD1306_SETLOWCOLUMN              0x00
#define SSD1306_SETHIGHCOLUMN             0x10
#define SSD1306_SETSTARTLINE              0x40
#define SSD1306_MEMORYMODE                0x20
#define SSD1306_COMSCANINC                0xC0
#define SSD1306_COMSCANDEC                0xC8
#define SSD1306_SEGREMAP                  0xA0
#define SSD1306_CHARGEPUMP                0x8D
#define SSD1306_EXTERNALVCC               0x1
#define SSD1306_INTERNALVCC               0x2
#define SSD1306_SWITCHCAPVCC              0x2


void writeCommand(unsigned char cmd);
void ssd1306_initalize();
void ssd1306update(); 
void ssd1306_clear();
void ssd1306_draw();
void ssd1306_printf(unsigned char dat[]);
void ssd1306_printchar(unsigned char dat);
 
