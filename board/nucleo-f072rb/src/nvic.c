/*-----------------------------------------------------------------------
* nvic.c  -
*
*
*
* Copyright (C) 2016 XCMG Group.
*
*-----------------------------------------------------------------------*/
#include "board.h"

void nvic_init(void)
{
  //
  // 四个组优先级(pre-emption priority)，可抢断，组优先级值越小优先级越高
  // 四个组内子优先级(subpriority)，同一个组内的中断不可推断，同时有中断时，
  // 子优先级值小的先处理
  //
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
}
