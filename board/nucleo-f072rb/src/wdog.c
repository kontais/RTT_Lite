#include "board.h"
//
// LSI = 32KHz
//
void wdog_init(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(4);   // prescaler = 4, ·ÖÆµÒò×Ó = 4 * 2 ^ prescaler = 64
    IWDG_SetReload(500);    // reload value = 500,  500 * (1 / (32KHz / 64)) = 1s
    IWDG_ReloadCounter();   // reload
    IWDG_Enable();
}

void wdog_feed(void)
{
    IWDG_ReloadCounter();   // reload
}
