/*-----------------------------------------------------------------------
* systick.c -
*
*
*
* Copyright (C) 2018 kontais@aliyun.com
*
*-----------------------------------------------------------------------*/
#include <board.h>

volatile uint32_t g_system_tick = 0;

void systick_init(void)
{
  /* Setup SysTick Timer for 10ms interrupts  */
  if (SysTick_Config(SystemCoreClock / 1000))
  {
    /* Capture error */
    while (1);
  }
  /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}

void SysTick_Handler(void)
{
    g_system_tick++;
}
