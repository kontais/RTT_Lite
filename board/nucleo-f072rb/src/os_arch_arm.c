/*-----------------------------------------------------------------------
* adc1.c  -
*
*
*
* Copyright (C) 2016 XCMG Group.
*
*-----------------------------------------------------------------------*/
#include <stm32f10x.h>

uint32_t os_enter_critical(void)
{
    uint32_t isr_ctx;

    isr_ctx = __get_PRIMASK();
    __disable_irq();
    return (isr_ctx & 1);
}

void os_exit_critical(uint32_t isr_ctx)
{
    if (!isr_ctx) {
        __enable_irq();
    }
}
