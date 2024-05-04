
#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
#include <delay.h>
#include <kernel.h>
#include <delay.h>
#include <gpio.h>
#include <uart.h>



uint8_t UART_RxBuf[UART_RX0_BUFFER_SIZE];
uint8_t UART_TxHead;
uint8_t UART_TxTail;
uint8_t UART_RxHead;
uint8_t UART_RxTail;
uint8_t UART_LastRxError;




void uartinit(){
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    // IO port A clock enable
//    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    hw_cfg_pin(GPIOx(PORT_TX), PIN_TX, GPIOCFG_MODE_ALT | 7 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(PORT_RX), PIN_RX, GPIOCFG_MODE_ALT | 7 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    USART1->CR1 &= ~ (1 << UART_EN);

    USART1->BRR = (USART1_DIV_M << 4) | USART1_DIV_F;
    // OVER8 = 0: oversampling by 16
    // TE = 1: Transmitter is enabled
    // RE = 1: Receiver is enabled and begins searching for a start bit
    // UE = 1: USART enable
    USART1->CR1 |= (1 << UART_INTR_RX) | (1 << UART_TX_EN) | (1 << UART_RX_EN);
    USART1->CR1 |= (1 << UART_EN);
    NVIC_EnableIRQ(USART1_IRQn);
    UART_TxHead = 0;
    UART_TxTail = 0;
    UART_RxHead = 0;
    UART_RxTail = 0;
}

void putcharuart(char ch)
{
	while (!(USART1->SR & USART_SR_TXE));
	USART1->DR = ch;
}

//--------------------------------------------
char getcharuart(void)
{
	char ch;

	while (!(USART1->SR & USART_SR_RXNE));
	ch = USART1->DR;
	return ch;
}


//--------------------------------------------
int putchar(int data)
{
	putcharuart((char)data);
	return data;
}

//--------------------------------------------
int getchar(void)
{
	return (int)getcharuart();
}


uint16_t usart_available(void){
    uint16_t ret;
    ret = (UART_RX0_BUFFER_SIZE + UART_RxHead - UART_RxTail) & UART_RX0_BUFFER_MASK;
    return ret;
}


uint16_t usart_getc(void){
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
} 


uint8_t usart_readBytes(char *buffer,uint8_t length){
  uint8_t count = 0;
  while (count < length) {
    int c = usart_getc();
    if (c < 0) break;
    *buffer++ = (char)c;
    count++;
    DELAY(10);
  }
  return count;
}



void USART1_Handler(){
	char ch;
	uint16_t tmphead;
    uint8_t data;
    uint8_t lastRxError;

    asm volatile("cpsid i");
	if(USART1->SR & USART_SR_RXNE){
		ch = USART1->DR;
		tmphead = (UART_RxHead + 1) & UART_RX0_BUFFER_MASK;
	    if (tmphead == UART_RxTail) {
	        //error: receive buffer overflow 
	        lastRxError = UART_BUFFER_OVERFLOW >> 8;
	    } else {
	        // store new index 
	        UART_RxHead = tmphead;
	        // store received data in buffer 
	        UART_RxBuf[tmphead] = ch;
	    }
	    UART_LastRxError = lastRxError;
		//putchar(ch);
		//kprintf("aquiii\n");
	}
	asm volatile("cpsie i");
}