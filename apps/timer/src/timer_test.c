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
#include <board.h>
#include <led.h>
#include <os.h>

os_timer_t test_timer;
os_timer_t test_timer2;

static void time_out1(void *arg)
{
    led_toggle(LED0_PORT, LED0_PIN);
}

static void time_out2(void *arg)
{
    led_toggle(LED1_PORT, LED1_PIN);
}

void timer_test(void)
{
    os_timer_init(&test_timer, time_out1, NULL, 100, OS_TIMER_PERIODIC);
    os_timer_start(&test_timer);

    os_timer_init(&test_timer2, time_out2, NULL, 200, OS_TIMER_PERIODIC);
    os_timer_start(&test_timer2);
}
