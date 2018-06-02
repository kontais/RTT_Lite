/*-----------------------------------------------------------------------
* rcc.c  -
*
*
*
* Copyright (C) 2016 XCMG Group.
*
*-----------------------------------------------------------------------*/
#include <stm32f10x.h>
#include <stdio.h>

void rcc_show(void)
{
    RCC_ClocksTypeDef  rcc_clocks;

    RCC_GetClocksFreq(&rcc_clocks);

    printf("SYSCLK      = %dHz\n", rcc_clocks.SYSCLK_Frequency);
    printf("HCLK(AHB)   = %dHz\n", rcc_clocks.HCLK_Frequency);
    printf("PCLK1(APB1) = %dHz\n", rcc_clocks.PCLK1_Frequency);
    printf("PCLK2(APB2) = %dHz\n", rcc_clocks.PCLK2_Frequency);
    printf("ADCCLK      = %dHz\n", rcc_clocks.ADCCLK_Frequency);
}
