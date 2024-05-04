/* gpio.c
 * (c) Tom Trebisky  9-24-2016
 * (c) Tom Trebisky  11-20-2020
 *
 * Basic GPIO driver for the F411
 *
 * Also includes LED routines
 */

#include <stdint.h>
#include "f411.h"





/* Where is the LED ?? */
#define LED_PIN		13	/* PC13 */
#define LED_GPIO	GPIOC	/* PC13 */

/* Here is the F411 gpio structure.
 *  very different from the F103.
 */
struct gpio {
	volatile unsigned int mode;	/* 0x00 */
	volatile unsigned int otype;	/* 0x04 */
	volatile unsigned int ospeed;	/* 0x08 */
	volatile unsigned int pupd;	/* 0x0c */
	volatile unsigned int idata;	/* 0x10 */
	volatile unsigned int odata;	/* 0x14 */
	volatile unsigned int bsrr;	/* 0x18 */
	volatile unsigned int lock;	/* 0x1c */
    volatile unsigned int afr[2];  /* 0x20 */
  //volatile unsigned int afl;  /* 0x20 */
  //volatile unsigned int afh;  /* 0x24 */
};

#define GPIOA_BASE	(struct gpio *) 0x40020000
#define GPIOB_BASE	(struct gpio *) 0x40020400
#define GPIOC_BASE	(struct gpio *) 0x40020800

static struct gpio *gpio_bases[] = {
    GPIOA_BASE, GPIOB_BASE, GPIOC_BASE
};


/**********************************************************************/
// GPIO by port number (A=0, B=1, ..)
#define GPIOx(no)               ((struct gpio*)(GPIOA_BASE + (no) * (GPIOB_BASE-GPIOA_BASE)))
#define GPIO_A                  0
#define GPIO_B                  1
#define GPIO_C                  2
#define GPIO_D                  3
#define GPIO_E                  4
#define GPIO_F                  5
#define GPIO_G                  6

// GPIOCFG macros
#define GPIO_AF_BITS            4       // width of bit field
#define GPIO_AF_MASK            0x000F  // mask in AFR[0/1]
#define GPIO_AF0_MCO            0
#define GPIO_AF1_TIM1           1
#define GPIO_AF4_I2C1           4
#define GPIO_AF4_I2C2           4
#define GPIO_AF4_I2C3           4
#define GPIO_AF5_SPI1           5
#define GPIO_AF5_SPI2           5
#define GPIO_AF6_SPI3           6
#define GPIO_AF7_USART1         7
#define GPIO_AF11_ETH           11
#define GPIO_AF12_FSMC          12
#define GPIO_AF12_SDIO          12
#define GPIO_AF13_DCMI          13

#define GPIOCFG_MODE_SHIFT      4
#define GPIOCFG_MODE_MASK      (3 << GPIOCFG_MODE_SHIFT)
#define GPIOCFG_MODE_INP       (0 << GPIOCFG_MODE_SHIFT)    // 00: Input (reset state)
#define GPIOCFG_MODE_OUT       (1 << GPIOCFG_MODE_SHIFT)    // 01: General purpose output mode
#define GPIOCFG_MODE_ALT       (2 << GPIOCFG_MODE_SHIFT)    // 10: Alternate function mode
#define GPIOCFG_MODE_ANA       (3 << GPIOCFG_MODE_SHIFT)    // 11: Analog mode
#define GPIOCFG_OTYPE_SHIFT     6
#define GPIOCFG_OTYPE_MASK     (1 << GPIOCFG_OTYPE_SHIFT)
#define GPIOCFG_OTYPE_PUPD     (0 << GPIOCFG_OTYPE_SHIFT)   // 0: Output push-pull (reset state)
#define GPIOCFG_OTYPE_OPEN     (1 << GPIOCFG_OTYPE_SHIFT)   // 1: Output open-drain
#define GPIOCFG_OSPEED_SHIFT    7
#define GPIOCFG_OSPEED_MASK    (3 << GPIOCFG_OSPEED_SHIFT)
#define GPIOCFG_OSPEED_LOW     (0 << GPIOCFG_OSPEED_SHIFT)  // 00: Low speed       (   2 MHz)
#define GPIOCFG_OSPEED_MEDIUM  (1 << GPIOCFG_OSPEED_SHIFT)  // 01: Medium speed    (  25 MHz)
#define GPIOCFG_OSPEED_HIGH    (2 << GPIOCFG_OSPEED_SHIFT)  // 10: High speed      (  50 MHz)
#define GPIOCFG_OSPEED_VHIGH   (3 << GPIOCFG_OSPEED_SHIFT)  // 11: Very high speed ( 100 Mhz)
#define GPIOCFG_PUPD_SHIFT      9
#define GPIOCFG_PUPD_MASK      (3 << GPIOCFG_PUPD_SHIFT)
#define GPIOCFG_PUPD_NONE      (0 << GPIOCFG_PUPD_SHIFT)    // 00: No pull-up, pull-down
#define GPIOCFG_PUPD_PUP       (1 << GPIOCFG_PUPD_SHIFT)    // 01: Pull-up
#define GPIOCFG_PUPD_PDN       (2 << GPIOCFG_PUPD_SHIFT)    // 10: Pull-down
#define GPIOCFG_PUPD_RFU       (3 << GPIOCFG_PUPD_SHIFT)    // 11: Reserved

// IRQ triggers
#define GPIO_IRQ_MASK           0x38
#define GPIO_IRQ_FALLING        0x20
#define GPIO_IRQ_RISING         0x28
#define GPIO_IRQ_CHANGE         0x30

// configure operation mode of GPIO pin
void hw_cfg_pin(struct gpio* gpioport, uint8_t pin, uint16_t gpiocfg);

// set state of GPIO output pin
void hw_set_pin(struct gpio* gpioport, uint8_t pin, uint8_t state);

// get state of GPIO input pin
uint8_t hw_get_pin(struct gpio* gpioport, uint8_t pin);



#define GPIO_AFRLR(i)           ((i)>>3)
#define GPIO_AF_PINi(i,af)      ((af)<<(((i)&7)*GPIO_AF_BITS))
#define GPIO_AF_set(gpio,i,af)  ((gpio)->afr[GPIO_AFRLR(i)] = \
                                ((gpio)->afr[GPIO_AFRLR(i)] \
                                & ~GPIO_AF_PINi(i,GPIO_AF_MASK)) \
                                | GPIO_AF_PINi(i,af))

//--------------------------------------------
// configure operation mode of GPIO pin
void hw_cfg_pin(struct gpio* gpioport, uint8_t pin, uint16_t gpiocfg)
{
    uint8_t pin2 = 2 * pin;

    GPIO_AF_set(gpioport, pin, gpiocfg & GPIO_AF_MASK);
    gpioport->mode   = (gpioport->mode   & ~(3 << pin2)) | (((gpiocfg >> GPIOCFG_MODE_SHIFT  ) & 3) << pin2);
    gpioport->ospeed = (gpioport->ospeed & ~(3 << pin2)) | (((gpiocfg >> GPIOCFG_OSPEED_SHIFT) & 3) << pin2);
    gpioport->otype  = (gpioport->otype  & ~(1 << pin )) | (((gpiocfg >> GPIOCFG_OTYPE_SHIFT ) & 1) << pin );
    gpioport->pupd   = (gpioport->pupd   & ~(3 << pin2)) | (((gpiocfg >> GPIOCFG_PUPD_SHIFT  ) & 3) << pin2);
}

//--------------------------------------------
// set state of GPIO output pin
void hw_set_pin(struct gpio* gpioport, uint8_t pin, uint8_t state)
{
    gpioport->odata = (gpioport->odata & ~(1 << pin)) | ((state & 1) << pin);
}

//--------------------------------------------
// get state of GPIO input pin
uint8_t hw_get_pin(struct gpio* gpioport, uint8_t pin)
{
    return (gpioport->idata & (1 << pin)) >> pin;
}

/******************************************************************************/
/* Change alternate function setting for a pin
 * These are 4 bit fields. All initially 0.
 */
void
gpio_af ( int gpio, int pin, int val )
{
	struct gpio *gp;
	int shift;

	gp = gpio_bases[gpio];

	if ( pin < 8 ) {
	    shift = pin * 4;
	    gp->afr[0] &= ~(0xf<<shift);
	    gp->afr[0] |= val<<shift;
	} else {
	    shift = (pin-8) * 4;
	    gp->afr[1] &= ~(0xf<<shift);
	    gp->afr[1] |= val<<shift;
	}
}

/* This is a 2 bit field */
void
gpio_mode ( int gpio, int pin, int val )
{
	struct gpio *gp;
	int shift;

	gp = gpio_bases[gpio];
	shift = pin * 2;
	gp->mode &= ~(0x3<<shift);
	gp->mode |= val<<shift;
}

/* kludge for now */
void
gpio_uart ( int gpio, int pin, int val )
{
	struct gpio *gp;
	int shift;

	/* XXX ignores val */
	gp = gpio_bases[gpio];

	gp->otype &= ~(1<<pin);

	shift = pin * 2;
	gp->ospeed |= (3<<shift);

	gp->pupd &= ~(3<<shift);
}

/*
ra=GET32(GPIOA_mode);
    ra&=~(3<<4); //PA2
    ra&=~(3<<6); //PA3
    ra|=2<<4; //PA2
    ra|=2<<6; //PA3
    PUT32(GPIOA_mode,ra);

    ra=GET32(GPIOA_otype);
    ra&=~(1<<2); //PA2
    ra&=~(1<<3); //PA3
    PUT32(GPIOA_otype,ra);

    ra=GET32(GPIOA_ospeed);
    ra|=3<<4; //PA2
    ra|=3<<6; //PA3
    PUT32(GPIOA_ospeed,ra);

    ra=GET32(GPIOA_pupd);
    ra&=~(3<<4); //PA2
    ra&=~(3<<6); //PA3
    PUT32(GPIOA_pupd,ra);
 */

/* Note that UART1 can be moved around a lot.
 * I make a choice here.
 * I suppose a general interface would allow this to
 *  be selected via a call argument.
 */
void
gpio_uart_init ( int uart )
{
	if ( uart == UART1 ) {
	    hw_cfg_pin(GPIOx(GPIO_A),     11,     GPIOCFG_MODE_ALT | 8 | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
        hw_cfg_pin(GPIOx(GPIO_A),     12,     GPIOCFG_MODE_ALT | 8 | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);


	} else if ( uart == UART2 ) {
	    gpio_af ( GPIOA, 2, 7 );	/* Tx */
	    gpio_af ( GPIOA, 3, 7 );	/* Rx */
	} else { /* UART3 */
	    gpio_af ( GPIOC, 6, 7 );	/* Tx */
	    gpio_af ( GPIOC, 7, 7 );	/* Rx */
	}
}

/* ========================================================== */

static struct gpio *led_gp;
static unsigned long on_mask;
static unsigned long off_mask;

void
led_init ( void )
{
	int conf;
	int shift;
	int pin = LED_PIN;

	// led_gp = GPIOC_BASE;
	led_gp = gpio_bases[LED_GPIO];

	shift = pin * 2;
	led_gp->mode &= ~(3<<shift);
	led_gp->mode |= (1<<shift);
	led_gp->otype &= ~(1<<pin);

	off_mask = 1 << pin;
	on_mask = 1 << (pin+16);
}

void
led_on ( void )
{
	led_gp->bsrr = on_mask;
}

void
led_off ( void )
{
	led_gp->bsrr = off_mask;
}

/* THE END */
