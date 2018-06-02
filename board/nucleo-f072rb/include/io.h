/*-----------------------------------------------------------------------
* io.h  -
*
*
*
* Copyright (C) 2016 XCMG Group.
*
*-----------------------------------------------------------------------*/
#ifndef _IO_H_
#define _IO_H_

typedef struct {
    GPIO_TypeDef*      port;
    uint16_t           pin;
    GPIOMode_TypeDef mode;
    uint32_t         record;
    int16_t          status;
    uint32_t         last_change_tick;
} IO_CFG;

extern IO_CFG input_config[];
extern IO_CFG feedback_config[];
extern IO_CFG output_config[];
extern uint16_t input_count;
extern uint16_t feedback_count;
extern uint16_t output_count;

#endif /* _IO_H_ */
