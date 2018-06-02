#include "board.h"
#include <stdio.h>

void (*tim3_callback)(void) = NULL;

void tim3_nvic_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the TIM3 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void tim3_init(uint16_t period50us)
{
    uint16_t PrescalerValue = 0;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    assert_param(period50us > 1);
    
    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    tim3_nvic_init();

    /* -----------------------------------------------------------------------
    TIM3 Configuration: Output Compare Timing Mode:
    
    In this example TIM3 input clock (TIM3CLK) is set to APB1 clock (PCLK1),  
      => TIM3CLK = PCLK1 = SystemCoreClock = 48 MHz
          
    To get TIM3 counter clock at 6 MHz, the prescaler is computed as follows:
       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
       Prescaler = (PCLK1 /6 MHz) - 1
                                              
    CC1 update rate = TIM3 counter clock / CCR1_Val = 146.48 Hz
    ==> Toggling frequency = 73.24 Hz
    
    C2 update rate = TIM3 counter clock / CCR2_Val = 219.7 Hz
    ==> Toggling frequency = 109.8 Hz
    
    CC3 update rate = TIM3 counter clock / CCR3_Val = 439.4 Hz
    ==> Toggling frequency = 219.7 Hz
    
    CC4 update rate = TIM3 counter clock / CCR4_Val = 878.9 Hz
    ==> Toggling frequency = 439.4 Hz

    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect.    
  ----------------------------------------------------------------------- */

    /* Compute the prescaler value */
    PrescalerValue = (uint16_t) ((SystemCoreClock) / 20000) - 1;

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = period50us - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* Prescaler configuration */
    TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);
}

void tim3_enable(void)
{
    TIM_SetCounter(TIM3, 0x0000);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

void tim3_disable(void)
{
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
    TIM_Cmd(TIM3, DISABLE);
}


void tim3_register_callback(void (callback)(void))
{
    tim3_callback = callback;
}

void tim3_close(void)
{
    NVIC_DisableIRQ(TIM3_IRQn);
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
}

//uint32_t tim3_count = 0;
//uint32_t tim3_stop  = 0;

void TIM3_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    if (tim3_callback != NULL) {
        tim3_callback();
    }
//    tim3_count++;
//    if ((tim3_count % 20000) == 0) {
//        printf("%d\n", tim3_count);
//        tim3_stop++;
//        if (tim3_stop > 5) {
//            tim3_disable();
//        }
//    }
}
