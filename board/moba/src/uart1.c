#include "stm32f4xx.h"
#include <stdio.h>

extern void Delay(__IO uint32_t nTime);

void uart1_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  //
  // USART1 Clock
  //
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

  //
  // PA9 USART1 TX
  //
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //
  // PB7 USART1 RX
  //
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


  //
  // 115200-8-N-1
  //
  USART_InitStructure.USART_BaudRate   = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits   = USART_StopBits_1;
  USART_InitStructure.USART_Parity     = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);

  USART_Cmd(USART1, ENABLE);

  //
  // Read Clear Status Register
  //
  USART_GetFlagStatus(USART1, USART_FLAG_TC);

}

int fputc(int ch, FILE *f)
{
  if (ch == '\n') {
    USART_SendData(USART1, '\r');
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
  }

  USART_SendData(USART1, (uint8_t) ch);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

  return ch;
}

int fgetc(FILE *fp)
{
  int ch = 0;

  while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

  ch = (int)USART1->DR & 0xFF;

  putchar(ch);

  return ch;
}

void uart1_test1(void)
{
  USART_SendData(USART1, 'a');

  /* Loop until the end of transmission */
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

int uart1_test_cnt;

void uart1_test(void)
{
  printf("uart1 ok %d\n", uart1_test_cnt++);
}
