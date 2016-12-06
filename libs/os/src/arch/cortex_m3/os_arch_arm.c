/*
 * File      : os_arch_arm.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2013, RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://openlab.rt-thread.com/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2016-12-06     kontais      kontais@aliyun.com
 */
#include "os/os.h"
#include "os/os_arch.h"

os_sr_t os_enter_critical(void)
{
    uint32_t isr_ctx;

    isr_ctx = __get_PRIMASK();
    __disable_irq();
    return (isr_ctx & 1);
}

void os_exit_critical(os_sr_t isr_ctx)
{
    if (!isr_ctx) {
        __enable_irq();
    }
}
