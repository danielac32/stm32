 
//#include <os.h>
#include <memory.h>
#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
#include <usbapi.h>

int ready_preemptive=0;
void Reset_Handler(void);
void systemInit(void);
extern void _estack(void); // fake definition, will be filled in by linker script.



// Hang, let the watchdog reboot us.
// TODO(lvd): reset usart0 and report unexpected irq
void default_IRQ_Handler(void) {
    for (;;) {
            __WFE();
    }
}

// CM4 core fault handlers
void Reset_Handler(void);
void NonMaskableInt_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Reserved_3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void MemoryManagement_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Reserved_7_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Reserved_8_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Reserved_9_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Reserved_10_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
//extern void __attribute__((naked)) SVC_Handler(void);// __attribute__((weak, alias("default_IRQ_Handler")));
void DebugMonitor_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Reserved_13_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
extern void  __attribute__((naked)) PendSV_Handler(void);// __attribute__((weak, alias("default_IRQ_Handler")));
extern void SysTick_Handler(void);

// STM32F411 IRQ Handlers
void WWDG_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void PVD_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TAMP_STAMP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RTC_WKUP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FLASH_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RCC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI0_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream0_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream5_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream6_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void ADC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_19_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_20_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_21_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_22_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI9_5_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_BRK_TIM9_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_UP_TIM10_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_TRG_COM_TIM11_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_CC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
extern void TIM2_IRQHandler(void);// __attribute__((weak, alias("default_IRQ_Handler")));
void TIM3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C1_EV_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C1_ER_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C2_EV_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C2_ER_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void USART1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void USART2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_39_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI15_10_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RTC_Alarm_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void OTG_FS_WKUP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_43_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_44_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_45_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_46_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream7_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_48_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SDIO_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM5_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_52_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_53_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_54_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_55_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream0_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_61_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_62_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_63_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_64_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_65_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_66_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
extern void OTG_FS_IRQHandler(void);//__attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream5_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream6_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream7_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void USART6_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C3_EV_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C3_ER_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_74_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_75_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_76_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_77_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_78_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_79_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_80_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FPU_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_82_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RESERVED_83_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI5_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
/*extern void svccall_handler(uint32 *sp);
static void _svc_asm2(){
    asm volatile(
            ".global svccall_handler\n"
            //"push {lr}\n"
            "MRS r0,PSP\n"
            "B svccall_handler\n"
        );
}*/


extern void SVC_Handler(void);
__attribute__ ((section(".isr_vector"))) void (*vector_table[])(void) = {
    0x2001f400,
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
    SVC_Handler,
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
    RESERVED_19_Handler,
    RESERVED_20_Handler,
    RESERVED_21_Handler,
    RESERVED_22_Handler,
    EXTI9_5_Handler,
    TIM1_BRK_TIM9_Handler,
    TIM1_UP_TIM10_Handler,
    TIM1_TRG_COM_TIM11_Handler,
    TIM1_CC_Handler,
    TIM2_IRQHandler,
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
    RESERVED_39_Handler,
    EXTI15_10_Handler,
    RTC_Alarm_Handler,
    OTG_FS_WKUP_Handler,
    RESERVED_43_Handler,
    RESERVED_44_Handler,
    RESERVED_45_Handler,
    RESERVED_46_Handler,
    DMA1_Stream7_Handler,
    RESERVED_48_Handler,
    SDIO_Handler,
    TIM5_Handler,
    SPI3_Handler,
    RESERVED_52_Handler,
    RESERVED_53_Handler,
    RESERVED_54_Handler,
    RESERVED_55_Handler,
    DMA2_Stream0_Handler,
    DMA2_Stream1_Handler,
    DMA2_Stream2_Handler,
    DMA2_Stream3_Handler,
    DMA2_Stream4_Handler,
    RESERVED_61_Handler,
    RESERVED_62_Handler,
    RESERVED_63_Handler,
    RESERVED_64_Handler,
    RESERVED_65_Handler,
    RESERVED_66_Handler,
    OTG_FS_IRQHandler,
    DMA2_Stream5_Handler,
    DMA2_Stream6_Handler,
    DMA2_Stream7_Handler,
    USART6_Handler,
    I2C3_EV_Handler,
    I2C3_ER_Handler,
    RESERVED_74_Handler,
    RESERVED_75_Handler,
    RESERVED_76_Handler,
    RESERVED_77_Handler,
    RESERVED_78_Handler,
    RESERVED_79_Handler,
    RESERVED_80_Handler,
    FPU_Handler,
    RESERVED_82_Handler,
    RESERVED_83_Handler,
    SPI4_Handler,
    SPI5_Handler,
};







void systemInit(void) {
    irq_disable();
    ready_preemptive=0;

    //SCB->VTOR = (uintptr_t)&vector_table; /* Vector Table Relocation in Internal FLASH. */
    //SCB->VTOR = FLASH_BASE | 0x00;
    _BMD(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_2WS);
    /* setting up PLL 16MHz HSI, VCO=144MHz, PLLP = 72MHz PLLQ = 48MHz  */
    _BMD(RCC->PLLCFGR,
        RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLSRC | RCC_PLLCFGR_PLLQ | RCC_PLLCFGR_PLLP,
        _VAL2FLD(RCC_PLLCFGR_PLLM, 8) | _VAL2FLD(RCC_PLLCFGR_PLLN, 72) | _VAL2FLD(RCC_PLLCFGR_PLLQ, 3));
    /* enabling PLL */
    _BST(RCC->CR, RCC_CR_PLLON);
    _WBS(RCC->CR, RCC_CR_PLLRDY);
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
        __NOP();
    /* switching to PLL */
    _BMD(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
    _WVL(RCC->CFGR, RCC_CFGR_SWS, RCC_CFGR_SWS_PLL);
    
    /* enabling GPIOA and setting PA11 and PA12 to AF10 (USB_FS) */
    _BST(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
    _BST(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
    _BST(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);

    _BST(GPIOA->AFR[1], (0x0A << 12) | (0x0A << 16));
    _BMD(GPIOA->MODER, (0x03 << 22) | (0x03 << 24), (0x02 << 22) | (0x02 << 24));
    
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) { /*  */ }

    SysTick->LOAD = STK_LOAD_RELOAD;                                         // max value
    SysTick->VAL  = 0;                                                       // reset
    SysTick->CTRL = STK_CTRL_CLKSOURCE | STK_CTRL_TICKINT | STK_CTRL_ENABLE; // enable at AHB == 96MHz
    
    UART_TxHead = 0;
    UART_TxTail = 0;
    UART_RxHead = 0;
    UART_RxTail = 0;
 
    
    meminit();
    //RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;
    cdc_init_usbd();
    NVIC_EnableIRQ(OTG_FS_IRQn);
    //NVIC_SetPriority(OTG_FS_IRQn, 2);
    usbd_enable(&udev, true);
    usbd_connect(&udev, true);
    
    irq_enable();
}





extern char  _sidata;
/* start address for the .data section. defined in linker script */  
extern char  _sdata;
/* end address for the .data section. defined in linker script */
extern char  _edata;
/* start address for the .bss section. defined in linker script */
extern char  _sbss;
/* end address for the .bss section. defined in linker script */
extern char  _ebss;
/* stack used for SystemInit_ExtMemCtl; always internal RAM used */
extern int main();







void Reset_Handler(void){
    unsigned int* psrc = &etext;    // start at the end of .text, where .data is loaded in flash
    unsigned int* pdest = &data;    // copy to start of .data, where it is linked to in ram
    
    while (pdest < &edata) {
        // copy the value from flash to ram
        *pdest = *psrc;
        ++psrc; 
        ++pdest;
    }

    pdest = &bss;       // start at the beginning of .bss in ram
    while (pdest < &ebss)
    {
        // set each value to zero
        *pdest = 0x0;
        ++pdest;
    }
    
    SCB->VTOR = (uintptr_t)&vector_table; /* Vector Table Relocation in Internal FLASH. */

    systemInit();
    //SysTick->LOAD = STK_LOAD_RELOAD;                                         // max value
    //SysTick->VAL  = 0;                                                       // reset
    //SysTick->CTRL = STK_CTRL_CLKSOURCE | STK_CTRL_TICKINT | STK_CTRL_ENABLE; // enable at AHB == 96MHz
    main();
    for (;;)
        __NOP(); // hang
}


