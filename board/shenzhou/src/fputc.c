#include <stdio.h>
#include <board.h>

int fputc(int ch, FILE *f)
{
  if (ch == '\n') {
    USART_SendData(USART2, '\r');
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
  }

  USART_SendData(USART2, (uint8_t) ch);
  while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

  return ch;
}
