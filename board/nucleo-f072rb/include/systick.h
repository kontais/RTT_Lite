/*-----------------------------------------------------------------------
* systick.h -
*
*
*
* Copyright (C) 2018 kontais@aliyun.com
*
*-----------------------------------------------------------------------*/
#ifndef _SYSTICK_H_
#define _SYSTICK_H_

void systick_init(void);
extern volatile uint32_t g_system_tick;

#endif  /* _SYSTICK_H_ */
