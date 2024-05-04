
#include "uart.h"
#include <stdarg.h>
#include "kernel.h"


struct uart_hw {
	unsigned int dr;
	unsigned int rsr;
	unsigned int unused1[4];
	unsigned int fr;
	unsigned int unuded2;
	unsigned int ilpr;
	unsigned int ibrd;
	unsigned int fbrd;
	unsigned int lcr_h;
	unsigned int cr;
	unsigned int ifls;
	unsigned int imsc;
	unsigned int ris;
	unsigned int mis;
	unsigned int icr;
};

#define uart0 ((volatile struct uart_hw*)UART0_BASE)
#define uart1 ((volatile struct uart_hw*)UART1_BASE)

#define DR_OE		(1 << 11)
#define DR_BE		(1 << 10)
#define DR_PE		(1 << 9)
#define DR_FE		(1 << 8)
#define DR_DATA		(0xFF)

#define RSR_OE		(1 << 3)
#define RSR_BE		(1 << 2)
#define RSR_PE		(1 << 1)
#define RSR_FE		(1 << 0)

#define FR_RI		(1 << 8)
#define FR_TXFE		(1 << 7)
#define FR_RXFF		(1 << 6)
#define FR_TXFF		(1 << 5)
#define FR_RXFE		(1 << 4)
#define FR_BUSY		(1 << 3)
#define FR_DCD		(1 << 2)
#define FR_DSR		(1 << 1)
#define FR_CTS		(1 << 0)

#define LCR_SPS		(1 << 7)
#define LCR_WLEN_5	(0 << 5)
#define LCR_WLEN_6	(1 << 5)
#define LCR_WLEN_7	(2 << 5)
#define LCR_WLEN_8	(3 << 5)
#define LCR_FEN		(1 << 4)
#define LCR_STP2	(1 << 3)
#define LCR_EPS		(1 << 2)
#define LCR_PEN		(1 << 1)
#define LCR_BRK		(1 << 0)

#define CR_CTSEN	(1 << 15)
#define CR_RTSEN	(1 << 14)
#define CR_OUT2		(1 << 13)
#define CR_OUT1		(1 << 12)
#define CR_RTS		(1 << 11)
#define CR_DTR		(1 << 10)
#define CR_RXE		(1 << 9)
#define CR_TXE		(1 << 8)
#define CR_LBE		(1 << 7)
#define CR_SIRLP	(1 << 2)
#define CR_SIREN	(1 << 1)
#define CR_UARTEN	(1 << 0)

#define IFLS_RXIFSEL	(7 << 3)
#define IFLS_TXIFSEL	(7 << 0)

#define IMSC_OE		(1 << 10)
#define IMSC_BE		(1 << 9)
#define IMSC_PE		(1 << 8)
#define IMSC_FE		(1 << 7)
#define IMSC_RT		(1 << 6)
#define IMSC_TX		(1 << 5)
#define IMSC_RX		(1 << 4)
#define IMSC_DSR	(1 << 3)
#define IMSC_DCD	(1 << 2)
#define IMSC_CTS	(1 << 1)
#define IMSC_RIM	(1 << 0)

// RIS, MIS, ICR



void uart_init(unsigned int id, unsigned int baudrate) {
	volatile struct uart_hw *uart;

	if (id == 0)
		uart = uart0;
	else
		uart = uart1;

	(void)baudrate;

	uart->cr = 0;
	uart->ibrd = 6;
	uart->fbrd = 33;
	uart->lcr_h = LCR_WLEN_8 | LCR_FEN;
	uart->imsc = 0;
	uart->icr = 0xFFFFFFFF;
	uart->cr = CR_TXE | CR_RXE | CR_UARTEN;
}

unsigned int uart_read(unsigned int id, char *buff, unsigned int len) {
	unsigned int i;
	volatile struct uart_hw *uart;

	if (id == 0)
		uart = uart0;
	else
		uart = uart1;

	for (i = 0; i < len; ++i) {
		if (uart->fr & FR_RXFE)
			break;

		buff[i] = uart->dr & DR_DATA;
	}
	return i;
}

unsigned int uart_write(unsigned int id, char *buff, unsigned int len) {
	unsigned int i;
	volatile struct uart_hw *uart;

	if (id == 0)
		uart = uart0;
	else
		uart = uart1;

	for (i = 0; i < len; ++i) {
		while (uart->fr & FR_TXFF)
			asm volatile ("nop");

		uart->dr = buff[i];
	}
	return i;
}

void uart_intr_enable(unsigned int id, unsigned int rx, unsigned int tx) {
	volatile struct uart_hw *uart;

	if (id == 0)
		uart = uart0;
	else
		uart = uart1;

	uart->icr = 0xFFFFFFFF;

	if (rx)
		uart->imsc |= IMSC_RT;
	else
		uart->imsc &= ~(IMSC_RT);

	if (tx)
		uart->imsc |= IMSC_TX;
	else
		uart->imsc &= ~(IMSC_TX);
}

void uart_flush(unsigned int id) {
	volatile struct uart_hw *uart;

	if (id == 0)
		uart = uart0;
	else
		uart = uart1;

	while (!(uart->fr & FR_TXFE))
		asm volatile ("nop");
}

void uart_deinit(unsigned int id) {
	volatile struct uart_hw *uart;

	if (id == 0)
		uart = uart0;
	else
		uart = uart1;

	uart->cr = 0;
	uart->imsc = 0;
}


static char *outptr;


void xputc (char c)
{
	//if (_CR_CRLF && c == '\n') xputc('\r');		/* CR -> CRLF */

 
	uart_write(0, &c, 1);
	
}
 


void xputs (					/* Put a string to the default device */
	const char* str				/* Pointer to the string */
)
{
	while (*str) {
		xputc(*str++);
	}
}

static
void xvprintf (
	const char*	fmt,	/* Pointer to the format string */
	va_list arp			/* Pointer to arguments */
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
		c = *fmt++;					/* Get a format character */
		if (!c) break;				/* End of format? */
		if (c != '%') {				/* Pass it through if not a % sequense */
			xputc(c); continue;
		}
		f = w = 0;					/* Clear parms */
		c = *fmt++;					/* Get first char of the sequense */
		if (c == '0') {				/* Flag: left '0' padded */
			f = 1; c = *fmt++;
		} else {
			if (c == '-') {			/* Flag: left justified */
				f = 2; c = *fmt++;
			}
		}
		if (c == '*') {				/* Minimum width from an argument */
			n = va_arg(arp, int);
			if (n < 0) {			/* Flag: left justified */
				n = 0 - n;
				f = 2;
			}
			w = n; c = *fmt++;
		} else {
			while (c >= '0' && c <= '9') {	/* Minimum width */
				w = w * 10 + c - '0';
				c = *fmt++;
			}
		}
		if (c == 'l' || c == 'L') {	/* Prefix: Size is long */
			f |= 4; c = *fmt++;
#if _USE_LONGLONG
			if (c == 'l' || c == 'L') {	/* Prefix: Size is long long */
				f |= 8; c = *fmt++;
			}
#endif
		}
		if (!c) break;				/* End of format? */
		d = c;
		if (d >= 'a') d -= 0x20;
		switch (d) {				/* Type is... */
		case 'S' :					/* String */
			p = va_arg(arp, char*);
			for (j = 0; p[j]; j++) ;
			while (!(f & 2) && j++ < w) xputc(' ');
			xputs(p);
			while (j++ < w) xputc(' ');
			continue;
#if XPRINTF_USE_FLOAT
		case 'F' :					/* Float */
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
		case 'C' :					/* Character */
			xputc((char)va_arg(arp, int)); continue;
		case 'B' :					/* Binary */
			r = 2; break;
		case 'O' :					/* Octal */
			r = 8; break;
		case 'D' :					/* Signed decimal */
		case 'U' :					/* Unsigned decimal */
			r = 10; break;
		case 'X' :					/* Hexdecimal */
			r = 16; break;
		default:					/* Unknown type (passthrough) */
			xputc(c); continue;
		}

#if XPRINTF_USE_FLOAT
#endif

		/* Get an argument and put it in numeral */
#if _USE_LONGLONG
		if (f & 8) {	/* long long argument? */
			v = va_arg(arp, _LONGLONG_t);
		} else {
			if (f & 4) {	/* long argument? */
				v = (d == 'D') ? (long)va_arg(arp, long) : (long)va_arg(arp, unsigned long);
			} else {		/* int/short/char argument */
				v = (d == 'D') ? (long)va_arg(arp, int) : (long)va_arg(arp, unsigned int);
			}
		}
#else
		if (f & 4) {	/* long argument? */
			v = va_arg(arp, long);
		} else {		/* int/short/char argument */
			v = (d == 'D') ? (long)va_arg(arp, int) : (long)va_arg(arp, unsigned int);
		}
#endif
		if (d == 'D' && v < 0) {	/* Negative value? */
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

void printf (			/* Put a formatted string to the default device */
	const char*	fmt,	/* Pointer to the format string */
	...					/* Optional arguments */
)
{
	va_list arp;


	va_start(arp, fmt);
	xvprintf(fmt, arp);
	va_end(arp);
}

void sprintf (			/* Put a formatted string to the memory */
	char* buff,			/* Pointer to the output buffer */
	const char*	fmt,	/* Pointer to the format string */
	...					/* Optional arguments */
)
{
	va_list arp;


	outptr = buff;		/* Switch destination for memory */

	va_start(arp, fmt);
	xvprintf(fmt, arp);
	va_end(arp);

	*outptr = 0;		/* Terminate output string with a \0 */
	outptr = 0;			/* Switch destination for device */
}

/*----------------------------------------------*/
/* Dump a line of binary dump                   */
/*----------------------------------------------*/

void put_dump (
	const void* buff,		/* Pointer to the array to be dumped */
	unsigned long addr,		/* Heading address value */
	int len,				/* Number of items to be dumped */
	int width				/* Size of the items (DF_CHAR, DF_SHORT, DF_LONG) */
)
{
	int i;
	const unsigned char *bp;
	const unsigned short *sp;
	const unsigned long *lp;


	printf("%08lX ", addr);		/* address */

	switch (width) {
	case DW_CHAR:
		bp = buff;
		for (i = 0; i < len; i++)		/* Hexdecimal dump */
			printf(" %02X", bp[i]);
		xputc(' ');
		for (i = 0; i < len; i++)		/* ASCII dump */
			xputc((unsigned char)((bp[i] >= ' ' && bp[i] <= '~') ? bp[i] : '.'));
		break;
	case DW_SHORT:
		sp = buff;
		do								/* Hexdecimal dump */
			printf(" %04X", *sp++);
		while (--len);
		break;
	case DW_LONG:
		lp = buff;
		do								/* Hexdecimal dump */
			printf(" %08LX", *lp++);
		while (--len);
		break;
	}

	xputc('\n');
}

static void hexdump_print(byte, byte);
static void hexdump_print(
	 byte	data,			/* Item to print		*/
	 byte	mode			/* ASCII or hex mode		*/
	)
{
    switch (mode)
    {
    case DEBUG_ASCII:
        data = (' ' <= data && data <= '~') ? data : '.';
        printf( "%c", data);
        break;
    case DEBUG_HEX:
        printf( "%02x ", data);
        break;
    default:
        break;
    }
}


void	hexdump(
	 void	*buffer,		/* Addresss of memory area	*/
	 uint32	length,			/* Length in bytes		*/
	 bool8	canon			/* Print in ASCII or hex	*/
	)
{
    uint32 m, n, remain;

    byte *b = (byte *)buffer;

    for (n = 0; n < length; n += 0x10) {
        printf( "%08x ", (uint32)buffer + n);

        remain = length - n;

        for (m = 0; m < remain && m < 0x10; m++) {
            if (m % 0x08 == 0) {
                printf( " ");
            }
            hexdump_print(b[n + m], DEBUG_HEX);
        }

        /* Pad the rest if needed */
        if (remain < 0x10) {
            for (m = 0; m < 0x10 - remain; m++) {
                if ((0 != m) && (0 == m % 0x08)) {
                    printf( " ");
                }
                printf( "   ");
            }
        }

        if (canon == TRUE) {
            printf( " |");
            for (m = 0; m < remain && m < 0x10; m++) {
                hexdump_print(b[n + m], DEBUG_ASCII);
            }
            printf( "|");
        }
        printf( "\n");
    }
}



uint16_t uart0_available(void)
{
  uint16_t ret;

 
    ret = (UART_RX0_BUFFER_SIZE + UART_RxHead - UART_RxTail) & UART_RX0_BUFFER_MASK;
 
  return ret;
} // uart0_available 

uint16_t uart0_getc(void)
{
  uint16_t tmptail;
  uint8_t data;
 
    if (UART_RxHead == UART_RxTail) {
      return UART_NO_DATA;   // no data available 
    }
  

  // calculate / store buffer index 
  tmptail = (UART_RxTail + 1) & UART_RX0_BUFFER_MASK;

  UART_RxTail = tmptail;

 // get data from receive buffer 
  data = UART_RxBuf[tmptail];

  return (UART_LastRxError << 8) + data;

} // uart0_getc 





void irq_uart0() {
    uint16_t tmphead;
    uint8_t data;
   
    uint8_t lastRxError;
 
	uint32_t mask=disable();

    uart_read(0, &data, 1);
	 tmphead = (UART_RxHead + 1) & UART_RX0_BUFFER_MASK;

    if (tmphead == UART_RxTail) {
        //error: receive buffer overflow 
        lastRxError = UART_BUFFER_OVERFLOW >> 8;
    } else {
        // store new index 
        UART_RxHead = tmphead;
        // store received data in buffer 
        UART_RxBuf[tmphead] = data;
    }
    UART_LastRxError = lastRxError;

	restore(mask);
}



static void delay2(long t) {
  while (t--)
    asm volatile ("nop");
}

uint8_t readBytes(char *buffer,uint8_t length){
  uint8_t count = 0;
  while (count < length) {
    int c = uart0_getc();
    if (c < 0) break;
    *buffer++ = (char)c;
    count++;
    delay2(5);
  }
  return count;
}