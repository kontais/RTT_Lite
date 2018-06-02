#ifndef _OS_ARHC_H_
#define _OS_ARHC_H_

uint32_t os_enter_critical(void);
void os_exit_critical(uint32_t isr_ctx);

#endif /* _OS_ARHC_H_ */

