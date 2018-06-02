#ifndef _TIM3_H_
#define _TIM3_H_

void tim3_init(uint16_t period50us);
void tim3_register_callback(void (callback)(void));
void tim3_enable(void);
void tim3_disable(void);
void tim3_close(void);

#endif /* _TIM3_H_ */
