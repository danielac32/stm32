
#include <gpio.h>
#include <buttons.h>

struct Buttons{
  int press;
};
struct Buttons btn1;
struct Buttons btn2;
struct Buttons btn3;
struct Buttons btn4;
struct Buttons btn5;
struct Buttons btn6;


void init_buttons(){
hw_cfg_pin(GPIOx(GPIO_A),8,GPIOCFG_MODE_INP | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_PUP);

hw_cfg_pin(GPIOx(GPIO_C),5,GPIOCFG_MODE_INP | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_PUP);
hw_cfg_pin(GPIOx(GPIO_C),4,GPIOCFG_MODE_INP | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_PUP);
hw_cfg_pin(GPIOx(GPIO_A),7,GPIOCFG_MODE_INP | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_PUP);
hw_cfg_pin(GPIOx(GPIO_A),6,GPIOCFG_MODE_INP | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_PUP);

btn1.press=1;
btn2.press=1;
btn3.press=1;
btn4.press=1;
btn5.press=1;
btn6.press=1;
}
int get_a8(){
 if(hw_get_pin(GPIOx(GPIO_A),8)==0)btn1.press=0;
  if(hw_get_pin(GPIOx(GPIO_A),8)==1 && !btn1.press){ //seleccionar
     btn1.press=1;
     return 1;
  }
  return 0;
//return hw_get_pin(GPIOx(GPIO_A),8);
}
int get_joystick1(){
  if(hw_get_pin(GPIOx(GPIO_C),5)==0)btn2.press=0;
  if(hw_get_pin(GPIOx(GPIO_C),5)==1 && !btn2.press){ //seleccionar
     btn2.press=1;
     return 1;
  }
  return 0;

//return hw_get_pin(GPIOx(GPIO_C),5);
}
int get_joystick2(){
  if(hw_get_pin(GPIOx(GPIO_C),4)==0)btn3.press=0;
  if(hw_get_pin(GPIOx(GPIO_C),4)==1 && !btn3.press){ //seleccionar
     btn3.press=1;
     return 1;
  }
  return 0;
//return hw_get_pin(GPIOx(GPIO_C),4);
}
int get_joystick3(){
  if(hw_get_pin(GPIOx(GPIO_A),7)==0)btn4.press=0;
  if(hw_get_pin(GPIOx(GPIO_A),7)==1 && !btn4.press){ //seleccionar
     btn4.press=1;
     return 1;
  }
  return 0;
//return hw_get_pin(GPIOx(GPIO_A),7);
}
int get_joystick4(){
  if(hw_get_pin(GPIOx(GPIO_A),6)==0)btn5.press=0;
  if(hw_get_pin(GPIOx(GPIO_A),6)==1 && !btn5.press){ //seleccionar
     btn5.press=1;
     return 1;
  }
  return 0;
//return hw_get_pin(GPIOx(GPIO_A),6);
}
