
#include "serial.h"
#include <string.h>

char spbuffer[50];
char buff[50];
#define reg_uart_data (*(volatile uint32_t*)0x300000)
void putc_( char ch){
     reg_uart_data=ch;
     /*register uint32_t a7 asm("a7")=24;
     register uint32_t a0 asm("a0")=ch;   //syscall event cpu                      
     asm("ecall"); */
} 

void print( char *str){
  while(*str){
    putc_(*str++);
  }
}

 void reverse(char *str,int len){
  int i=0,j=len-1,temp;
  while(i<j){
    temp = str[i];
    str[i]=str[j];
    str[j]=temp;
    i++;
    j--;
  }
}

 int intToStr(int32_t x,char str[],int d){
  int32_t i=0;
  while(x){
    str[i++]=(x%10)+'0';
    x=x/10;
  }
  while(i<d){
    str[i++]='0';
  }
  reverse(str,i);
  str[i]='\0';
  return i;
}
 float _pow_(float base,float ex){
  if(ex==0){
    return 1;
  }else if(ex<0){
    return 1/_pow_(base,-ex);
  }else if((int)ex %2 ==0){
    float half__pow_=_pow_(base,ex/2);
    return half__pow_ * half__pow_;
  }else{
    return base * _pow_(base,ex-1);
  }
}
void ftoa(float n,char *res,int afterpoint){
  int ipart=(int)n;
  float fpart = n -(float)ipart;
  int i= intToStr(ipart,res,0);
  if(afterpoint!=0){
     res[i]='.';
     fpart=fpart * _pow_(10,afterpoint);
     intToStr((int)fpart,res+i+1,afterpoint);
  }
}


#define PUTC(c) putc_(c)
static void xtoa(unsigned long x, const unsigned long *dp)
{
    char c;
    unsigned long d;
    if(x) {
        while(x < *dp) ++dp;
        do {
            d = *dp++;
            c = '0';
            while(x >= d) ++c, x -= d;
            PUTC(c);
        } while(!(d & 1));
    } else
        PUTC('0');
}

static void puth(unsigned n)
{
    static const char hex[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    PUTC(hex[n & 15]);
}
 
void print_hex(unsigned int val,int digits){
for (int i = (4*digits)-4; i >=0; i-=4)
{
    PUTC("0123456789ABCDEF"[(val >> i) % 16]);
}
}
 
void floatprint(float f){
  ftoa(f,buff,2);
  myprintf("%s",buff);
}
int myprintf(const char *format, ...)
{
    char c;
    int i,count;
    long n;
    int fill_zeros;
    unsigned d;
    float val;
    int hex;
    int nu=2;
    ///char buff[80];
    va_list a;
    va_start(a, format);
    while((c = *format++)) {
        if(c == '%') {
            fill_zeros = 0;
            parse_fmt_char:
            switch(c = *format++) {
                case 'f':
                    val =va_arg(a, double);

                    break;
                case 's':                       // String
                    //io_puts_no_newline(va_arg(a, char*));
                    print(va_arg(a, char*));
                    break;
                case 'c':                       // Char
                    PUTC(va_arg(a, int)); // TODO: 'char' generated a warning
                    break;
                case 'd':
                case 'i':                       // 16 bit Integer
                case 'u':                       // 16 bit Unsigned
                    i = va_arg(a, int);
                    if(c == 'i' && i < 0) i = -i, PUTC('-');
                    xtoa((unsigned)i, dv + 5);
                    break;
                case 'l':                       // 32 bit Long
                case 'n':                       // 32 bit uNsigned loNg
                    n = va_arg(a, long);
                    if(c == 'l' &&  n < 0) n = -n, PUTC('-');
                    xtoa((unsigned long)n, dv);
                    break;
                /*case 'x':                       // 16 bit heXadecimal
                    i = va_arg(a, int);
                    d = i >> 12;
                    if (d > 0 || fill_zeros >= 4)
                        puth(d);
                    d = i >> 8;
                    if (d > 0 || fill_zeros >= 3)
                        puth(d);
                    d = i >> 4;
                    if (d > 0 || fill_zeros >= 2)
                        puth(d);
                    puth(i);
                    break;*/
                case 'x':    
                     {
                     // if(c!='x')nu=(int)c-'0';
                     //nu=2;
                      hex = va_arg(a, int);
                      
                      print_hex(hex,nu);
                     }                   // 16 bit heXadecimal
                    
                    break;
                case '2':
                case '4':
                case '8':
                   nu=(int)c-'0';
                   goto parse_fmt_char;
                break;

                case '0':
                    c = *format++;
                    fill_zeros = c - '0';
                    goto parse_fmt_char;
                case 0: return 0;
                default: goto bad_fmt;
            }
        } else
bad_fmt:    PUTC(c);
    }
    va_end(a);
    return 0; // TODO: return number of chars printed
}

int mysprintf( char *buff,const char *format, ...){

    char c;
    int i,count;
    long n;
    int fill_zeros;
    unsigned d;
    float val;

    //
     char *str;
     char ch;
     int dec; 
     long ln;
     int hex;
     double temp;
     char b[10];
    //
    int nu=2;
    memset(buff,0,(sizeof(buff)));

    va_list a;
    va_start(a, format);
    while((c = *format++)) {
        if(c == '%') {
            parse_fmt_char:
            switch(c = *format++) {
                case 'f':                       // String
                    {
                      //print("str ");
                      temp=va_arg(a, double);
                      //ftoa((float)temp,b,2);
                      //myprintf("%s",b);
                    }
                    break; 
                case 's':                       // String
                    {
                      //print("str ");
                      str=va_arg(a, char*);
                      while(*str){
                        *buff++=*str++;
                      }
                      *buff++='\0';
                    }
                    break;
                case 'c':                       // Char
                    {
                      //print("char ");
                      ch=va_arg(a, int);
                      *buff++=ch;
                      *buff++='\0';
                    }
                    break;
                case 'i':                       // 16 bit Integer
                case 'u':                       // 16 bit Unsigned
                    {
                      char n[20];
                      char *p;
                      dec = va_arg(a, int);
                      itoa(dec,n,10);
                      p=n;
                      while(*p)*buff++=*p++;
                    }
                    //if(c == 'i' && i < 0) i = -i, PUTC('-');
                    //xtoa((unsigned)i, dv + 5);
                    break;
                case 'l':                       // 32 bit Long
                case 'n':                       // 32 bit uNsigned loNg
                    {
                      char n[20];
                      char *p;
                      ln = va_arg(a, long);
                      itoa(ln,n,10);
                      p=n;
                      while(*p)*buff++=*p++;
                    }
                    //if(c == 'l' &&  n < 0) n = -n, PUTC('-');
                    //xtoa((unsigned long)n, dv);
                    break;
                case 'x':    
                     {
                     // if(c!='x')nu=(int)c-'0';
                     //nu=2;
                      hex = va_arg(a, int);
                      
                      for (int i = (4*nu)-4; i >=0; i-=4){
                           *buff++= "0123456789ABCDEF"[(hex >> i) % 16];
                      }
                      //*buff++='\0';
                     }                   // 16 bit heXadecimal
                    
                    break;
                case '2':
                case '4':
                case '8':
                   nu=(int)c-'0';
                   goto parse_fmt_char;
                break;
                default:// PUTC(c);
                   // nu=(int)c-'0';
                    //goto _else;
                break;
            }
        } else{
            //PUTC(c);
          _else:
          *buff++=c;
        }
    }
    *buff++='\0';
    va_end(a);
    return 0; // TODO: return number of chars printed
}