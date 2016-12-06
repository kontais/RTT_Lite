/*
 * File      : mmu.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-01-10     bernard      porting to AM1808
 */

#include <os.h>
#include <rtcpu.h>
#include <board.h>

#include "cp15.h"

#define DESC_SEC       (0x2)
#define CB             (3<<2)  //cache_on, write_back
#define CNB            (2<<2)  //cache_on, write_through
#define NCB            (1<<2)  //cache_off,WR_BUF on
#define NCNB           (0<<2)  //cache_off,WR_BUF off
#define AP_RW          (3<<10) //supervisor=RW, user=RW
#define AP_RO          (2<<10) //supervisor=RW, user=RO
#define XN             (1<<4)  // eXecute Never

#define DOMAIN_FAULT   (0x0)
#define DOMAIN_CHK     (0x1)
#define DOMAIN_NOTCHK  (0x3)
#define DOMAIN0        (0x0<<5)
#define DOMAIN1        (0x1<<5)

#define DOMAIN0_ATTR   (DOMAIN_CHK<<0)
#define DOMAIN1_ATTR   (DOMAIN_FAULT<<2)

/* Read/Write, cache, write back */
#define RW_CB          (AP_RW|DOMAIN0|CB|DESC_SEC)
/* Read/Write, cache, write through */
#define RW_CNB         (AP_RW|DOMAIN0|CNB|DESC_SEC)
/* Read/Write without cache and write buffer */
#define RW_NCNB        (AP_RW|DOMAIN0|NCNB|DESC_SEC)
/* Read/Write without cache and write buffer, no execute */
#define RW_NCNBXN      (AP_RW|DOMAIN0|NCNB|DESC_SEC|XN)
/* Read/Write without cache and write buffer */
#define RW_FAULT       (AP_RW|DOMAIN1|NCNB|DESC_SEC)

/* dump 2nd level page table */
void os_arch_dump_page_table_2nd(uint32_t *ptb)
{
    int i;
    int fcnt = 0;

    for (i = 0; i < 256; i++) {
        uint32_t pte2 = ptb[i];
        if ((pte2 & 0x3) == 0) {
            if (fcnt == 0)
                printk("    ");
            printk("%04x: ", i);
            fcnt++;
            if (fcnt == 16) {
                printk("fault\n");
                fcnt = 0;
            }
            continue;
        }
        if (fcnt != 0) {
            printk("fault\n");
            fcnt = 0;
        }

        printk("    %04x: %x: ", i, pte2);
        if ((pte2 & 0x3) == 0x1) {
            printk("L,ap:%x,xn:%d,texcb:%02x\n",
                       ((pte2 >> 7) | (pte2 >> 4))& 0xf,
                       (pte2 >> 15) & 0x1,
                       ((pte2 >> 10) | (pte2 >> 2)) & 0x1f);
        } else {
            printk("S,ap:%x,xn:%d,texcb:%02x\n",
                       ((pte2 >> 7) | (pte2 >> 4))& 0xf, pte2 & 0x1,
                       ((pte2 >> 4) | (pte2 >> 2)) & 0x1f);
        }
    }
}

void os_arch_dump_page_table(uint32_t *ptb)
{
    int i;
    int fcnt = 0;

    printk("page table@%p\n", ptb);
    for (i = 0; i < 1024*4; i++) {
        uint32_t pte1 = ptb[i];
        if ((pte1 & 0x3) == 0) {
            printk("%03x: ", i);
            fcnt++;
            if (fcnt == 16) {
                printk("fault\n");
                fcnt = 0;
            }
            continue;
        }
        if (fcnt != 0) {
            printk("fault\n");
            fcnt = 0;
        }

        printk("%03x: %08x: ", i, pte1);
        if ((pte1 & 0x3) == 0x3) {
            printk("LPAE\n");
        } else if ((pte1 & 0x3) == 0x1) {
            printk("pte,ns:%d,domain:%d\n",
                       (pte1 >> 3) & 0x1, (pte1 >> 5) & 0xf);
            /*
             *os_arch_dump_page_table_2nd((void*)((pte1 & 0xfffffc000)
             *                               - 0x80000000 + 0xC0000000));
             */
        } else if (pte1 & (1 << 18)) {
            printk("super section,ns:%d,ap:%x,xn:%d,texcb:%02x\n",
                       (pte1 >> 19) & 0x1,
                       ((pte1 >> 13) | (pte1 >> 10))& 0xf,
                       (pte1 >> 4) & 0x1,
                       ((pte1 >> 10) | (pte1 >> 2)) & 0x1f);
        } else {
            printk("section,ns:%d,ap:%x,"
                       "xn:%d,texcb:%02x,domain:%d\n",
                       (pte1 >> 19) & 0x1,
                       ((pte1 >> 13) | (pte1 >> 10))& 0xf,
                       (pte1 >> 4) & 0x1,
                       (((pte1 & (0x7 << 12)) >> 10) |
                        ((pte1 &        0x0c) >>  2)) & 0x1f,
                       (pte1 >> 5) & 0xf);
        }
    }
}

/* level1 page table, each entry for 1MB memory. */
volatile static uint32_t MMUTable[4*1024] __attribute__((aligned(16*1024)));
void os_arch_mmu_setmtt(uint32_t vaddrStart,
                      uint32_t vaddrEnd,
                      uint32_t paddrStart,
                      uint32_t attr)
{
    volatile uint32_t *pTT;
    volatile int i, nSec;
    pTT  = (uint32_t *)MMUTable + (vaddrStart >> 20);
    nSec = (vaddrEnd >> 20) - (vaddrStart >> 20);
    for (i = 0; i <= nSec; i++) {
        *pTT = attr | (((paddrStart >> 20) + i) << 20);
        pTT++;
    }
}

uint32_t os_arch_set_domain_register(uint32_t domain_val)
{
    uint32_t old_domain;

    asm volatile ("mrc p15, 0, %0, c3, c0\n" : "=r" (old_domain));
    asm volatile ("mcr p15, 0, %0, c3, c0\n" : :"r" (domain_val) : "memory");

    return old_domain;
}

void os_arch_mmu_init(void)
{
    os_arch_dcache_disable();
    os_arch_icache_disable();
    rt_os_arch_mmu_disable();

    /* set page table */
    /* 4G 1:1 memory */
    os_arch_mmu_setmtt(0, 0xffffffff-1, 0, RW_CB);
    /* IO memory region */
    os_arch_mmu_setmtt(0x44000000, 0x80000000-1, 0x44000000, RW_NCNBXN);

    /*os_arch_dump_page_table(MMUTable);*/
    os_arch_set_domain_register(0x55555555);

    rt_os_arch_tlb_set(MMUTable);

    rt_os_arch_mmu_enable();

    os_arch_icache_enable();
    os_arch_dcache_enable();
}

