//#include <os.h>
#include <memory.h>
#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>

extern void _estack(void); // fake definition, will be filled in by linker script.

// Hang, let the watchdog reboot us.
// TODO(lvd): reset usart0 and report unexpected irq
void default_IRQ_Handler(void) {
    for (;;) {
            __WFE();
    }
}

// CM4 core fault handlers
void Reset_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void NonMaskableInt_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Reserved_3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void MemoryManagement_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Reserved_7_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Reserved_8_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Reserved_9_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Reserved_10_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SVCall_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DebugMonitor_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Reserved_13_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));

void WWDG_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                  
void PVD_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                   
void TAMP_STAMP_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));            
void RTC_WKUP_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));              
void FLASH_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                 
void RCC_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                   
void EXTI0_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                 
void EXTI1_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                 
void EXTI2_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                 
void EXTI3_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                 
void EXTI4_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                 
void DMA1_Stream0_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));          
void DMA1_Stream1_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));          
void DMA1_Stream2_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));          
void DMA1_Stream3_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));          
void DMA1_Stream4_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));          
void DMA1_Stream5_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));          
void DMA1_Stream6_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));          
void ADC_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                   
void CAN1_TX_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));               
void CAN1_RX0_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));              
void CAN1_RX1_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));              
void CAN1_SCE_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));              
void EXTI9_5_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));               
void TIM1_BRK_TIM9_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));         
void TIM1_UP_TIM10_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));         
void TIM1_TRG_COM_TIM11_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));    
void TIM1_CC_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));               
void TIM2_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                  
void TIM3_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                  
void TIM4_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                  
void I2C1_EV_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));               
void I2C1_ER_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));               
void I2C2_EV_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));               
void I2C2_ER_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));               
void SPI1_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                  
void SPI2_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                  
extern void USART1_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                
void USART2_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                
void USART3_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                
void EXTI15_10_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));             
void RTC_Alarm_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));             
void OTG_FS_WKUP_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));           
void TIM8_BRK_TIM12_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));        
void TIM8_UP_TIM13_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));         
void TIM8_TRG_COM_TIM14_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));    
void TIM8_CC_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));               
void DMA1_Stream7_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));          
void FSMC_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                  
void SDIO_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                  
void TIM5_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                  
void SPI3_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                  
void UART4_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                 
void UART5_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                 
void TIM6_DAC_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));              
void TIM7_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                  
void DMA2_Stream0_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));          
void DMA2_Stream1_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));          
void DMA2_Stream2_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));          
void DMA2_Stream3_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));          
void DMA2_Stream4_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));          
void CAN2_TX_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));               
void CAN2_RX0_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));              
void CAN2_RX1_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));              
void CAN2_SCE_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));              
void OTG_FS_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                
void DMA2_Stream5_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));          
void DMA2_Stream6_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));          
void DMA2_Stream7_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));          
void USART6_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                
void I2C3_EV_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));               
void I2C3_ER_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));               
void OTG_HS_EP1_OUT_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));        
void OTG_HS_EP1_IN_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));         
void OTG_HS_WKUP_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));           
void OTG_HS_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                
void RNG_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));                   
void FPU_Handler (void ) __attribute__((weak, alias("default_IRQ_Handler")));


__attribute__((section(".isr_vector"))) void (*vector_table[])(void) = {
    _estack,
    Reset_Handler,
    NonMaskableInt_Handler,
    Reserved_3_Handler,
    MemoryManagement_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    Reserved_7_Handler,
    Reserved_8_Handler,
    Reserved_9_Handler,
    Reserved_10_Handler,
    SVCall_Handler,
    DebugMonitor_Handler,
    Reserved_13_Handler,
    PendSV_Handler,
    SysTick_Handler,
    
	WWDG_Handler,                  
	PVD_Handler,                   
	TAMP_STAMP_Handler,            
	RTC_WKUP_Handler,              
	FLASH_Handler,                 
	RCC_Handler,                   
	EXTI0_Handler,                 
	EXTI1_Handler,                 
	EXTI2_Handler,                 
	EXTI3_Handler,                 
	EXTI4_Handler,                 
	DMA1_Stream0_Handler,          
	DMA1_Stream1_Handler,          
	DMA1_Stream2_Handler,          
	DMA1_Stream3_Handler,          
	DMA1_Stream4_Handler,          
	DMA1_Stream5_Handler,          
	DMA1_Stream6_Handler,          
	ADC_Handler,                   
	CAN1_TX_Handler,               
	CAN1_RX0_Handler,              
	CAN1_RX1_Handler,              
	CAN1_SCE_Handler,              
	EXTI9_5_Handler,               
	TIM1_BRK_TIM9_Handler,         
	TIM1_UP_TIM10_Handler,         
	TIM1_TRG_COM_TIM11_Handler,    
	TIM1_CC_Handler,               
	TIM2_Handler,                  
	TIM3_Handler,                  
	TIM4_Handler,                  
	I2C1_EV_Handler,               
	I2C1_ER_Handler,               
	I2C2_EV_Handler,               
	I2C2_ER_Handler,               
	SPI1_Handler,                  
	SPI2_Handler,                  
	USART1_Handler,                
	USART2_Handler,                
	USART3_Handler,                
	EXTI15_10_Handler,             
	RTC_Alarm_Handler,             
	OTG_FS_WKUP_Handler,           
	TIM8_BRK_TIM12_Handler,        
	TIM8_UP_TIM13_Handler,         
	TIM8_TRG_COM_TIM14_Handler,    
	TIM8_CC_Handler,               
	DMA1_Stream7_Handler,          
	FSMC_Handler,                  
	SDIO_Handler,                  
	TIM5_Handler,                  
	SPI3_Handler,                  
	UART4_Handler,                 
	UART5_Handler,                 
	TIM6_DAC_Handler,              
	TIM7_Handler,                  
	DMA2_Stream0_Handler,          
	DMA2_Stream1_Handler,          
	DMA2_Stream2_Handler,          
	DMA2_Stream3_Handler,          
	DMA2_Stream4_Handler,          
	CAN2_TX_Handler,               
	CAN2_RX0_Handler,              
	CAN2_RX1_Handler,              
	CAN2_SCE_Handler,              
	OTG_FS_Handler,                
	DMA2_Stream5_Handler,          
	DMA2_Stream6_Handler,          
	DMA2_Stream7_Handler,          
	USART6_Handler,                
	I2C3_EV_Handler,               
	I2C3_ER_Handler,               
	OTG_HS_EP1_OUT_Handler,        
	OTG_HS_EP1_IN_Handler,         
	OTG_HS_WKUP_Handler,           
	OTG_HS_Handler,                
	RNG_Handler,                   
	FPU_Handler,
};