/*
 * File      : trap.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://openlab.rt-thread.com/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-08-25     Bernard      first version
 */

#include <os.h>
#include <rtcpu.h>

#include "AT91SAM7S.h"

/**
 * @addtogroup AT91SAM7
 */
/*@{*/

void os_arch_trap_irq()
{
    rt_isr_handler_t hander = (rt_isr_handler_t)AT91C_AIC_IVR;

    hander(AT91C_AIC_ISR);

    /* end of interrupt */
    AT91C_AIC_EOICR = 0;
}

void os_arch_trap_fiq()
{
    printk("fast interrupt request\n");
}

/*@}*/
