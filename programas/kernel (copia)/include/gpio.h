#include "rp2040.h" 
#ifndef GPIO_H_
#define GPIO_H_


#define GPIO_FUNC_XIP			0
#define GPIO_FUNC_SPI			1
#define GPIO_FUNC_UART			2
#define GPIO_FUNC_I2C			3
#define GPIO_FUNC_PWM			4
#define GPIO_FUNC_SIO			5
#define GPIO_FUNC_PIO0			6
#define GPIO_FUNC_PIO1			7
#define GPIO_FUNC_CLK			8
#define GPIO_FUNC_USB			9
#define GPIO_FUNC_NULL			0xF

#define GPIO_INT_LEVEL_LOW		1
#define GPIO_INT_LEVEL_HIGH		2
#define GPIO_INT_EDGE_FALL		4
#define GPIO_INT_EDGE_RISE		8

#define GPIO_OUT 1
#define GPIO_IN 0


void gpio_init(unsigned int  gpio, unsigned int  func);
void gpio_dir(unsigned int  gpio, unsigned int  out);
void gpio_set(unsigned int  gpio, unsigned int  value);
unsigned int  gpio_get(unsigned int  gpio);
void gpio_deinit(unsigned int  gpio);
void gpio_xor(unsigned int  gpio);
void gpio_pullup(unsigned int  gpio, unsigned int  enable);
void gpio_pulldown(unsigned int  gpio, unsigned int  enable);

void gpio_int_set(unsigned int  gpio, unsigned int  enable, unsigned int  event);
void gpio_int_ack(unsigned int  gpio);
#endif