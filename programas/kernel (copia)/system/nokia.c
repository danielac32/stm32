#include <os.h>



void delay_ms(long t){
    while(t--)
        asm("nop \n nop \n nop");
}

/*
#define RST 10
#define _CE  5
#define _DC  9
#define _SDO 3
#define _SCK 2
*/
/*
#define RST 10
#define _CE  5
#define _DC  9
#define _SDO 3 //miso input
#define _SDI 4 //mosi
#define _SCK 2
*/

void nokia_init(){
 
gpio_init(_CE, GPIO_FUNC_SIO);
gpio_dir(_CE, GPIO_OUT);

gpio_init(_DC, GPIO_FUNC_SIO);
gpio_dir(_DC, GPIO_OUT);

//gpio_init(RST, GPIO_FUNC_SIO);
//gpio_dir(RST, GPIO_OUT);
 spi_init(_SCK,_SDI,_SDO);


gpio_set(_DC,1);
gpio_set(_CE,1);
//gpio_set(RST,1);


nokia_write_command(0x21);    //Extended instructions set
nokia_write_command(0xc2);    //Vop
nokia_write_command(0x13);    //Bias
nokia_write_command(0x20);    //Horizontal mode from Left to Right, X coordinate increments automatically,
                             //0x22 for vertical addressing, also normal instructions set again
nokia_write_command(0x09);    //Everything ON
delay_ms(1);
nokia_clean_buffer();         //Memory map clean
nokia_refresh();              //Update display
delay_ms(1);
nokia_write_command(0x08);   //Everything OFF
delay_ms(1);
nokia_write_command(0x0c);   //Normal mode
}
void nokia_clean_buffer() {
   unsigned char i,j;
   for(j=0;j<6;j++){             //Memory map erased
      for(i=0;i<84;i++){
         buffer_lcd[i][j]=0x00;
      }
   }
}
void nokia_refresh() {
   unsigned int i,j;
   nokia_write_command(0x40|0);
   nokia_write_command(0x80|0);
   for(j=0;j<6;j++){             //Sends memory map to LCD
       for(i=0;i<84;i++){
         nokia_write_data(buffer_lcd[i][j]);
      }
   }
}
void nokia_write_command(unsigned char comando){
   //uint32 mask=disable();
   gpio_set(_DC,0);           //Command
   gpio_set(_CE,0);           //Chip enable
   nokia_shift(comando);
   gpio_set(_CE,1);          //Chip disable
   //restore(mask);
}
void nokia_write_data(unsigned char dato){
   ///uint32 mask=disable();
   gpio_set(_DC,1);         //Data
   gpio_set(_CE,0);           //Chip enable
   nokia_shift(dato);
   gpio_set(_CE,1);          //Chip disable
   //restore(mask);
}
void nokia_shift(unsigned char n_dato){

   #if spi == 0
   unsigned char caa;
   gpio_set(_SCK,0);  //nok_sclk=0;
   for (caa=8;caa;caa--){      //MSB first
      if (n_dato & 0x80){
          gpio_set(_SDO,1);//nok_sda=1;
      }else{
          gpio_set(_SDO,0);//nok_sda=0;
      }
      n_dato = n_dato << 1;

      gpio_set(_SCK,1);//nok_sclk=1;
      gpio_set(_SCK,0);// nok_sclk=0;
   }
   #else 
   transfer(n_dato);
   #endif
}
void nokia_gotoxy(unsigned char x, unsigned char y)
{
   if(x>83)
      x=83;
   if(y>5)
      y=5;
   idx=x;
   idy=y;
}
void nokia_contrast(unsigned char contraste)
{
    nokia_write_command(0x21);                  //LCD extended commands
    nokia_write_command(0x80 | contraste);      //Vop setting (contrast)
    nokia_write_command(0x20);                  //Returns to LCD normal commands and horizontal addressing
}
void nokia_custom_char(const unsigned char *bitmap,int invert){
     unsigned char s = 0;
    for(s = 0; s < 5; s++)
    {
        if (idx>83){
         idx=0;
         idy++;
         if (idy>5)
            idy=0;
        }
        if(!invert) {
           buffer_lcd[idx][idy]=~bitmap[s];
        }else{
           buffer_lcd[idx][idy]=bitmap[s];
        }
        idx++;
    }
}
void nokia_printchar(unsigned char c,unsigned char color)
{
   char char_row,char_pos,char_data;
   if (c<0x20) return;                          //Verifies that the character is viewable
   if (c>0x7f) return;

   for (char_row=0;char_row<5;char_row++)
   {
      if (idx>83){
         idx=0;
         idy++;
         if (idy>5)
            idy=0;
      }
      char_data = font[(c - 0x20)][char_row];
      if(color==0){
         char_data = ~char_data;
      }
      buffer_lcd[idx][idy]=char_data;
      idx++;
   }

}

void nokia_plot_xor(unsigned int x, unsigned int y) {
    unsigned char offset;
    char  _data;
    if (x > 83) return;         //x must be less than 83
    if (y > 47) return;         //y must be less than 47

    offset = y - ((y / 8) * 8);
    nokia_gotoxy(x, y/8);
    _data = (0x01 << offset);
     buffer_lcd[idx][idy]=buffer_lcd[idx][idy]^_data;  //OR between buffer position and new pixel
}
void nokia_clrplot(unsigned int x, unsigned int y) {
    unsigned char offset;
    char  _data;
    if (x > 83) return;         //x must be less than 83
    if (y > 47) return;         //y must be less than 47

    nokia_gotoxy(x, y/8);
    offset = y - ((y / 8) * 8);
    buffer_lcd[idx][idy]=buffer_lcd[idx][idy]&~(1<<offset);
}
void nokia_plot(unsigned int x, unsigned int y, unsigned char color) {
    unsigned char offset;
    char  _data;
    if (x > 83) return;         //x must be less than 83
    if (y > 47) return;         //y must be less than 47

    offset = y - ((y / 8) * 8);
    nokia_gotoxy(x, y/8);
    if (color)
         _data = (0x01 << offset);
    else {
         _data = 0xff;
    }
    if (color){
         buffer_lcd[idx][idy]=buffer_lcd[idx][idy]|_data;  //OR between buffer position and new pixel
    }else{
         buffer_lcd[idx][idy]=buffer_lcd[idx][idy]&_data;
    }
}
/////////////////////////////////////////////////////////////////////////
//Draws a line
//(x1, y1) - starting point
//(x2, y2) - end point
//color - on/off
void nokia_line(signed int x1,signed int y1,signed int x2, signed int y2, unsigned char color)
{
   signed int dy, dx;
   signed int  addx=1, addy=1;
   signed int P, diff;

   signed int i=0;
   dx = abs((signed int)(x2 - x1));
   dy = abs((signed int)(y2 - y1));

   if(x1 > x2)
      addx = -1;
   if(y1 > y2)
      addy = -1;

   if(dx >= dy)
   {
      dy *= 2;
      P = dy - dx;
      diff = P - dx;

      for(; i<=dx; ++i)
      {
         nokia_plot(x1, y1, color);

         if(P < 0)
         {
            P  += dy;
            x1 += addx;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
   else
   {
      dx *= 2;
      P = dx - dy;
      diff = P - dy;

      for(; i<=dy; ++i)
      {
         nokia_plot(x1, y1, color);

         if(P < 0)
         {
            P  += dx;
            y1 += addy;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
}


void nokia_clrline(signed int x1,signed int y1,signed int x2, signed int y2)
{
   signed int dy, dx;
   signed int  addx=1, addy=1;
   signed int P, diff;

   signed int i=0;
   dx = abs((signed int)(x2 - x1));
   dy = abs((signed int)(y2 - y1));

   if(x1 > x2)
      addx = -1;
   if(y1 > y2)
      addy = -1;

   if(dx >= dy)
   {
      dy *= 2;
      P = dy - dx;
      diff = P - dx;

      for(; i<=dx; ++i)
      {
         nokia_clrplot(x1, y1);

         if(P < 0)
         {
            P  += dy;
            x1 += addx;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
   else
   {
      dx *= 2;
      P = dx - dy;
      diff = P - dy;

      for(; i<=dy; ++i)
      {
         nokia_clrplot(x1, y1);

         if(P < 0)
         {
            P  += dx;
            y1 += addy;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
}
/////////////////////////////////////////////////////////////////////////
//Draws a rectangle
//(x1, y1) - starting point
//(x2, y2) - end point
//fill  - yes/no
//color - on/off


void nokia_box(signed int x1, signed int y1, signed int x2, signed int y2, unsigned char fill, unsigned char color)
{
   if(fill)
   {
      signed int  i, xmin, xmax, ymin, ymax;

      if(x1 < x2)                            //Finds min and max X
      {
         xmin = x1;
         xmax = x2;
      }
      else
      {
         xmin = x2;
         xmax = x1;
      }

      if(y1 < y2)                            //Finds min and max Y
      {
         ymin = y1;
         ymax = y2;
      }
      else
      {
         ymin = y2;
         ymax = y1;
      }

      for(; xmin <= xmax; ++xmin)
      {
         for(i=ymin; i<=ymax; ++i)
         {
            nokia_plot(xmin, i, color);
         }
      }
   }
   else
   {
      nokia_line(x1, y1, x2, y1, color);      //Draws 4 sides
      nokia_line(x1, y2, x2, y2, color);
      nokia_line(x1, y1, x1, y2, color);
      nokia_line(x2, y1, x2, y2, color);
   }
}


void nokia_clrbox(signed int x1, signed int y1, signed int x2, signed int y2)
{
      nokia_clrline(x1, y1, x2, y1);      //Draws 4 sides
      nokia_clrline(x1, y2, x2, y2);
      nokia_clrline(x1, y1, x1, y2);
      nokia_clrline(x2, y1, x2, y2);
}
/////////////////////////////////////////////////////////////////////////
//Draws a bar
//(x1, y1) - starting point
//(x2, y2) - end point
//width  - number of pixel wide
//color - on/off
void nokia_bar(signed int x1, signed int y1, signed int x2, signed int y2, signed int width, unsigned char color)
{
   signed int half_width;
   signed int dy, dx;
   signed int  addx=1, addy=1, j;
   signed int P, diff, c1, c2;

   signed int i=0;
   dx = abs((signed int)(x2 - x1));
   dy = abs((signed int)(y2 - y1));

   half_width = width/2;
   c1 = -(dx*x1 + dy*y1);
   c2 = -(dx*x2 + dy*y2);

   if(x1 > x2)
   {
      unsigned char temp;
      temp = c1;
      c1 = c2;
      c2 = temp;
      addx = -1;
   }
   if(y1 > y2)
   {
      unsigned char temp;
      temp = c1;
      c1 = c2;
      c2 = temp;
      addy = -1;
   }

   if(dx >= dy)
   {
      P = 2*dy - dx;
      diff = P - dx;

      for(i=0; i<=dx; ++i)
      {
         for(j=-half_width; j<half_width+width%2; ++j)
         {
               nokia_plot(x1, y1+j, color);
         }
         if(P < 0)
         {
            P  += 2*dy;
            x1 += addx;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
   else
   {
      P = 2*dx - dy;
      diff = P - dy;

      for(i=0; i<=dy; ++i)
      {
         if(P < 0)
         {
            P  += 2*dx;
            y1 += addy;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
         for(j=-half_width; j<half_width+width%2; ++j)
         {
               nokia_plot(x1+j, y1, color);
         }
      }
   }
}
/////////////////////////////////////////////////////////////////////////
//Draws a circle
//(x,y) - center point
//radius - circle radius
//fill - yes/no
//color - on/off
void nokia_circle(signed int x, signed int y, signed int radius, unsigned char fill, unsigned char color)
{
   signed int a, b, P;

   a = 0;
   b = radius;
   P = 1 - radius;

   do
   {
      if(fill)
      {
         nokia_line(x-a, y+b, x+a, y+b, color);
         nokia_line(x-a, y-b, x+a, y-b, color);
         nokia_line(x-b, y+a, x+b, y+a, color);
         nokia_line(x-b, y-a, x+b, y-a, color);
      }
      else
      {
         nokia_plot(a+x, b+y, color);
         nokia_plot(b+x, a+y, color);
         nokia_plot(x-a, b+y, color);
         nokia_plot(x-b, a+y, color);
         nokia_plot(b+x, y-a, color);
         nokia_plot(a+x, y-b, color);
         nokia_plot(x-a, y-b, color);
         nokia_plot(x-b, y-a, color);
      }

      if(P < 0)
         P += 3 + 2 * a++;
      else
         P += 5 + 2 * (a++ - b--);
    } while(a <= b);
}
///////////////////////////////////////////////////////////////////////////////
//Shows a character string at [x,y]
void nokia_printstr(unsigned char* message,unsigned char color)
{
   for(; *message != '\0';message++){
      nokia_printchar(*message,color);
   }
}

void nokia_const_printstr(const char* message,unsigned char color)
{
   for(; *message != '\0';message++){
      nokia_printchar(*message,color);
   }
}
///////////////////////////////////////////////////////////////////////////////
//Draws a bitmap, origin (x,y), dimensions (xdim,ydim), align (LSB UP = 0)
void nokia_bitmap(const unsigned char *bitmap,unsigned char x, unsigned char y, unsigned char sx, unsigned char sy, unsigned char color) {
int cy=0;
int cx=0;
int _bit;
char _data;
     for (cy=0; cy<sy; cy++){
          _bit= cy % 8;
           for(cx=0; cx<sx; cx++){
                _data=bitmap[cx+((cy/8)*sx)];
                if ((_data & (1<<_bit))>0)
                    nokia_plot(x+cx, y+cy, color);
                else
                    nokia_plot(x+cx, y+cy,0);
           }
     }
}

