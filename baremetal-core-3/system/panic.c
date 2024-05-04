#include <xinu.h>

void	panic (
	 char	*msg			/* Message to display		*/
	)
{
	disable();			/* Disable interrupts		*/
	kprintf("\n\n\rpanic: %s\n\n", msg);
	hw_cfg_pin(GPIOx(GPIO_C),13,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);

	while(TRUE) {
		hw_set_pin(GPIOx(GPIO_C), 13, 1);//pin_set(pin_c13);
		sleep(50);
		hw_set_pin(GPIOx(GPIO_C), 13, 0);//pin_reset(pin_c13);
		sleep(50);
	}			/* Busy loop forever		*/
}