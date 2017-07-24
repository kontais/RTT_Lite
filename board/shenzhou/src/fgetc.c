#include <stdio.h>
#include <board.h>

int fgetc(FILE *fp)
{
  int ch = 0;

  while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);

  ch = (int)USART2->DR & 0xFF;

  putchar(ch);

  return ch;
}
