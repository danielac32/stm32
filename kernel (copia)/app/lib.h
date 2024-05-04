#define XINU_NULLPROCESS  1
#define XINU_PUTC 2
#define XINU_PIN_MODE 3
#define XINU_PIN_SET  4
#define XINU_PIN_GET  5
#define XINU_TEST1    6
#define XINU_TEST2    7

#define XINU_PRINTF    100
//#define XINU_PUTC    8
#define XINU_SLEEP    9
extern int __syscall(int a,...);

typedef struct
{
  unsigned int MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
  unsigned int OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
  unsigned int OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
  unsigned int PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  unsigned int IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
  unsigned int ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
  unsigned int BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
  //__IO uint16_t BSRRL;
  //__IO uint16_t BSRRH;
  unsigned int LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
  unsigned int AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} GPIO_TypeDef;

#define PERIPH_BASE           0x40000000UL
#define APB1PERIPH_BASE       PERIPH_BASE
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400UL)


#define GPIOx(no)               ((GPIO_TypeDef*)(GPIOA_BASE + (no) * (GPIOB_BASE-GPIOA_BASE)))
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