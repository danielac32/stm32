#include <stdarg.h>
#define tx *(char *)0xFA002
 

void print(char *str){
  while(*str){
       tx=*str++;
  }
}


#define XPRINTF_USE_XFUNC_OUT      1   /* 1: Use floating point */
#define _CR_CRLF    1     /* 1: Convert \n ==> \r\n in the output char */
#define _USE_LONGLONG 1     /* 1: Enable long long integer in type "ll". */
#define _LONGLONG_t   long long /* Platform dependent long long integer type */
#define XPRINTF_USE_XFUNC_IN  1
#define _LINE_ECHO    1


void xputc (char c);
void xputs (const char* str);
void printf (const char* fmt, ...);
void sprintf (char* buff, const char* fmt, ...);
void put_dump (const void* buff, unsigned long addr, int len, int width);
#define DW_CHAR   sizeof(char)
#define DW_SHORT  sizeof(short)
#define DW_LONG   sizeof(long)

#define DEBUG_ASCII 0x01
#define DEBUG_HEX   0x02


static char *outptr;


void xputc (char c)
{
  //if (_CR_CRLF && c == '\n') xputc('\r');   /* CR -> CRLF */

 
  tx=c;
  
}
 


void xputs (          /* Put a string to the default device */
  const char* str       /* Pointer to the string */
)
{
  while (*str) {
    xputc(*str++);
  }
}

static
void xvprintf (
  const char* fmt,  /* Pointer to the format string */
  va_list arp     /* Pointer to arguments */
)
{
  unsigned int r, i, j, w, f;
  int n;
  char s[32], c, d, *p;
#if _USE_LONGLONG
  _LONGLONG_t v;
  unsigned _LONGLONG_t vs;
#else
  long v;
  unsigned long vs;
#endif


  for (;;) {
    c = *fmt++;         /* Get a format character */
    if (!c) break;        /* End of format? */
    if (c != '%') {       /* Pass it through if not a % sequense */
      xputc(c); continue;
    }
    f = w = 0;          /* Clear parms */
    c = *fmt++;         /* Get first char of the sequense */
    if (c == '0') {       /* Flag: left '0' padded */
      f = 1; c = *fmt++;
    } else {
      if (c == '-') {     /* Flag: left justified */
        f = 2; c = *fmt++;
      }
    }
    if (c == '*') {       /* Minimum width from an argument */
      n = va_arg(arp, int);
      if (n < 0) {      /* Flag: left justified */
        n = 0 - n;
        f = 2;
      }
      w = n; c = *fmt++;
    } else {
      while (c >= '0' && c <= '9') {  /* Minimum width */
        w = w * 10 + c - '0';
        c = *fmt++;
      }
    }
    if (c == 'l' || c == 'L') { /* Prefix: Size is long */
      f |= 4; c = *fmt++;
#if _USE_LONGLONG
      if (c == 'l' || c == 'L') { /* Prefix: Size is long long */
        f |= 8; c = *fmt++;
      }
#endif
    }
    if (!c) break;        /* End of format? */
    d = c;
    if (d >= 'a') d -= 0x20;
    switch (d) {        /* Type is... */
    case 'S' :          /* String */
      p = va_arg(arp, char*);
      for (j = 0; p[j]; j++) ;
      while (!(f & 2) && j++ < w) xputc(' ');
      xputs(p);
      while (j++ < w) xputc(' ');
      continue;
#if XPRINTF_USE_FLOAT
    case 'F' :          /* Float */
      {
        char a[48];
        //ftoa(a,48,precision?(precision-'0'):2,(double)va_arg( args, double ));
        ftoa((double)va_arg( arp, double ),a,48,3);
        p = a;
        while(*p)
          xputc(*p++);
        continue;
      }
#endif
    case 'C' :          /* Character */
      xputc((char)va_arg(arp, int)); continue;
    case 'B' :          /* Binary */
      r = 2; break;
    case 'O' :          /* Octal */
      r = 8; break;
    case 'D' :          /* Signed decimal */
    case 'U' :          /* Unsigned decimal */
      r = 10; break;
    case 'X' :          /* Hexdecimal */
      r = 16; break;
    default:          /* Unknown type (passthrough) */
      xputc(c); continue;
    }

#if XPRINTF_USE_FLOAT
#endif

    /* Get an argument and put it in numeral */
#if _USE_LONGLONG
    if (f & 8) {  /* long long argument? */
      v = va_arg(arp, _LONGLONG_t);
    } else {
      if (f & 4) {  /* long argument? */
        v = (d == 'D') ? (long)va_arg(arp, long) : (long)va_arg(arp, unsigned long);
      } else {    /* int/short/char argument */
        v = (d == 'D') ? (long)va_arg(arp, int) : (long)va_arg(arp, unsigned int);
      }
    }
#else
    if (f & 4) {  /* long argument? */
      v = va_arg(arp, long);
    } else {    /* int/short/char argument */
      v = (d == 'D') ? (long)va_arg(arp, int) : (long)va_arg(arp, unsigned int);
    }
#endif
    if (d == 'D' && v < 0) {  /* Negative value? */
      v = 0 - v; f |= 16;
    }
    i = 0; vs = v;
    do {
      d = (char)(vs % r); vs /= r;
      if (d > 9) d += (c == 'x') ? 0x27 : 0x07;
      s[i++] = d + '0';
    } while (vs != 0 && i < sizeof s);
    if (f & 16) s[i++] = '-';
    j = i; d = (f & 1) ? '0' : ' ';
    while (!(f & 2) && j++ < w) xputc(d);
    do xputc(s[--i]); while (i != 0);
    while (j++ < w) xputc(' ');
  }
}

void printf (     /* Put a formatted string to the default device */
  const char* fmt,  /* Pointer to the format string */
  ...         /* Optional arguments */
)
{
  va_list arp;


  va_start(arp, fmt);
  xvprintf(fmt, arp);
  va_end(arp);
}

void sprintf (      /* Put a formatted string to the memory */
  char* buff,     /* Pointer to the output buffer */
  const char* fmt,  /* Pointer to the format string */
  ...         /* Optional arguments */
)
{
  va_list arp;


  outptr = buff;    /* Switch destination for memory */

  va_start(arp, fmt);
  xvprintf(fmt, arp);
  va_end(arp);

  *outptr = 0;    /* Terminate output string with a \0 */
  outptr = 0;     /* Switch destination for device */
}


char *s="esta es una prueba";



long fib( long n) {
   long a = 0;
   long b = 1,i;
  for(  i = 1; i <= n; i += 1) {
     long nb = a + b;
    a = b;
    b = nb;
  }
  return a;
}

 
int main()
{


int i=0;
int b=0;
int a=1;
for ( i = 0; i < 10; ++i)
{
  printf("%s %d\n",s,i);

}
for ( i = 0; i < 10; ++i)
{
  printf("%d) fib: %d\n",i,fib(i) );
}



 b = a + (a + (a + (a + (a + (a + a)))));
 printf("%d\n",b );
  exit();
  while(1);
  return 0;
}
