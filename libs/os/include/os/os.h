/*
 * File      : os.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2012, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-03-18     Bernard      the first version
 * 2006-04-26     Bernard      add semaphore APIs
 * 2006-08-10     Bernard      add version information
 * 2007-01-28     Bernard      rename OS_OBJECT_Class_Static to RT_Object_Class_Static
 * 2007-03-03     Bernard      clean up the definitions to os_def.h
 * 2010-04-11     yi.qiu       add module feature
 * 2013-06-24     Bernard      add printk re-define when not use OS_CFG_CONSOLE.
 */

#ifndef _OS_H_
#define _OS_H_

enum os_err {
  OS_OK       = 0, 	/**< There is no error */
  OS_ERROR    = 1, 	/**< A generic error happens */
  OS_TIMEOUT  = 2, 	/**< Timed out */
  OS_EFULL    = 3, 	/**< The resource is full */
  OS_EEMPTY   = 4, 	/**< The resource is empty */
  OS_ENOMEM   = 5, 	/**< No memory */
  OS_NOSYS    = 6, 	/**< No system */
  OS_EBUSY    = 7, 	/**< Busy */
  OS_EIO      = 8, 	/**< IO error */
};

typedef enum os_err os_err_t;

void os_init(void);
void os_start(void);

#include <stdint.h>

#include <os/os_cfg.h>
#include <compiler.h>
#include <os/os_def.h>
#include <os/os_misc.h>
#include <os/os_debug.h>

#include <rtstring.h>
#include <rtstdio.h>

#define OS_IDLE_TASK_PRIO             (OS_TASK_PRIORITY_MAX - 1)

#include <os/os_list.h>
#include <os/os_error.h>
#include <os/os_version.h>

#include <os/os_arch.h>
#include <os/rtcpu.h>

#include <os/os_console.h>
#include <os/os_irq.h>
#include <os/os_tick.h>
#include <os/os_timer.h>
#include <os/os_task.h>
#include <os/os_idle.h>
#include <os/os_sched.h>

/* os components */
#ifdef OS_CFG_HEAP
#include <os/os_heap.h>
#endif

#include <os/os_mpool.h>
#include <os/os_sem.h>
#include <os/os_mutex.h>
#include <os/os_event.h>
#include <os/os_mbox.h>
#include <os/os_mqueue.h>

#ifdef OS_CFG_SOFT_TIMER
#include <os/os_timer_task.h>
#endif

#include <os/os_ipc.h>

#endif	/* _OS_H_ */
