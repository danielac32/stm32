#include "font.h"

void delay_ms(long t);
void nokia_init();
void nokia_clean_buffer();
void nokia_refresh();
void nokia_write_command(unsigned char comando);
void nokia_write_data(unsigned char dato);
void nokia_shift(unsigned char n_dato);
void nokia_gotoxy(unsigned char x, unsigned char y);
void nokia_contrast(unsigned char contraste);
void nokia_printchar(unsigned char c,unsigned char color);
void nokia_plot(unsigned int x, unsigned int y, unsigned char color);
void nokia_line(signed int x1,signed int y1,signed int x2, signed int y2, unsigned char color);
void nokia_box(signed int x1, signed int y1, signed int x2, signed int y2, unsigned char fill, unsigned char color);
void nokia_bar(signed int x1, signed int y1, signed int x2, signed int y2, signed int width, unsigned char color);
void nokia_circle(signed int x, signed int y, signed int radius, unsigned char fill, unsigned char color);
void nokia_printstr(unsigned char* message,unsigned char color);
void nokia_bitmap(const unsigned char *bitmap,unsigned char x, unsigned char y, unsigned char sx, unsigned char sy, unsigned char color);
void nokia_custom_char(const unsigned char *bitmap,int invert);

unsigned char buffer_lcd[84][6];      //Memory map: 504 bytes, 84x6
unsigned char idx,idy;

/*
#define RST 2
#define _CE  3
#define _DC  4
#define _SDO 5
#define _SCK 6*/


#define RST 10
#define _CE  5
#define _DC  9
#define _SDO 3 //miso input
#define _SDI 4 //mosi
#define _SCK 2