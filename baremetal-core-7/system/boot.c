 
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
extern void main(void);                            // in main.c
extern void (*vector_table[])(void);               // in vector.c
extern char _sidata, _sdata, _edata, _sbss, _ebss; // provided by linker script



/*
static int setSysClockTo96MHz(){
    RCC->CR |= RCC_CR_HSEON;
    
    for (int i = 0; i < 5000; i++)
        if (RCC->CR & RCC_CR_HSERDY)
            break;

    if ((RCC->CR & RCC_CR_HSERDY) == 0)
        return 0; // HSE clock failed to become ready

    // Power control: Scale 1 mode <= 100 MHz
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    
    PWR->CR = (PWR->CR & ~PWR_CR_VOS) | ((3 << 14) & PWR_CR_VOS);
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE) | ((0 << 4) & RCC_CFGR_HPRE);
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE1) | ((4 << 10) & RCC_CFGR_PPRE1);
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE2) | ((0 << 13) & RCC_CFGR_PPRE2);
    RCC->PLLCFGR = RCC_PLLCFGR_PLLSRC;
    RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLM) | ((25 << 0) & RCC_PLLCFGR_PLLM);
    RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLN) | ((384 << 6) & RCC_PLLCFGR_PLLN);
    RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLP) | ((((4/2)-1) << 16) & RCC_PLLCFGR_PLLP);
    RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLQ) | ((8 << 24) & RCC_PLLCFGR_PLLQ);
    RCC->CR |= RCC_CR_PLLON;

    // Wait till PLL is ready
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
        __NOP();
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN;
    FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | ((3 << 0) & FLASH_ACR_LATENCY);
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | ((2 << 0) & RCC_CFGR_SW);
    
    while((RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SWS) | ((2 << 2) & RCC_CFGR_SWS))!=2)
        __NOP();

    return 1;
}*/

void systemInit(void) {
    irq_disable();
    _BCL(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN);
    _BCL(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
    _BCL(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
    _BCL(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);

    RCC->CR |= 1UL << 0;
    RCC->CFGR = 0;
    RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_PLLON | RCC_CR_HSEBYP);
    RCC->PLLCFGR = 0x24003010;   // Documented reset value
    RCC->CIR = 0;  // disable all rcc interrupts
    ready_preemptive=0;
    

 #if 1
    _BMD(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_3WS);
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
    #endif
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
 
    
    
    //irq_enable();
}



void Reset_Handler(void) {
    char* src = &_sidata;
    char* dst = &_sdata;

    while (dst < &_edata)
        *dst++ = *src++;

    for (dst = &_sbss; dst < &_ebss; dst++)
        *dst = 0;

    SCB->VTOR = (uintptr_t)&vector_table; /* Vector Table Relocation in Internal FLASH. */

    systemInit();




    meminit();
    RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;
    cdc_init_usbd();
    
    NVIC_EnableIRQ(OTG_FS_IRQn);
    //NVIC_SetPriority(OTG_FS_IRQn, 2);
    usbd_enable(&udev, true);
    usbd_connect(&udev, true);
    irq_enable();
    main();

    for (;;)
        __NOP(); // hang
}
