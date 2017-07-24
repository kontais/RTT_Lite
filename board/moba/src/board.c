/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2013 RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      first implementation
 */

#include <board.h>
#include <led.h>
#include <usart1.h>
#include <os.h>

/**
 * @addtogroup STM32
 */

/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
    printf("\n\r Wrong parameter value detected on\r\n");
    printf("       file  %s\r\n", file);
    printf("       line  %d\r\n", line);

    while (1) ;
}

/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handler(void)
{
    os_isr_enter();

    os_tick_increase();

    os_isr_leave();
}

/**
 * This function will initial STM32 board.
 */
void board_init(void)
{
    RCC_ClocksTypeDef RCC_Clocks;

    /* Configure the SysTick */
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency / OS_TICKS_PER_SEC);
	
    led_init();

    usart1_init();
    console_output = usart1_output;
}

/*@}*/
