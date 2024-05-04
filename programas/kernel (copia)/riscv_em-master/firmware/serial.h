#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>


#ifndef SERIAL_H
#define SERIAL_H


 void putc_( char ch);
 void print( char *str);
 void reverse(char *str,int len);
 int intToStr(int32_t x,char str[],int d);
 float _pow_(float base,float ex);
 void ftoa(float n,char *res,int afterpoint);
static const unsigned long dv[] = {
//  4294967296      // 32 bit unsigned max
    1000000000,     // +0
     100000000,     // +1
      10000000,     // +2
       1000000,     // +3
        100000,     // +4
//       65535      // 16 bit unsigned max     
         10000,     // +5
          1000,     // +6
           100,     // +7
            10,     // +8
             1,     // +9
};
static void xtoa(unsigned long x, const unsigned long *dp);
static void puth(unsigned n);
void print_hex(unsigned int val,int digits);
int myprintf(const char *format, ...);
int mysprintf(char *buff,const char *format, ...);
void floatprint(float f);
#endif