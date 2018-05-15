/*
 * File      : led.h
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
#ifndef _LED_H_
#define _LED_H_

#define LED0_PORT       GPIOF
#define LED0_PIN        GPIO_Pin_8

#define LED1_PORT       GPIOF
#define LED1_PIN        GPIO_Pin_9

#define LED2_PORT       GPIOF
#define LED2_PIN        GPIO_Pin_10

void led_toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void led_init(void);

#endif /* _LED_H_ */
