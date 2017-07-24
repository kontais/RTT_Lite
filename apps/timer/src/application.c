/*
 * File      : application.c
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
#include <os.h>
#include <stdio.h>

#define INIT_TASK_STACK_SIZE    2048
static os_task_t init;
ALIGN(OS_ALIGN_SIZE)
static uint8_t init_task_stack[INIT_TASK_STACK_SIZE];

void init_task_cleanup(os_task_t *task)
{
    printk("init_task_cleanup\n");
}


void timer_test(void);

float f1 = 1.0F/3.0F;
double d1 = 1.0L/3.0L;

void init_task_entry(void* parameter)
{
    os_task_t *task;

    task = os_task_self();
    task->cleanup = init_task_cleanup;

    timer_test();
}

int application_init(void)
{
    uint32_t os_ver;

    os_ver = os_version_get();

    printf("os verion %d.%d.%d\n", OS_VER_MAJOR(os_ver), OS_VER_MINOR(os_ver), OS_VER_REVISION(os_ver));
    os_task_init(&init, "init", init_task_entry, NULL, &init_task_stack[0], INIT_TASK_STACK_SIZE, OS_TASK_PRIORITY_MAX/3, 20);
    os_task_startup(&init);

    return 0;
}
