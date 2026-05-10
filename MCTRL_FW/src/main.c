#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_rcc.h"

#include "main.h"
#include "terminal.h"

static void SystemClock_Init(void);

int main(void)
{
  SystemClock_Init();
  Terminal_Init();

  while (1)
  {
    Terminal_Send("Hello World!\n");
  }
}

static void SystemClock_Init(void)
{
  /* Enable HSE oscillator */
  LL_RCC_HSE_Enable();
  while (LL_RCC_HSE_IsReady() != 1);

  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 336, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();
  while (LL_RCC_PLL_IsReady() != 1);

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1); // 168 MHz
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

  /* Set APB1 & APB2 prescaler */
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4); // 42 MHz
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2); // 84 MHz

  /* Set systick to 1ms */
  SysTick_Config(168000000 / 1000);

  /* Update CMSIS variable */
  SystemCoreClock = 168000000;
}
