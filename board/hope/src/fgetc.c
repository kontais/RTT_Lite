#include <stdio.h>
#include <board.h>

int fgetc(FILE *fp)
{
  int ch = 0;

  while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

  ch = (int)USART1->DR & 0xFF;

  putchar(ch);

  return ch;
}
