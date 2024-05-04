
#include <gpio.h>


sem_t sem_gpio;
#define GPIO_AFRLR(i)           ((i)>>3)
#define GPIO_AF_PINi(i,af)      ((af)<<(((i)&7)*GPIO_AF_BITS))
#define GPIO_AF_set(gpio,i,af)  ((gpio)->AFR[GPIO_AFRLR(i)] = \
                                ((gpio)->AFR[GPIO_AFRLR(i)] \
                                & ~GPIO_AF_PINi(i,GPIO_AF_MASK)) \
                                | GPIO_AF_PINi(i,af))

//--------------------------------------------
// configure operation mode of GPIO pin
void hw_cfg_pin(GPIO_TypeDef* gpioport, uint8 pin, uint16 gpiocfg)
{
    uint8 pin2 = 2 * pin;
    sem_wait(&sem_gpio);
    GPIO_AF_set(gpioport, pin, gpiocfg & GPIO_AF_MASK);
    gpioport->MODER   = (gpioport->MODER   & ~(3 << pin2)) | (((gpiocfg >> GPIOCFG_MODE_SHIFT  ) & 3) << pin2);
    gpioport->OSPEEDR = (gpioport->OSPEEDR & ~(3 << pin2)) | (((gpiocfg >> GPIOCFG_OSPEED_SHIFT) & 3) << pin2);
    gpioport->OTYPER  = (gpioport->OTYPER  & ~(1 << pin )) | (((gpiocfg >> GPIOCFG_OTYPE_SHIFT ) & 1) << pin );
    gpioport->PUPDR   = (gpioport->PUPDR   & ~(3 << pin2)) | (((gpiocfg >> GPIOCFG_PUPD_SHIFT  ) & 3) << pin2);
    sem_signal(&sem_gpio);
}

//--------------------------------------------
// set state of GPIO output pin
void hw_set_pin(GPIO_TypeDef* gpioport, uint8 pin, uint8 state)
{
  
    sem_wait(&sem_gpio);
    gpioport->ODR = (gpioport->ODR & ~(1 << pin)) | ((state & 1) << pin);
    sem_signal(&sem_gpio);
}


void hw_toggle_pin(GPIO_TypeDef* gpioport, uint8 pin)
{
    sem_wait(&sem_gpio);
    gpioport->ODR ^= (gpioport->ODR & ~(1 << pin)) | ((1 & 1) << pin);
    sem_signal(&sem_gpio);
}


//--------------------------------------------
// get state of GPIO input pin
uint8 hw_get_pin(GPIO_TypeDef* gpioport, uint8 pin)
{
    uint8 ret;
    sem_wait(&sem_gpio);
    ret = (gpioport->IDR & (1 << pin)) >> pin;
    sem_signal(&sem_gpio);
    return ret;
}