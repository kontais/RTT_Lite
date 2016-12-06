/*
 * File      : cpu.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://openlab.rt-thread.com/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-03-13     Bernard      first version
 */

#include <rtcpu.h>
#include <os.h>
#include "s3c24x0.h"

/**
 * @addtogroup S3C24X0
 */
/*@{*/

#define ICACHE_MASK    (uint32_t)(1 << 12)
#define DCACHE_MASK    (uint32_t)(1 << 2)

#ifdef __GNUC__
rt_inline uint32_t cp15_rd(void)
{
    uint32_t i;

    asm ("mrc p15, 0, %0, c1, c0, 0":"=r" (i));
    return i;
}

rt_inline void cache_enable(uint32_t bit)
{
    __asm__ __volatile__(            \
        "mrc  p15,0,r0,c1,c0,0\n\t"    \
        "orr  r0,r0,%0\n\t"            \
           "mcr  p15,0,r0,c1,c0,0"        \
        :                            \
        :"r" (bit)                    \
        :"memory");
}

rt_inline void cache_disable(uint32_t bit)
{
    __asm__ __volatile__(            \
        "mrc  p15,0,r0,c1,c0,0\n\t"    \
        "bic  r0,r0,%0\n\t"            \
        "mcr  p15,0,r0,c1,c0,0"        \
        :                            \
        :"r" (bit)                    \
        :"memory");
}
#endif

#ifdef __CC_ARM
rt_inline uint32_t cp15_rd(void)
{
    uint32_t i;

    __asm
    {
        mrc p15, 0, i, c1, c0, 0
    }

    return i;
}

rt_inline void cache_enable(uint32_t bit)
{
    uint32_t value;

    __asm
    {
        mrc p15, 0, value, c1, c0, 0
        orr value, value, bit
        mcr p15, 0, value, c1, c0, 0
    }
}

rt_inline void cache_disable(uint32_t bit)
{
    uint32_t value;

    __asm
    {
        mrc p15, 0, value, c1, c0, 0
        bic value, value, bit
        mcr p15, 0, value, c1, c0, 0
    }
}
#endif

/**
 * enable I-Cache
 *
 */
void os_arch_icache_enable()
{
    cache_enable(ICACHE_MASK);
}

/**
 * disable I-Cache
 *
 */
void os_arch_icache_disable()
{
    cache_disable(ICACHE_MASK);
}

/**
 * return the status of I-Cache
 *
 */
int32_t os_arch_icache_status()
{
    return (cp15_rd() & ICACHE_MASK);
}

/**
 * enable D-Cache
 *
 */
void os_arch_dcache_enable()
{
    cache_enable(DCACHE_MASK);
}

/**
 * disable D-Cache
 *
 */
void os_arch_dcache_disable()
{
    cache_disable(DCACHE_MASK);
}

/**
 * return the status of D-Cache
 *
 */
int32_t os_arch_dcache_status()
{
    return (cp15_rd() & DCACHE_MASK);
}

/**
 * reset cpu by dog's time-out
 *
 */
void os_arch_reset()
{
    /* Disable all interrupt except the WDT */
    INTMSK = (~((uint32_t)1 << INTWDT));

    /* Disable watchdog */
    WTCON = 0x0000;

    /* Initialize watchdog timer count register */
    WTCNT = 0x0001;

    /* Enable watchdog timer; assert reset at timer timeout */
    WTCON = 0x0021;

    while (1);    /* loop forever and wait for reset to happen */

    /* NEVER REACHED */
}

/**
 *  shutdown CPU
 *
 */
void os_arch_shutdown()
{
    uint32_t level;
    printk("shutdown...\n");

    sr = os_enter_critical();
    while (level) {
        OS_ASSERT(0);
    }
}

/*@}*/
