#include "board.h"
#include <stdio.h>

///* USART2 */
//int fputc(int ch, FILE *f)
//{
//    if (ch == '\n') {
//        USART_SendData(USART2, '\r');
//        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
//    }

//    USART_SendData(USART2, (uint8_t) ch);
//    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		

//    return ch;
//}

/* USART1 */
int fputc(int ch, FILE *f)
{
    if (ch == '\n') {
        USART_SendData(USART1, '\r');
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }

    USART_SendData(USART1, (uint8_t) ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		

    return ch;
}
