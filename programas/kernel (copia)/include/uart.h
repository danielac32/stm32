

#include "rp2040.h"
#include "kernel.h"
#ifndef UART_H_
#define UART_H_


void uart_init(unsigned int id, unsigned int baudrate);
unsigned int uart_read(unsigned int id, char *buff, unsigned int len);
unsigned int uart_write(unsigned int id, char *buff, unsigned int len);
void uart_intr_enable(unsigned int id, unsigned int rx, unsigned int tx);
void uart_flush(unsigned int id);
void uart_deinit(unsigned int id);
#define UART_RX0_BUFFER_SIZE 64

#define UART_RX0_BUFFER_MASK (UART_RX0_BUFFER_SIZE - 1)
#define UART_FRAME_ERROR      0x0800             
#define UART_OVERRUN_ERROR    0x0400            
#define UART_BUFFER_OVERFLOW  0x0200           
#define UART_NO_DATA          0x0100   

uint8_t UART_RxBuf[UART_RX0_BUFFER_SIZE];

uint8_t UART_TxHead;
uint8_t UART_TxTail;
uint8_t UART_RxHead;
uint8_t UART_RxTail;
uint8_t UART_LastRxError;
uint16_t uart0_available(void);
uint16_t uart0_getc(void);
void irq_uart0();
uint8_t readBytes(char *buffer,uint8_t length);


#define XPRINTF_USE_XFUNC_OUT      1   /* 1: Use floating point */
#define	_CR_CRLF		1	    /* 1: Convert \n ==> \r\n in the output char */
#define	_USE_LONGLONG	1	    /* 1: Enable long long integer in type "ll". */
#define	_LONGLONG_t		long long	/* Platform dependent long long integer type */
#define XPRINTF_USE_XFUNC_IN	1
#define	_LINE_ECHO		1


void xputc (char c);
void xputs (const char* str);
void printf (const char* fmt, ...);
void sprintf (char* buff, const char* fmt, ...);
void put_dump (const void* buff, unsigned long addr, int len, int width);
#define DW_CHAR		sizeof(char)
#define DW_SHORT	sizeof(short)
#define DW_LONG		sizeof(long)

#define DEBUG_ASCII 0x01
#define DEBUG_HEX   0x02

void hexdump(void *buffer, uint32 length, bool8 text);

#endif