
#include "main.h"

#include "w25qxxx.h"


#include <gpio.h>

 
static void MX_USART1_UART_Init(void);
static void MX_GPIO_Init(void);
void SystemClock_Config(void);

#define PLL_M      8
#define PLL_N      336
#define PLL_P      2
#define PLL_Q      7

void set_sysclk_to_168(void)
{
  /* Enable HSE (CR: bit 16) */
  RCC->CR |= (1U << 16);
  /* Wait till HSE is ready (CR: bit 17) */
  while(!(RCC->CR & (1 << 17)));

  /* Enable power interface clock (APB1ENR:bit 28) */
  RCC->APB1ENR |= (1 << 28);

  /* set voltage scale to 1 for max frequency (PWR_CR:bit 14)
   * (0b0) scale 2 for fCLK <= 144 Mhz
   * (0b1) scale 1 for 144 Mhz < fCLK <= 168 Mhz
   */
  PWR->CR |= (1 << 14);

  /* set AHB prescaler to /1 (CFGR:bits 7:4) */
  RCC->CFGR |= (0 << 4);
  /* set APB low speed prescaler to /4 (APB1) (CFGR:bits 12:10) */
  RCC->CFGR |= (5 << 10);
  /* set APB high speed prescaler to /2 (APB2) (CFGR:bits 15:13) */
  RCC->CFGR |= (4 << 13);

  /* Set M, N, P and Q PLL dividers
   * PLLCFGR: bits 5:0 (M), 14:6 (N), 17:16 (P), 27:24 (Q)
   * Set PLL source to HSE, PLLCFGR: bit 22, 1:HSE, 0:HSI
   */
  RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                 (PLL_Q << 24) | (1 << 22);
  /* Enable the main PLL (CR: bit 24) */
  RCC->CR |= (1 << 24);
  /* Wait till the main PLL is ready (CR: bit 25) */
  while(!(RCC->CR & (1 << 25)));
  /* Configure Flash
   * prefetch enable (ACR:bit 8)
   * instruction cache enable (ACR:bit 9)
   * data cache enable (ACR:bit 10)
   * set latency to 5 wait states (ARC:bits 2:0)
   *   see Table 10 on page 80 in RM0090
   */
  FLASH->ACR = (1 << 8) | (1 << 9) | (1 << 10 ) | (5 << 0);

  /* Select the main PLL as system clock source, (CFGR:bits 1:0)
   * 0b00 - HSI
   * 0b01 - HSE
   * 0b10 - PLL
   */
  RCC->CFGR &= ~(3U << 0);
  RCC->CFGR |= (2 << 0);
  /* Wait till the main PLL is used as system clock source (CFGR:bits 3:2) */
  while (!(RCC->CFGR & (2U << 2)));

  // update SystemCoreClock variable
  SystemCoreClock = 168000000;
}

 

char Message[] = "Welcome to Microcontrollers Lab\r\n"; /* Message to be transmitted through UART */


int main(void) {
  HAL_Init();
  //SystemClock_Config();
  SystemCoreClockUpdate();
  set_sysclk_to_168();
 // HAL_InitTick(1);
  MX_GPIO_Init();
  MX_USART1_UART_Init();



  hw_cfg_pin(GPIOx(GPIO_A),0,GPIOCFG_MODE_INP | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_PUP);

 
  //SPI_Flash_Init();
  
  //if (!hw_get_pin(GPIOx(GPIO_A),0)){
       kprintf("mass storage mode\n");
       MX_USB_DEVICE_Init();
       while(1);
  //}





return 0;
}

//pa10
//pa11


 
static void MX_USART1_UART_Init(void)
{

  __HAL_RCC_GPIOA_CLK_ENABLE(); /* Enable clock to PORTA - UART2 pins PA2 and PA3 */
  __HAL_RCC_USART1_CLK_ENABLE();

  #define PORT_TX        GPIO_A   // PA9  --> RXD
  #define PIN_TX         9
  #define PORT_RX        GPIO_A   // PA10 <-- TXD
  #define PIN_RX         10

  #define UART_EN   13 
#define UART_INTR_RX  5
#define UART_INTR_TCIE  6
#define UART_INTR_TX  7
#define UART_TX_EN  3
#define UART_RX_EN  2
  #define USART1_DIV_M       45
// USARTDIV_Fraction = 0.573 / (1 / 16) = 0.573 / 0.0625 ~= 9
#define USART1_DIV_F       9

  hw_cfg_pin(GPIOx(PORT_TX), PIN_TX, GPIOCFG_MODE_ALT | 7 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
  hw_cfg_pin(GPIOx(PORT_RX), PIN_RX, GPIOCFG_MODE_ALT | 7 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);


  USART1->CR1 &= ~ (1 << UART_EN);

  USART1->BRR = (USART1_DIV_M << 4) | USART1_DIV_F;
    // OVER8 = 0: oversampling by 16
    // TE = 1: Transmitter is enabled
    // RE = 1: Receiver is enabled and begins searching for a start bit
    // UE = 1: USART enable
    USART1->CR1 |= (1 << UART_INTR_RX) | (1 << UART_TX_EN) | (1 << UART_RX_EN);
    USART1->CR1 |= (1 << UART_EN);


  
}

static void MX_GPIO_Init(void)
{
  //GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

}
 
#if 0
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
 // RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};


//  __HAL_RCC_FLASH_CLK_ENABLE();



  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_ACR_LATENCY_5WS) != HAL_OK) {
    Error_Handler();
  }
   
}
 #endif
 
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}