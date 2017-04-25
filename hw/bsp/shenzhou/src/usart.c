/*
 * File      : usart.c
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
#include <mcu/stm32f10x.h>

void USART_RCC_Configuration(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
}

void USART_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);

    //
    // PD5 USART2_TX  (Alternate functions Remap)
    //
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOD, &GPIO_InitStruct);

    //
    // PD6 USART2_RX  (Alternate functions Remap)
    //
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6;

    GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void USART_DMA_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  //
  // 发送通道配置
  //
  DMA_DeInit(DMA1_Channel7);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;       // 外设地址为USART2 Data Register
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;       // 方向由内存到外设
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;   // 外设地址不变化
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;        // 内存递增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据宽度为字节
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;     // 外设数据宽度为字节
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;             // 一次传输
  DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;         // 优先级：中
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;             // 禁止内存到内存传输
  DMA_Init(DMA1_Channel7, &DMA_InitStructure);

  //
  // 接收通道配置
  //
  DMA_DeInit(DMA1_Channel6);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_Init(DMA1_Channel6, &DMA_InitStructure);
}

void bsp_usart_init(void)
{
    USART_InitTypeDef USART_InitStructure;

    USART_RCC_Configuration();
    USART_GPIO_Configuration();

  // USART_DMA_Configuration();

  //
  // USART2 参数配置
  //
  USART_InitStructure.USART_BaudRate   = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits   = USART_StopBits_1;
  USART_InitStructure.USART_Parity     = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART2, &USART_InitStructure);
  USART_Cmd(USART2, ENABLE);

  USART_GetFlagStatus(USART2, USART_FLAG_TC); // Read clear status register
}

void bsp_usart_output(const char *str)
{
    char ch;

    ch = *str++;
    while (ch) {
        if (ch == '\n') {
            USART_SendData(USART2, '\r');
            while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
        }

        USART_SendData(USART2, ch);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

        ch = *str++;
    }
}
