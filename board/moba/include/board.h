/*
 * File      : board.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2013, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-09-22     Bernard      add board.h to this bsp
 */

// <<< Use Configuration Wizard in Context Menu >>>
#ifndef _BOARD_H_
#define _BOARD_H_

#define LED0_PORT       GPIOF
#define LED0_PIN        GPIO_Pin_0

#define LED1_PORT       GPIOF
#define LED1_PIN        GPIO_Pin_1

#define LED2_PORT       GPIOF
#define LED2_PIN        GPIO_Pin_2

void board_init(void);

#endif  /* _BOARD_H_ */
