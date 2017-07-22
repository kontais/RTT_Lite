/*
 * File      : timer_test.c
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
#include "stm32f4xx.h"
#include <io.h>
#include <board.h>
#include <os.h>


os_timer_t test_timer;
os_timer_t test_timer2;

os_timer_t test_timer3;
os_timer_t test_timer4;

static void time_out1(void *arg)
{
    GPIO_TogglePin(LED0_PORT, LED0_PIN);
}

static void time_out2(void *arg)
{
    GPIO_TogglePin(LED1_PORT, LED1_PIN);
}

static void time_out3(void *arg)
{
    printk("time_out3\n");
    os_task_sleep(200);
    printk("time_out3 -> 2\n");

}

static void time_out4(void *arg)
{
    printk("time_out4\n");
    os_task_sleep(500);
    printk("time_out4 -> 2\n");
}

void timer_test(void)
{
    os_timer_init(&test_timer, "xx1", time_out1, NULL, 100, OS_TIMER_PERIODIC);
    os_timer_start(&test_timer);

    os_timer_init(&test_timer2, "xx2", time_out2, NULL, 200, OS_TIMER_PERIODIC);
    os_timer_start(&test_timer2);

    os_timer_init(&test_timer3, "xx3", time_out3, NULL, 1000, OS_TIMER_PERIODIC | OS_TIMER_SOFT_TIMER);
    os_timer_start(&test_timer3);

    os_timer_init(&test_timer4, "xx4", time_out4, NULL, 2000, OS_TIMER_PERIODIC | OS_TIMER_SOFT_TIMER);
    os_timer_start(&test_timer4);
}
