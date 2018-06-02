/*-----------------------------------------------------------------------
* io.c  -
*
*
*
* Copyright (C) 2016 XCMG Group.
*
*-----------------------------------------------------------------------*/
#include <stm32f10x.h>
#include <io.h>

IO_CFG input_config[] = {
    {GPIOC, GPIO_Pin_6,  GPIO_Mode_IPD, 0x00},   /* 00 - PC6 */
    {GPIOG, GPIO_Pin_8,  GPIO_Mode_IPD, 0x00},   /* 01 - PG8 */
    {GPIOG, GPIO_Pin_7,  GPIO_Mode_IPD, 0x00},   /* 02 - PG7 */
    {GPIOG, GPIO_Pin_6,  GPIO_Mode_IPD, 0x00},   /* 03 - PG6 */
    {GPIOG, GPIO_Pin_5,  GPIO_Mode_IPD, 0x00},   /* 04 - PG5 */
    {GPIOG, GPIO_Pin_4,  GPIO_Mode_IPD, 0x00},   /* 05 - PG4 */
    {GPIOG, GPIO_Pin_3,  GPIO_Mode_IPD, 0x00},   /* 06 - PG3 */
    {GPIOG, GPIO_Pin_2,  GPIO_Mode_IPD, 0x00},   /* 07 - PG2 */
    {GPIOD, GPIO_Pin_15, GPIO_Mode_IPD, 0x00},   /* 08 - PD15 */
    {GPIOD, GPIO_Pin_14, GPIO_Mode_IPD, 0x00},   /* 09 - PD14 */
    {GPIOD, GPIO_Pin_13, GPIO_Mode_IPD, 0x00},   /* 10 - PD13*/
    {GPIOD, GPIO_Pin_12, GPIO_Mode_IPD, 0x00},   /* 11 - PD12 */
    {GPIOD, GPIO_Pin_11, GPIO_Mode_IPD, 0x00},   /* 12 - PD11 */
    {GPIOD, GPIO_Pin_10, GPIO_Mode_IPD, 0x00},   /* 13 - PD10 */
    {GPIOD, GPIO_Pin_9,  GPIO_Mode_IPD, 0x00},   /* 14 - PD9 */
    {GPIOD, GPIO_Pin_8,  GPIO_Mode_IPD, 0x00},   /* 15 - PD8 */
    {GPIOB, GPIO_Pin_15, GPIO_Mode_IPD, 0x00},   /* 16 - PB15 */
    {GPIOB, GPIO_Pin_14, GPIO_Mode_IPD, 0x00},   /* 17 - PB14 */
    {GPIOB, GPIO_Pin_13, GPIO_Mode_IPD, 0x00},   /* 18 - PB13 */
    {GPIOB, GPIO_Pin_12, GPIO_Mode_IPD, 0x00},   /* 19 - PB12 */
};

IO_CFG feedback_config[] = {
    {GPIOC, GPIO_Pin_11, GPIO_Mode_IPD, 0x00},   /* 00 - PC11 */
    {GPIOC, GPIO_Pin_12, GPIO_Mode_IPD, 0x00},   /* 01 - PC12 */
    {GPIOD, GPIO_Pin_0,  GPIO_Mode_IPD, 0x00},   /* 02 - PD0 */
    {GPIOD, GPIO_Pin_1,  GPIO_Mode_IPD, 0x00},   /* 03 - PD1 */
    {GPIOD, GPIO_Pin_2,  GPIO_Mode_IPD, 0x00},   /* 04 - PD2 */
    {GPIOD, GPIO_Pin_3,  GPIO_Mode_IPD, 0x00},   /* 05 - PD3 */
    {GPIOD, GPIO_Pin_4,  GPIO_Mode_IPD, 0x00},   /* 06 - PD4 */
    {GPIOD, GPIO_Pin_5,  GPIO_Mode_IPD, 0x00},   /* 07 - PD5 */
    {GPIOD, GPIO_Pin_6,  GPIO_Mode_IPD, 0x00},   /* 08 - PD6 */
    {GPIOD, GPIO_Pin_7,  GPIO_Mode_IPD, 0x00},   /* 09 - PD7 */
    {GPIOG, GPIO_Pin_9,  GPIO_Mode_IPD, 0x00},   /* 10 - PG9 */
    {GPIOG, GPIO_Pin_10, GPIO_Mode_IPD, 0x00},   /* 11 - PG10 */
    {GPIOG, GPIO_Pin_11, GPIO_Mode_IPD, 0x00},   /* 12 - PG11 */
    {GPIOG, GPIO_Pin_12, GPIO_Mode_IPD, 0x00},   /* 13 - PG12 */
    {GPIOG, GPIO_Pin_13, GPIO_Mode_IPD, 0x00},   /* 14 - PG13 */
    {GPIOG, GPIO_Pin_14, GPIO_Mode_IPD, 0x00},   /* 15 - PG14 */
    {GPIOB, GPIO_Pin_6,  GPIO_Mode_IPD, 0x00},   /* 16 - PB6 */
    {GPIOB, GPIO_Pin_7,  GPIO_Mode_IPD, 0x00},   /* 17 - PB7 */
};

IO_CFG output_config[] = {
    {GPIOB, GPIO_Pin_8,  GPIO_Mode_Out_PP, 0x00},   /* 00 - PB8 */
    {GPIOB, GPIO_Pin_9,  GPIO_Mode_Out_PP, 0x00},   /* 01 - PB9 */
    {GPIOE, GPIO_Pin_0,  GPIO_Mode_Out_PP, 0x00},   /* 02 - PE0 */
    {GPIOE, GPIO_Pin_1,  GPIO_Mode_Out_PP, 0x00},   /* 03 - PE1 */
    {GPIOE, GPIO_Pin_2,  GPIO_Mode_Out_PP, 0x00},   /* 04 - PE2 */
    {GPIOE, GPIO_Pin_3,  GPIO_Mode_Out_PP, 0x00},   /* 05 - PE3 */
    {GPIOE, GPIO_Pin_4,  GPIO_Mode_Out_PP, 0x00},   /* 06 - PE4 */
    {GPIOE, GPIO_Pin_5,  GPIO_Mode_Out_PP, 0x00},   /* 07 - PE5 */
    {GPIOE, GPIO_Pin_6,  GPIO_Mode_Out_PP, 0x00},   /* 08 - PE6 */
    {GPIOC, GPIO_Pin_13, GPIO_Mode_Out_PP, 0x00},   /* 09 - PC13 */
    {GPIOC, GPIO_Pin_14, GPIO_Mode_Out_PP, 0x00},   /* 10 - PC14 */
    {GPIOC, GPIO_Pin_15, GPIO_Mode_Out_PP, 0x00},   /* 11 - PC15 */
    {GPIOF, GPIO_Pin_0,  GPIO_Mode_Out_PP, 0x00},   /* 12 - PF0 */
    {GPIOF, GPIO_Pin_1,  GPIO_Mode_Out_PP, 0x00},   /* 13 - PF1 */
    {GPIOF, GPIO_Pin_2,  GPIO_Mode_Out_PP, 0x00},   /* 14 - PF2 */
    {GPIOF, GPIO_Pin_3,  GPIO_Mode_Out_PP, 0x00},   /* 15 - PF3 */
    {GPIOF, GPIO_Pin_4,  GPIO_Mode_Out_PP, 0x00},   /* 16 - PF4 */
    {GPIOF, GPIO_Pin_5,  GPIO_Mode_Out_PP, 0x00},   /* 17 - PF5 */
};

uint16_t input_count    = sizeof (input_config) / sizeof(IO_CFG);
uint16_t feedback_count = sizeof (feedback_config) / sizeof(IO_CFG);
uint16_t output_count   = sizeof (output_config) / sizeof(IO_CFG);

void io_init(void)
{
    int count;
    IO_CFG *iocfg;

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);

    //
    // input io init
    //
    count = input_count;
    iocfg = &input_config[0];

    while(count--) {
        GPIO_InitStructure.GPIO_Pin   = iocfg->pin;
        GPIO_InitStructure.GPIO_Mode  = iocfg->mode;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_Init(iocfg->port, &GPIO_InitStructure);
        iocfg++;
    }

    //
    // output feedback io init
    //
    count = feedback_count;
    iocfg = &feedback_config[0];

    while(count--) {
        GPIO_InitStructure.GPIO_Pin   = iocfg->pin;
        GPIO_InitStructure.GPIO_Mode  = iocfg->mode;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_Init(iocfg->port, &GPIO_InitStructure);
        iocfg++;
    }

    //
    // output io init
    //
    count = output_count;
    iocfg = &output_config[0];

    while(count--) {
        //
        // 先配置输出值
        //
        if (iocfg->record == 1) {
            GPIO_SetBits(iocfg->port, iocfg->pin);
        }

        if (iocfg->record == 0) {
            GPIO_ResetBits(iocfg->port, iocfg->pin);
        }

        //
        // 再将IO设置为输出
        //
        GPIO_InitStructure.GPIO_Pin   = iocfg->pin;
        GPIO_InitStructure.GPIO_Mode  = iocfg->mode;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_Init(iocfg->port, &GPIO_InitStructure);
        iocfg++;
    }
}
