#ifndef _NVRAM_H_
#define _NVRAM_H_

void nvram_init(void);

uint8_t nvram_read(uint32_t address, uint8_t *buffer, uint32_t length);
uint8_t nvram_write(uint32_t address, uint8_t *buffer, uint32_t length);
uint8_t nvram_store(void);
uint8_t nvram_erase(void);

#endif /* _NVRAM_H_ */
