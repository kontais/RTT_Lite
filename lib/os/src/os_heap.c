/*
 * File      : os_heap.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2008 - 2012, RT-Thread Development Team
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
 * 2010-10-14     Bernard      fix os_realloc issue when realloc a NULL pointer.
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *         Simon Goldschmidt
 *
 */
#include <os.h>

#define OS_HEAP_STATS

#if defined (OS_CFG_HEAP)

#define HEAP_MAGIC 0x1ea0
struct heap_mem
{
    /* magic and used flag */
    uint16_t magic;
    uint16_t used;

    size_t next, prev;
};

/** pointer to the heap: for alignment, heap_ptr is now a pointer instead of an array */
static uint8_t *heap_ptr;

/** the last entry, always unused! */
static struct heap_mem *heap_end;

#define MIN_SIZE 12
#define MIN_SIZE_ALIGNED     OS_ALIGN(MIN_SIZE, OS_ALIGN_SIZE)
#define SIZEOF_STRUCT_MEM    OS_ALIGN(sizeof(struct heap_mem), OS_ALIGN_SIZE)

static struct heap_mem *lfree;   /* pointer to the lowest free block */

static os_sem_t heap_sem;
static size_t mem_size_aligned;

#ifdef OS_HEAP_STATS
static size_t used_mem, max_mem;
#endif

static void plug_holes(struct heap_mem *mem)
{
    struct heap_mem *nmem;
    struct heap_mem *pmem;

    OS_ASSERT((uint8_t *)mem >= heap_ptr);
    OS_ASSERT((uint8_t *)mem < (uint8_t *)heap_end);
    OS_ASSERT(mem->used == 0);

    /* plug hole forward */
    nmem = (struct heap_mem *)&heap_ptr[mem->next];
    if (mem != nmem &&
        nmem->used == 0 &&
        (uint8_t *)nmem != (uint8_t *)heap_end) {
        /* if mem->next is unused and not end of heap_ptr,
         * combine mem and mem->next
         */
        if (lfree == nmem) {
            lfree = mem;
        }
        mem->next = nmem->next;
        ((struct heap_mem *)&heap_ptr[nmem->next])->prev = (uint8_t *)mem - heap_ptr;
    }

    /* plug hole backward */
    pmem = (struct heap_mem *)&heap_ptr[mem->prev];
    if (pmem != mem && pmem->used == 0) {
        /* if mem->prev is unused, combine mem and mem->prev */
        if (lfree == mem) {
            lfree = pmem;
        }
        pmem->next = mem->next;
        ((struct heap_mem *)&heap_ptr[mem->next])->prev = (uint8_t *)pmem - heap_ptr;
    }
}

/**
 * @ingroup SystemInit
 *
 * This function will initialize system heap memory.
 *
 * @param begin_addr the beginning address of system heap memory.
 * @param end_addr the end address of system heap memory.
 */
void os_heap_init(void *begin_addr, void *end_addr)
{
    struct heap_mem *mem;
    uint32_t begin_align = OS_ALIGN((uint32_t)begin_addr, OS_ALIGN_SIZE);
    uint32_t end_align = OS_ALIGN_DOWN((uint32_t)end_addr, OS_ALIGN_SIZE);

    OS_DEBUG_NOT_IN_INTERRUPT;

    /* alignment addr */
    if ((end_align > (2 * SIZEOF_STRUCT_MEM)) &&
        ((end_align - 2 * SIZEOF_STRUCT_MEM) >= begin_align)) {
        /* calculate the aligned memory size */
        mem_size_aligned = end_align - begin_align - 2 * SIZEOF_STRUCT_MEM;
    } else {
        printf("mem init, error begin address 0x%x, and end address 0x%x\n",
                   (uint32_t)begin_addr, (uint32_t)end_addr);

        return;
    }

    /* point to begin address of heap */
    heap_ptr = (uint8_t *)begin_align;

    OS_DEBUG_LOG(OS_DEBUG_HEAP, ("mem init, heap begin address 0x%x, size %d\n",
                                (uint32_t)heap_ptr, mem_size_aligned));

    /* initialize the start of the heap */
    mem        = (struct heap_mem *)heap_ptr;
    mem->magic = HEAP_MAGIC;
    mem->next  = mem_size_aligned + SIZEOF_STRUCT_MEM;
    mem->prev  = 0;
    mem->used  = 0;

    /* initialize the end of the heap */
    heap_end        = (struct heap_mem *)&heap_ptr[mem->next];
    heap_end->magic = HEAP_MAGIC;
    heap_end->used  = 1;
    heap_end->next  = mem_size_aligned + SIZEOF_STRUCT_MEM;
    heap_end->prev  = mem_size_aligned + SIZEOF_STRUCT_MEM;

    os_sem_init(&heap_sem, 1, OS_IPC_FIFO);

    /* initialize the lowest-free pointer to the start of the heap */
    lfree = (struct heap_mem *)heap_ptr;
}

/**
 * @addtogroup MM
 */

/*@{*/

/**
 * Allocate a block of memory with a minimum of 'size' bytes.
 *
 * @param size is the minimum size of the requested block in bytes.
 *
 * @return pointer to allocated memory or NULL if no free memory was found.
 */
void *os_malloc(size_t size)
{
    size_t ptr, ptr2;
    struct heap_mem *mem, *mem2;

    OS_DEBUG_NOT_IN_INTERRUPT;

    if (size == 0)
        return NULL;

    if (size != OS_ALIGN(size, OS_ALIGN_SIZE))
        OS_DEBUG_LOG(OS_DEBUG_HEAP, ("malloc size %d, but align to %d\n",
                                    size, OS_ALIGN(size, OS_ALIGN_SIZE)));
    else
        OS_DEBUG_LOG(OS_DEBUG_HEAP, ("malloc size %d\n", size));

    /* alignment size */
    size = OS_ALIGN(size, OS_ALIGN_SIZE);

    if (size > mem_size_aligned) {
        OS_DEBUG_LOG(OS_DEBUG_HEAP, ("no memory\n"));

        return NULL;
    }

    /* every data block must be at least MIN_SIZE_ALIGNED long */
    if (size < MIN_SIZE_ALIGNED)
        size = MIN_SIZE_ALIGNED;

    /* take memory semaphore */
    os_sem_take(&heap_sem, OS_WAIT_FOREVER);

    for (ptr = (uint8_t *)lfree - heap_ptr;
         ptr < mem_size_aligned - size;
         ptr = ((struct heap_mem *)&heap_ptr[ptr])->next) {
        mem = (struct heap_mem *)&heap_ptr[ptr];

        if ((!mem->used) && (mem->next - (ptr + SIZEOF_STRUCT_MEM)) >= size) {
            /* mem is not used and at least perfect fit is possible:
             * mem->next - (ptr + SIZEOF_STRUCT_MEM) gives us the 'user data size' of mem */

            if (mem->next - (ptr + SIZEOF_STRUCT_MEM) >=
                (size + SIZEOF_STRUCT_MEM + MIN_SIZE_ALIGNED)) {
                /* (in addition to the above, we test if another struct heap_mem (SIZEOF_STRUCT_MEM) containing
                 * at least MIN_SIZE_ALIGNED of data also fits in the 'user data space' of 'mem')
                 * -> split large block, create empty remainder,
                 * remainder must be large enough to contain MIN_SIZE_ALIGNED data: if
                 * mem->next - (ptr + (2*SIZEOF_STRUCT_MEM)) == size,
                 * struct heap_mem would fit in but no data between mem2 and mem2->next
                 * @todo we could leave out MIN_SIZE_ALIGNED. We would create an empty
                 *       region that couldn't hold data, but when mem->next gets freed,
                 *       the 2 regions would be combined, resulting in more free memory
                 */
                ptr2 = ptr + SIZEOF_STRUCT_MEM + size;

                /* create mem2 struct */
                mem2       = (struct heap_mem *)&heap_ptr[ptr2];
                mem2->magic = HEAP_MAGIC;
                mem2->used = 0;
                mem2->next = mem->next;
                mem2->prev = ptr;

                /* and insert it between mem and mem->next */
                mem->next = ptr2;
                mem->used = 1;

                if (mem2->next != mem_size_aligned + SIZEOF_STRUCT_MEM) {
                    ((struct heap_mem *)&heap_ptr[mem2->next])->prev = ptr2;
                }
#ifdef OS_HEAP_STATS
                used_mem += (size + SIZEOF_STRUCT_MEM);
                if (max_mem < used_mem)
                    max_mem = used_mem;
#endif
            } else {
                /* (a mem2 struct does no fit into the user data space of mem and mem->next will always
                 * be used at this point: if not we have 2 unused structs in a row, plug_holes should have
                 * take care of this).
                 * -> near fit or excact fit: do not split, no mem2 creation
                 * also can't move mem->next directly behind mem, since mem->next
                 * will always be used at this point!
                 */
                mem->used = 1;
#ifdef OS_HEAP_STATS
                used_mem += mem->next - ((uint8_t*)mem - heap_ptr);
                if (max_mem < used_mem)
                    max_mem = used_mem;
#endif
            }
            /* set memory block magic */
            mem->magic = HEAP_MAGIC;

            if (mem == lfree) {
                /* Find next free block after mem and update lowest free pointer */
                while (lfree->used && lfree != heap_end)
                    lfree = (struct heap_mem *)&heap_ptr[lfree->next];

                OS_ASSERT(((lfree == heap_end) || (!lfree->used)));
            }

            os_sem_give(&heap_sem);
            OS_ASSERT((uint32_t)mem + SIZEOF_STRUCT_MEM + size <= (uint32_t)heap_end);
            OS_ASSERT((uint32_t)((uint8_t *)mem + SIZEOF_STRUCT_MEM) % OS_ALIGN_SIZE == 0);
            OS_ASSERT((((uint32_t)mem) & (OS_ALIGN_SIZE-1)) == 0);

            OS_DEBUG_LOG(OS_DEBUG_HEAP,
                         ("allocate memory at 0x%x, size: %d\n",
                          (uint32_t)((uint8_t *)mem + SIZEOF_STRUCT_MEM),
                          (uint32_t)(mem->next - ((uint8_t *)mem - heap_ptr))));

            /* return the memory data except mem struct */
            return (uint8_t *)mem + SIZEOF_STRUCT_MEM;
        }
    }

    os_sem_give(&heap_sem);

    return NULL;
}

/**
 * This function will change the previously allocated memory block.
 *
 * @param rmem pointer to memory allocated by os_malloc
 * @param newsize the required new size
 *
 * @return the changed memory block address
 */
void *os_realloc(void *rmem, size_t newsize)
{
    size_t size;
    size_t ptr, ptr2;
    struct heap_mem *mem, *mem2;
    void *nmem;

    OS_DEBUG_NOT_IN_INTERRUPT;

    /* alignment size */
    newsize = OS_ALIGN(newsize, OS_ALIGN_SIZE);
    if (newsize > mem_size_aligned) {
        OS_DEBUG_LOG(OS_DEBUG_HEAP, ("realloc: out of memory\n"));

        return NULL;
    }

    /* allocate a new memory block */
    if (rmem == NULL)
        return os_malloc(newsize);

    os_sem_take(&heap_sem, OS_WAIT_FOREVER);

    if ((uint8_t *)rmem < (uint8_t *)heap_ptr ||
        (uint8_t *)rmem >= (uint8_t *)heap_end) {
        /* illegal memory */
        os_sem_give(&heap_sem);

        return rmem;
    }

    mem = (struct heap_mem *)((uint8_t *)rmem - SIZEOF_STRUCT_MEM);

    ptr = (uint8_t *)mem - heap_ptr;
    size = mem->next - ptr - SIZEOF_STRUCT_MEM;
    if (size == newsize) {
        /* the size is the same as */
        os_sem_give(&heap_sem);

        return rmem;
    }

    if (newsize + SIZEOF_STRUCT_MEM + MIN_SIZE < size) {
        /* split memory block */
#ifdef OS_HEAP_STATS
        used_mem -= (size - newsize);
#endif

        ptr2 = ptr + SIZEOF_STRUCT_MEM + newsize;
        mem2 = (struct heap_mem *)&heap_ptr[ptr2];
        mem2->magic= HEAP_MAGIC;
        mem2->used = 0;
        mem2->next = mem->next;
        mem2->prev = ptr;
        mem->next = ptr2;
        if (mem2->next != mem_size_aligned + SIZEOF_STRUCT_MEM) {
            ((struct heap_mem *)&heap_ptr[mem2->next])->prev = ptr2;
        }

        plug_holes(mem2);

        os_sem_give(&heap_sem);

        return rmem;
    }
    os_sem_give(&heap_sem);

    /* expand memory */
    nmem = os_malloc(newsize);
    if (nmem != NULL) /* check memory */
    {
        memcpy(nmem, rmem, size < newsize ? size : newsize);
        os_free(rmem);
    }

    return nmem;
}

/**
 * This function will contiguously allocate enough space for count objects
 * that are size bytes of memory each and returns a pointer to the allocated
 * memory.
 *
 * The allocated memory is filled with bytes of value zero.
 *
 * @param count number of objects to allocate
 * @param size size of the objects to allocate
 *
 * @return pointer to allocated memory / NULL pointer if there is an error
 */
void *os_calloc(size_t count, size_t size)
{
    void *p;

    OS_DEBUG_NOT_IN_INTERRUPT;

    /* allocate 'count' objects of size 'size' */
    p = os_malloc(count * size);

    /* zero the memory */
    if (p)
        memset(p, 0, count * size);

    return p;
}

/**
 * This function will release the previously allocated memory block by
 * os_malloc. The released memory block is taken back to system heap.
 *
 * @param rmem the address of memory which will be released
 */
void os_free(void *rmem)
{
    struct heap_mem *mem;

    OS_DEBUG_NOT_IN_INTERRUPT;

    if (rmem == NULL)
        return;
    OS_ASSERT((((uint32_t)rmem) & (OS_ALIGN_SIZE-1)) == 0);
    OS_ASSERT((uint8_t *)rmem >= (uint8_t *)heap_ptr &&
              (uint8_t *)rmem < (uint8_t *)heap_end);

    if ((uint8_t *)rmem < (uint8_t *)heap_ptr ||
        (uint8_t *)rmem >= (uint8_t *)heap_end) {
        OS_DEBUG_LOG(OS_DEBUG_HEAP, ("illegal memory\n"));

        return;
    }

    /* Get the corresponding struct heap_mem ... */
    mem = (struct heap_mem *)((uint8_t *)rmem - SIZEOF_STRUCT_MEM);

    OS_DEBUG_LOG(OS_DEBUG_HEAP,
                 ("release memory 0x%x, size: %d\n",
                  (uint32_t)rmem,
                  (uint32_t)(mem->next - ((uint8_t *)mem - heap_ptr))));

    /* protect the heap from concurrent access */
    os_sem_take(&heap_sem, OS_WAIT_FOREVER);

    /* ... which has to be in a used state ... */
    OS_ASSERT(mem->used);
    OS_ASSERT(mem->magic == HEAP_MAGIC);
    /* ... and is now unused. */
    mem->used  = 0;
    mem->magic = HEAP_MAGIC;

    if (mem < lfree) {
        /* the newly freed struct is now the lowest */
        lfree = mem;
    }

#ifdef OS_HEAP_STATS
    used_mem -= (mem->next - ((uint8_t*)mem - heap_ptr));
#endif

    /* finally, see if prev or next are free also */
    plug_holes(mem);
    os_sem_give(&heap_sem);
}

#ifdef OS_HEAP_STATS
void os_memory_info(uint32_t *total,
                    uint32_t *used,
                    uint32_t *max_used)
{
    if (total != NULL)
        *total = mem_size_aligned;
    if (used  != NULL)
        *used = used_mem;
    if (max_used != NULL)
        *max_used = max_mem;
}

#endif

/*@}*/

#endif /* end of OS_CFG_HEAP */
