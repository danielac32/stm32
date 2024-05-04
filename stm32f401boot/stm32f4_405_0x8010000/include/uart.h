#include <stm32.h>
#include <stdint.h>



#define USART1_DIV_M       45
// USARTDIV_Fraction = 0.573 / (1 / 16) = 0.573 / 0.0625 ~= 9
#define USART1_DIV_F       9

#define PORT_TX        GPIO_A   // PA9  --> RXD
#define PIN_TX         9
#define PORT_RX        GPIO_A   // PA10 <-- TXD
#define PIN_RX         10

void uartinit();
void putcharuart(char ch);
char getcharuart(void);
int putchar(int data);
int getchar(void);




#define UART_EN		13 
#define UART_INTR_RX	5
#define UART_INTR_TCIE	6
#define UART_INTR_TX	7
#define UART_TX_EN 	3
#define UART_RX_EN 	2

#define PARITY		12

/* Status register */
#define UART_RXNE	0x20
#define UART_TC		0x40
#define UART_TC_TXE	0xC0

#define UART_TEST	6

#define UART_FIFO_SIZE	1
#define UARTBUF 256
#define ECHO 1

#define UART_RX0_BUFFER_SIZE 64
#define UART_RX0_BUFFER_MASK (UART_RX0_BUFFER_SIZE - 1)
#define UART_FRAME_ERROR      0x0800             
#define UART_OVERRUN_ERROR    0x0400            
#define UART_BUFFER_OVERFLOW  0x0200           
#define UART_NO_DATA          0x0100   

extern uint8_t UART_RxBuf[];
extern uint8_t UART_TxHead;
extern uint8_t UART_TxTail;
extern uint8_t UART_RxHead;
extern uint8_t UART_RxTail;
extern uint8_t UART_LastRxError;
extern uint16_t usart_available(void);
extern uint16_t usart_getc(void);
extern uint8_t usart_readBytes(char *buffer,uint8_t length);

