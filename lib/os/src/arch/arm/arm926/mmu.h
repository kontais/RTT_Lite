/*
 * File      : mmu.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
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
 */

#ifndef __MMU_H__
#define __MMU_H__

#include <os.h>

#define CACHE_LINE_SIZE     32

#define DESC_SEC            (0x2|(1<<4))
#define CB                  (3<<2)  //cache_on, write_back
#define CNB                 (2<<2)  //cache_on, write_through
#define NCB                 (1<<2)  //cache_off,WR_BUF on
#define NCNB                (0<<2)  //cache_off,WR_BUF off
#define AP_RW               (3<<10) //supervisor=RW, user=RW
#define AP_RO               (2<<10) //supervisor=RW, user=RO

#define DOMAIN_FAULT        (0x0)
#define DOMAIN_CHK          (0x1)
#define DOMAIN_NOTCHK       (0x3)
#define DOMAIN0             (0x0<<5)
#define DOMAIN1             (0x1<<5)

#define DOMAIN0_ATTR        (DOMAIN_CHK<<0)
#define DOMAIN1_ATTR        (DOMAIN_FAULT<<2)

#define RW_CB       (AP_RW|DOMAIN0|CB|DESC_SEC)     /* Read/Write, cache, write back */
#define RW_CNB      (AP_RW|DOMAIN0|CNB|DESC_SEC)    /* Read/Write, cache, write through */
#define RW_NCNB     (AP_RW|DOMAIN0|NCNB|DESC_SEC)   /* Read/Write without cache and write buffer */
#define RW_FAULT    (AP_RW|DOMAIN1|NCNB|DESC_SEC)   /* Read/Write without cache and write buffer */

struct mem_desc
{
    uint32_t vaddr_start;
    uint32_t vaddr_end;
    uint32_t paddr_start;
    uint32_t attr;
};

void os_arch_mmu_init(struct mem_desc *mdesc, uint32_t size);

#endif
