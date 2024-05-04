#ifndef __MAIN_H
#define __MAIN_H
 

#include <stdarg.h>
//#include <stdio.h>
#include "stm32f401xc.h"
#define MHZ                   *1000000UL

#define UART_BAUDRATE(FCLK, P_SPEED)    (((FCLK) + ((P_SPEED)/2U)) / (P_SPEED))

 
  /* MACRO to emulate printf() via UART */
//#define uprintf(...) for(char _b[160]; snprintf(_b, sizeof(_b), __VA_ARGS__), u_puts(_b), 0;){}


 
 

__STATIC_INLINE void init_sys(void) {

  RCC->AHB1ENR = (
    1 * RCC_AHB1ENR_GPIOAEN          | /*  0x00000001                                            */
    0 * RCC_AHB1ENR_GPIOBEN          | /*  0x00000002                                            */
    1 * RCC_AHB1ENR_GPIOCEN          | /*  0x00000004                                            */
    0 * RCC_AHB1ENR_GPIODEN          | /*  0x00000008                                            */
    0 * RCC_AHB1ENR_GPIOEEN          | /*  0x00000010                                            */
    0 * RCC_AHB1ENR_GPIOHEN          | /*  0x00000080                                            */
    0 * RCC_AHB1ENR_CRCEN            | /*  0x00001000                                            */
    0 * RCC_AHB1ENR_DMA1EN           | /*  0x00200000                                            */
    0 * RCC_AHB1ENR_DMA2EN             /*  0x00400000                                            */
  );

  RCC->AHB2ENR = (
    0 * RCC_AHB2ENR_OTGFSEN           /*  0x00000080                                             */
  );

  RCC->APB1ENR = (
    0 * RCC_APB1ENR_TIM2EN           | /*  0x00000001                                            */
    0 * RCC_APB1ENR_TIM3EN           | /*  0x00000002                                            */
    0 * RCC_APB1ENR_TIM4EN           | /*  0x00000004                                            */
    0 * RCC_APB1ENR_TIM5EN           | /*  0x00000008                                            */
    0 * RCC_APB1ENR_WWDGEN           | /*  0x00000800                                            */
    0 * RCC_APB1ENR_SPI2EN           | /*  0x00004000                                            */
    0 * RCC_APB1ENR_SPI3EN           | /*  0x00008000                                            */
    0 * RCC_APB1ENR_USART2EN         | /*  0x00020000                                            */
    0 * RCC_APB1ENR_I2C1EN           | /*  0x00200000                                            */
    0 * RCC_APB1ENR_I2C2EN           | /*  0x00400000                                            */
    0 * RCC_APB1ENR_I2C3EN           | /*  0x00800000                                            */
    1 * RCC_APB1ENR_PWREN              /*  0x10000000                                            */
  );

  RCC->APB2ENR = (
    0 * RCC_APB2ENR_TIM1EN           | /*  0x00000001                                            */
    0 * RCC_APB2ENR_USART1EN         | /*  0x00000010                                            */
    1 * RCC_APB2ENR_USART6EN         | /*  0x00000020                                            */
    0 * RCC_APB2ENR_ADC1EN           | /*  0x00000100                                            */
    0 * RCC_APB2ENR_SDIOEN           | /*  0x00000800                                            */
    1 * RCC_APB2ENR_SPI1EN           | /*  0x00001000                                            */
    0 * RCC_APB2ENR_SPI4EN           | /*  0x00002000                                            */
    0 * RCC_APB2ENR_SYSCFGEN         | /*  0x00004000                                            */
    0 * RCC_APB2ENR_TIM9EN           | /*  0x00010000                                            */
    0 * RCC_APB2ENR_TIM10EN          | /*  0x00020000                                            */
    0 * RCC_APB2ENR_TIM11EN            /*  0x00040000                                            */
  );

  /*  HSE / M * N / P                                                                            */

  /*  25Mhz / 15 * 120 / 2 = 100Mhz                                                              */

  RCC->PLLCFGR = (
                                                                                                 /* 
       PLLM: Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
            Caution:The software has to set these bits correctly to ensure that the VCO
            input frequency ranges from 1 to 2 MHz. It is recommended to select
            a frequency of 2 MHz to limit PLL jitter. 
       ================================================================================          */

    0 * RCC_PLLCFGR_PLLM             | /*  0x0000003F                                            */
    1 * RCC_PLLCFGR_PLLM_0           | /*    0x00000001                                          */
    1 * RCC_PLLCFGR_PLLM_1           | /*    0x00000002                                          */
    1 * RCC_PLLCFGR_PLLM_2           | /*    0x00000004                                          */
    1 * RCC_PLLCFGR_PLLM_3           | /*    0x00000008                                          */
    0 * RCC_PLLCFGR_PLLM_4           | /*    0x00000010                                          */
    0 * RCC_PLLCFGR_PLLM_5           | /*    0x00000020                                          */
                                                                                                 /*
       PLLN: Main PLL (PLL) multiplication factor for VCO                                        
            Caution:The software has to set these bits correctly to ensure that                  
            the VCO output frequency is between 192 and 432 MHz.                                 
       ================================================================================          */

    0 * RCC_PLLCFGR_PLLN             | /*  0x00007FC0                                            */
    0 * RCC_PLLCFGR_PLLN_0           | /*    0x00000040                                          */
    0 * RCC_PLLCFGR_PLLN_1           | /*    0x00000080                                          */
    0 * RCC_PLLCFGR_PLLN_2           | /*    0x00000100                                          */
    1 * RCC_PLLCFGR_PLLN_3           | /*    0x00000200                                          */
    1 * RCC_PLLCFGR_PLLN_4           | /*    0x00000400                                          */
    1 * RCC_PLLCFGR_PLLN_5           | /*    0x00000800                                          */
    1 * RCC_PLLCFGR_PLLN_6           | /*    0x00001000                                          */
    0 * RCC_PLLCFGR_PLLN_7           | /*    0x00002000                                          */
    0 * RCC_PLLCFGR_PLLN_8           | /*    0x00004000                                          */
                                                                                                 /*
       PLLP: Main PLL (PLL) division factor for main system clock
            PLL output clock frequency = VCO frequency / PLLP with PLLP = 2, 4, 6, or 8
            00: PLLP = 2
            01: PLLP = 4
            10: PLLP = 6
            11: PLLP = 8
       ================================================================================          */

    0 * RCC_PLLCFGR_PLLP             | /*  0x00030000                                            */
    0 * RCC_PLLCFGR_PLLP_0           | /*    0x00010000                                          */
    0 * RCC_PLLCFGR_PLLP_1           | /*    0x00020000                                          */
                                                                                                 /*
       PLLSRC: Main PLL(PLL) and audio PLL (PLLI2S) entry clock source
            Set and cleared by software to select PLL and PLLI2S clock source.
            This bit can be written only when PLL and PLLI2S are disabled.
            0: HSI clock selected as PLL and PLLI2S clock entry
            1: HSE oscillator clock selected as PLL and PLLI2S clock entry                       
       ================================================================================          */

    1 * RCC_PLLCFGR_PLLSRC_HSE       | /*  0x00400000                                            */
    0 * RCC_PLLCFGR_PLLSRC_HSI       | /*                                                        */
                                                                                                 /*
       PLLQ: Main PLL (PLL) division factor for USB OTG FS, SDIO and RNG clocks                  
       ================================================================================          */

    0 * RCC_PLLCFGR_PLLQ             | /*  0x0F000000                                            */
    0 * RCC_PLLCFGR_PLLQ_0           | /*    0x01000000                                          */
    0 * RCC_PLLCFGR_PLLQ_1           | /*    0x02000000                                          */
    0 * RCC_PLLCFGR_PLLQ_2           | /*    0x04000000                                          */
    0 * RCC_PLLCFGR_PLLQ_3             /*    0x08000000                                          */
  );

  RCC->CR = (
    RCC_CR_HSEON                     | /*  Switch HSE ON                                         */
    RCC_CR_PLLON                       /*  Switch PLL ON                                         */
  );                                                                                             /*

    After system reset, the RTC registers are protected against parasitic write access with the
    DBP bit of the PWR power control register (PWR_CR). The DBP bit must be set to enable
    RTC registers write access.                                                                  */

  PWR->CR = PWR_CR_VOS_1 | PWR_CR_DBP; /*  Enable Backup Domain Access (leave VOS default)       */
  RCC->BDCR = (
    RCC_BDCR_LSEON                   | /*  LSE oscillator clock used as RTC clock                */
    RCC_BDCR_RTCSEL_0                | /*  LSE oscillator clock used as RTC clock                */
    RCC_BDCR_RTCEN                     /*  Enable RTC                                            */
  );

  // while (!(RCC->BDCR & RCC_BDCR_LSERDY)) { /*  */ }

  // RCC->BDCR = (
  //   RCC_BDCR_LSEON                   | /*  LSE oscillator clock used as RTC clock                */
  //   1 * RCC_BDCR_RTCSEL_0            | /*  LSE oscillator clock used as RTC clock                */
  //   1 * RCC_BDCR_RTCEN                 /*  Enable RTC                                            */
  // );

                                                                                                 /*
    After backup domain reset, all the RTC registers are write-protected. Writing to the RTC
    registers is enabled by writing a key into the Write Protection register, RTC_WPR.
    
    The following steps are required to unlock the write protection on all the RTC registers
    except for RTC_ISR[13:8], RTC_TAFCR, and RTC_BKPxR.

      1. Write �0xCA� into the RTC_WPR register.
      2. Write �0x53� into the RTC_WPR register.                                                 */

  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;                                                                               /*

    To program the initial time and date calendar values, including the time format and the
    prescaler configuration, the following sequence is required:

      1. Set INIT bit to 1 in the RTC_ISR register to enter initialization mode. In this mode, 
         the calendar counter is stopped and its value can be updated.

      2. Poll INITF bit of in the RTC_ISR register. The initialization phase mode is entered 
         when INITF is set to 1. It takes from 1 to 2 RTCCLK clock cycles (due to clock
         synchronization).

      3. To generate a 1 Hz clock for the calendar counter, program first the synchronous
         prescaler factor in RTC_PRER register, and then program the asynchronous prescaler
         factor. Even if only one of the two fields needs to be changed, 2 separate write
         accesses must be performed to the RTC_PRER register.

      4. Load the initial time and date values in the shadow registers (RTC_TR and RTC_DR),
         and configure the time format (12 or 24 hours) through the FMT bit in the RTC_CR
         register.

      5. Exit the initialization mode by clearing the INIT bit. The actual calendar counter 
         value is then automatically loaded and the counting restarts after 4 RTCCLK clock 
         cycles.                                                                                 */

  RTC->ISR = ~RTC_ISR_WUTF;            /* clear WUTF flag, set INIT flag                         */
  while(!(RTC->ISR & RTC_ISR_INITF)) { /*  */ }

  RTC->PRER = UINT8_MAX;               /*  program first the synchronous prescaler factor        */
  RTC->PRER = (                        /*  then program the asynchronous prescaler factor        */
    RTC_PRER_PREDIV_A                | /*  0x007F0000                                            */
    UINT8_MAX                          /*  0x000000FF                                            */
  );
                                                                                               
  //                                                                                                
  // RTC->CALIBR = (                                                                                
  //   0 * RTC_CALIBR_DCS               | /*  0x00000080                                            */
  //   0 * RTC_CALIBR_DC                  /*  0x0000001F                                            */
  // );                                                                                             
  //                                                                                                
  // RTC->CALR = (                                                                                  
  //                                                                                                
  //   0 * RTC_CALR_CALP                | /*  0x00008000                                            */
  //   0 * RTC_CALR_CALW8               | /*    0x00004000                                          */
  //   0 * RTC_CALR_CALW16              | /*    0x00002000                                          */
  //                                                                                                
  //   0 * RTC_CALR_CALM                | /*  0x000001FF                                            */
  //   0 * RTC_CALR_CALM_0              | /*    0x00000001                                          */
  //   0 * RTC_CALR_CALM_1              | /*    0x00000002                                          */
  //   0 * RTC_CALR_CALM_2              | /*    0x00000004                                          */
  //   0 * RTC_CALR_CALM_3              | /*    0x00000008                                          */
  //   0 * RTC_CALR_CALM_4              | /*    0x00000010                                          */
  //   0 * RTC_CALR_CALM_5              | /*    0x00000020                                          */
  //   0 * RTC_CALR_CALM_6              | /*    0x00000040                                          */
  //   0 * RTC_CALR_CALM_7              | /*    0x00000080                                          */
  //   0 * RTC_CALR_CALM_8                /*    0x00000100                                          */
  // );

  if (!(RTC->ISR & RTC_ISR_INITS)) {
    RTC->TR = 0x025034; 
    RTC->DR = 0x191124;
  }

  RTC->CR = 0;                                                                                   /*

          When the wakeup timer is disabled, wait for WUTWF=1 before enabling it again.          */

  while(!(RTC->ISR & RTC_ISR_WUTWF));
    
  RTC->WUTR = 2047;

  RTC->CR = (
    0 * RTC_CR_COE                   | /*  0x00800000                                            */
                                                                                                 
    0 * RTC_CR_OSEL                  | /*  0x00600000                                            */
    0 * RTC_CR_OSEL_0                | /*    0x00200000                                          */
    0 * RTC_CR_OSEL_1                | /*    0x00400000                                          */
                                                                                                 
    0 * RTC_CR_POL                   | /*  0x00100000                                            */
    1 * RTC_CR_COSEL                 | /*  0x00080000                                            */
    0 * RTC_CR_BKP                   | /*  0x00040000                                            */
    0 * RTC_CR_SUB1H                 | /*  0x00020000                                            */
    0 * RTC_CR_ADD1H                 | /*  0x00010000                                            */
    0 * RTC_CR_TSIE                  | /*  0x00008000                                            */
    0 * RTC_CR_WUTIE                 | /*  0x00004000                                            */
    0 * RTC_CR_ALRBIE                | /*  0x00002000                                            */
    0 * RTC_CR_ALRAIE                | /*  0x00001000                                            */
    0 * RTC_CR_TSE                   | /*  0x00000800                                            */
    1 * RTC_CR_WUTE                  | /*  0x00000400                                            */
    0 * RTC_CR_ALRBE                 | /*  0x00000200                                            */
    0 * RTC_CR_ALRAE                 | /*  0x00000100                                            */
    0 * RTC_CR_DCE                   | /*  0x00000080                                            */
    0 * RTC_CR_FMT                   | /*  0x00000040                                            */
    0 * RTC_CR_BYPSHAD               | /*  0x00000020                                            */
    0 * RTC_CR_REFCKON               | /*  0x00000010                                            */
    0 * RTC_CR_TSEDGE                | /*  0x00000008                                            */
                                                                                                 
    0 * RTC_CR_WUCKSEL               | /*  0x00000007                                            */
    0 * RTC_CR_WUCKSEL_0             | /*    0x00000001                                          */
    0 * RTC_CR_WUCKSEL_1             | /*    0x00000002                                          */
    0 * RTC_CR_WUCKSEL_2               /*    0x00000004                                          */
  );

  RTC->ISR = ~RTC_ISR_INIT;

  RTC->WPR = 0xFF;                     /* Enable RTC write protection                            */

  // while(!(RCC->BDCR & RCC_BDCR_LSERDY)) { /*  */ }

  // PWR->CR = PWR_CR_VOS_1;              /*  Disable Backup Domain Access                          */
  // RCC->APB1ENR = 0;                    /*  Disable PWR interface                                 */

  FLASH->ACR = (
    0 * FLASH_ACR_LATENCY_0WS        | /*                                                        */
    0 * FLASH_ACR_LATENCY_1WS        | /*                                                        */
    1 * FLASH_ACR_LATENCY_2WS        | /*                                                        */
    0 * FLASH_ACR_LATENCY_3WS        | /*                                                        */
    0 * FLASH_ACR_LATENCY_4WS        | /*                                                        */
    0 * FLASH_ACR_LATENCY_5WS        | /*                                                        */
    0 * FLASH_ACR_LATENCY_6WS        | /*                                                        */
    0 * FLASH_ACR_LATENCY_7WS        | /*                                                        */
    1 * FLASH_ACR_PRFTEN             | /*  0x00000100                                            */
    1 * FLASH_ACR_ICEN               | /*  0x00000200                                            */
    1 * FLASH_ACR_DCEN               | /*  0x00000400                                            */
    0 * FLASH_ACR_ICRST              | /*  0x00000800                                            */
    0 * FLASH_ACR_DCRST              | /*  0x00001000                                            */
    0 * FLASH_ACR_BYTE0_ADDRESS      | /*  0x40023C00                                            */
    0 * FLASH_ACR_BYTE2_ADDRESS        /*  0x40023C03                                            */
  );

  while(!(RCC->CR & RCC_CR_HSERDY))  { /*  Wait till HSE is ready */ }
  while(!(RCC->CR & RCC_CR_PLLRDY))  { /*  Wait till PLL is ready */ }

  RCC->CFGR = (
    0 * RCC_CFGR_SW                  | /*  0x00000003 SW[1:0] bits (System clock Switch)         */
    0 * RCC_CFGR_SW_0                | /*    0x00000001                                          */
    0 * RCC_CFGR_SW_1                | /*    0x00000002                                          */
    0 * RCC_CFGR_SW_HSI              | /*    HSI selected as system clock                        */
    0 * RCC_CFGR_SW_HSE              | /*    HSE selected as system clock                        */
    1 * RCC_CFGR_SW_PLL              | /*    PLL selected as system clock                        */

    0 * RCC_CFGR_SWS                 | /*  0x0000000C SWS[1:0] bits (System Clock Switch Status) */
    0 * RCC_CFGR_SWS_0               | /*    0x00000004                                          */
    0 * RCC_CFGR_SWS_1               | /*    0x00000008                                          */
    0 * RCC_CFGR_SWS_HSI             | /*    HSI oscillator used as system clock                 */
    0 * RCC_CFGR_SWS_HSE             | /*    HSE oscillator used as system clock                 */
    0 * RCC_CFGR_SWS_PLL             | /*    PLL used as system clock                            */
                                                                                                 /*
       HPRE: AHB prescaler
            Set and cleared by software to control AHB clock division factor.
            Caution: The clocks are divided with the new prescaler factor
            from 1 to 16 AHB cycles after HPRE write.

              0xxx: system clock not divided
              1000: system clock divided by 2
              1001: system clock divided by 4
              1010: system clock divided by 8
              1011: system clock divided by 16
              1100: system clock divided by 64
              1101: system clock divided by 128
              1110: system clock divided by 256
              1111: system clock divided by 512
       ================================================================================          */

    0 * RCC_CFGR_HPRE                | /*  0x000000F0 HPRE[3:0] bits (AHB prescaler)             */
    0 * RCC_CFGR_HPRE_0              | /*    0x00000010                                          */
    0 * RCC_CFGR_HPRE_1              | /*    0x00000020                                          */
    0 * RCC_CFGR_HPRE_2              | /*    0x00000040                                          */
    0 * RCC_CFGR_HPRE_3              | /*    0x00000080                                          */
    1 * RCC_CFGR_HPRE_DIV1           | /*    SYSCLK not divided                                  */
    0 * RCC_CFGR_HPRE_DIV2           | /*    SYSCLK divided by 2                                 */
    0 * RCC_CFGR_HPRE_DIV4           | /*    SYSCLK divided by 4                                 */
    0 * RCC_CFGR_HPRE_DIV8           | /*    SYSCLK divided by 8                                 */
    0 * RCC_CFGR_HPRE_DIV16          | /*    SYSCLK divided by 16                                */
    0 * RCC_CFGR_HPRE_DIV64          | /*    SYSCLK divided by 64                                */
    0 * RCC_CFGR_HPRE_DIV128         | /*    SYSCLK divided by 128                               */
    0 * RCC_CFGR_HPRE_DIV256         | /*    SYSCLK divided by 256                               */
    0 * RCC_CFGR_HPRE_DIV512         | /*    SYSCLK divided by 512                               */

    0 * RCC_CFGR_PPRE1               | /*  0x00001C00 PRE1[2:0] bits (APB1 prescaler)            */
    0 * RCC_CFGR_PPRE1_0             | /*    0x00000400                                          */
    0 * RCC_CFGR_PPRE1_1             | /*    0x00000800                                          */
    0 * RCC_CFGR_PPRE1_2             | /*    0x00001000                                          */
    0 * RCC_CFGR_PPRE1_DIV1          | /*    HCLK not divided                                    */
    1 * RCC_CFGR_PPRE1_DIV2          | /*    HCLK divided by 2                                   */
    0 * RCC_CFGR_PPRE1_DIV4          | /*    HCLK divided by 4                                   */
    0 * RCC_CFGR_PPRE1_DIV8          | /*    HCLK divided by 8                                   */
    0 * RCC_CFGR_PPRE1_DIV16         | /*    HCLK divided by 16                                  */

    0 * RCC_CFGR_PPRE2               | /*  0x0000E000 PRE2[2:0] bits (APB2 prescaler)            */
    0 * RCC_CFGR_PPRE2_0             | /*    0x00002000                                          */
    0 * RCC_CFGR_PPRE2_1             | /*    0x00004000                                          */
    0 * RCC_CFGR_PPRE2_2             | /*    0x00008000                                          */
    1 * RCC_CFGR_PPRE2_DIV1          | /*    HCLK not divided                                    */
    0 * RCC_CFGR_PPRE2_DIV2          | /*    HCLK divided by 2                                   */
    0 * RCC_CFGR_PPRE2_DIV4          | /*    HCLK divided by 4                                   */
    0 * RCC_CFGR_PPRE2_DIV8          | /*    HCLK divided by 8                                   */
    0 * RCC_CFGR_PPRE2_DIV16         | /*    HCLK divided by 16                                  */

    0 * RCC_CFGR_RTCPRE              | /*  0x001F0000                                            */
    0 * RCC_CFGR_RTCPRE_0            | /*    0x00010000                                          */
    0 * RCC_CFGR_RTCPRE_1            | /*    0x00020000                                          */
    0 * RCC_CFGR_RTCPRE_2            | /*    0x00040000                                          */
    0 * RCC_CFGR_RTCPRE_3            | /*    0x00080000                                          */
    0 * RCC_CFGR_RTCPRE_4            | /*    0x00100000                                          */

    0 * RCC_CFGR_MCO1                | /*  0x00600000                                            */
    0 * RCC_CFGR_MCO1_0              | /*    0x00200000                                          */
    0 * RCC_CFGR_MCO1_1              | /*    0x00400000                                          */

    0 * RCC_CFGR_I2SSRC              | /*  0x00800000                                            */

    0 * RCC_CFGR_MCO1PRE             | /*  0x07000000                                            */
    0 * RCC_CFGR_MCO1PRE_0           | /*    0x01000000                                          */
    0 * RCC_CFGR_MCO1PRE_1           | /*    0x02000000                                          */
    0 * RCC_CFGR_MCO1PRE_2           | /*    0x04000000                                          */

    0 * RCC_CFGR_MCO2PRE             | /*  0x38000000                                            */
    0 * RCC_CFGR_MCO2PRE_0           | /*    0x08000000                                          */
    0 * RCC_CFGR_MCO2PRE_1           | /*    0x10000000                                          */
    0 * RCC_CFGR_MCO2PRE_2           | /*    0x20000000                                          */

    0 * RCC_CFGR_MCO2                | /*  0xC0000000                                            */
    0 * RCC_CFGR_MCO2_0              | /*    0x40000000                                          */
    0 * RCC_CFGR_MCO2_1                /*    0x80000000                                          */
  );
                                                                              /*
                Wait till System clock is ready
                ===============================                               */

  while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) { /*  */ }
                                                                              /*
                   Configure SysTick timer
   By default the clock source of SysTick is AHB/8 = 12.5 MHz.
   ===========================================================                */

  //SysTick->LOAD = 12500UL - 1;                /* set reload register */
  //SysTick->VAL  = 1000UL - 1;                 /* load counter value  */
  //SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;    /* start SysTick timer */

}

 

#if 0
__STATIC_INLINE void init_gpio(void) {
 
  
  *(volatile uint64_t *)&GPIOA->AFR = (
    PIN_AF(PIN(11),  AF(8)) |  
    PIN_AF(PIN(12),  AF(8)) 
  );
 
  
  GPIOA->MODER = ALL_ANALOG - (
    PIN_CONF(PIN(11), PINV_ALT_FUNC) |  
    PIN_CONF(PIN(12), PINV_ALT_FUNC) //| 
    //PIN_CONF(PIN(9), PINV_OUTPUT) | 
    //PIN_CONF(PIN(10), PINV_OUTPUT)
  );
}

__STATIC_INLINE void init_usart(uint32_t baudrate) {
  USART6->BRR = UART_BAUDRATE(100 MHZ, baudrate);
  USART6->CR1 = (
    0 * USART_CR1_SBK        | /* 0x00000001 Send Break                             */
    0 * USART_CR1_RWU        | /* 0x00000002 Receiver wakeup                        */
    1 * USART_CR1_RE         | /* 0x00000004 Receiver Enable                        */
    1 * USART_CR1_TE         | /* 0x00000008 Transmitter Enable                     */
    0 * USART_CR1_IDLEIE     | /* 0x00000010 IDLE Interrupt Enable                  */
    1 * USART_CR1_RXNEIE     | /* 0x00000020 RXNE Interrupt Enable                  */
    0 * USART_CR1_TCIE       | /* 0x00000040 Transmission Complete Interrupt Enable */
    0 * USART_CR1_TXEIE      | /* 0x00000080 TXE Interrupt Enable                   */
    0 * USART_CR1_PEIE       | /* 0x00000100 PE Interrupt Enable                    */
    0 * USART_CR1_PS         | /* 0x00000200 Parity Selection                       */
    0 * USART_CR1_PCE        | /* 0x00000400 Parity Control Enable                  */
    0 * USART_CR1_WAKE       | /* 0x00000800 Wakeup method                          */
    0 * USART_CR1_M          | /* 0x00001000 Word length                            */
    1 * USART_CR1_UE         | /* 0x00002000 USART Enable                           */
    0 * USART_CR1_OVER8        /* 0x00008000 USART Oversampling by 8 enable         */
  );
}
#endif

#define init_systick(s,en)      \
{                               \
  SysTick->CTRL |= 0x00000;     \
  SysTick->CTRL |= (en << 1);   \
  SysTick->LOAD = s;            \
  SysTick->VAL = 0;             \
  SysTick->CTRL |= (1 << 0);    \
}



 

#endif /* __MAIN_H */
