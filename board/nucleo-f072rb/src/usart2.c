#include "board.h"
#include "usart2.h"

void usart2_nvic_init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USART Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void usart2_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    /* Enable UART clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* Connect PXx to USART2_Tx*/
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);

    /* Connect PXx to USART2_Rx*/
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2  ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART2 mode config */
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_Even;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure); 

    usart2_nvic_init();

    USART_Cmd(USART2, ENABLE);

    USART_ClearFlag(USART2, USART_FLAG_TC);
}

//    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
//    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
//    USART_ITConfig(USART2, USART_IT_TXE, DISABLE);

//void USART2_IRQHandler(void)
//{
//    uint16_t c;

//    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) {
//        c = USART_ReceiveData(USART2);
//        USART_SendData(USART2, c);
//    }

//    if (USART_GetITStatus(USART2, USART_IT_TXE) == SET) {
////        USART_SendData(USART2, 'c');
//    }
//}
