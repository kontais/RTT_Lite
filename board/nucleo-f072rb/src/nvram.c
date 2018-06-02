#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include <wdog.h>

#define FLASH_PAGE_SIZE    0x800        // 每页2K Bytes
#define NVRAM_FLASH_BASE   0x08000000
#define NVRAM_FLASH_PAGE   255          // 最后一页
#define NVRAM_FLASH_SIZE   (FLASH_PAGE_SIZE * 1)
#define NVRAM_FLASH_ADDR   (NVRAM_FLASH_BASE + NVRAM_FLASH_PAGE * FLASH_PAGE_SIZE) // == 0x0807F800

//static __attribute__((aligned(4))) uint8_t nvram_ram[NVRAM_FLASH_SIZE];
__attribute__((aligned(4))) uint8_t nvram_ram[NVRAM_FLASH_SIZE];

void nvram_load(void)
{
    memcpy(nvram_ram, (const void *)NVRAM_FLASH_ADDR, NVRAM_FLASH_SIZE);
}

uint8_t nvram_store(void)
{
    uint32_t i, *data;
    volatile FLASH_Status status;

    FLASH_Unlock();

    //
    // 擦除
    //
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    status = FLASH_ErasePage(NVRAM_FLASH_ADDR);
    if (status != FLASH_COMPLETE) {
        FLASH_Lock();
        return 1;
    }

    data = (uint32_t *)nvram_ram;

    for (i = 0; i < NVRAM_FLASH_SIZE; i += 4) {
        status = FLASH_ProgramWord(NVRAM_FLASH_ADDR + i, *data++);
        if (status != FLASH_COMPLETE) {
            FLASH_Lock();
            return 1;
        }
    }

    FLASH_Lock();
    
    wdog_feed();

    return 0;
}

uint8_t nvram_erase(void)
{
    volatile FLASH_Status status;

    FLASH_Unlock();

    //
    // 擦除
    //
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    status = FLASH_ErasePage(NVRAM_FLASH_ADDR);
    if (status != FLASH_COMPLETE) {
        FLASH_Lock();
        return 1;
    }

    FLASH_Lock();
    
    wdog_feed();

    return 0;
}

uint8_t nvram_read(uint32_t address, uint8_t *buffer, uint32_t length)
{
    uint32_t len;

    if (buffer == NULL) {
        return 1;
    }

    if (address > NVRAM_FLASH_SIZE) {
        return 2;
    }

    len = length;
    if ((address + len) > NVRAM_FLASH_SIZE) {
        len = NVRAM_FLASH_SIZE - address;
    }

    memcpy(buffer, &nvram_ram[address], len);

    return 0;
}

uint8_t nvram_write(uint32_t address, uint8_t *buffer, uint32_t length)
{
    uint32_t len;
    volatile FLASH_Status status;

    if (buffer == NULL) {
        return 1;
    }

    if (address > NVRAM_FLASH_SIZE) {
        return 2;
    }

    len = length;
    if ((address + len) > NVRAM_FLASH_SIZE) {
        len = NVRAM_FLASH_SIZE - address;
    }

    memcpy(&nvram_ram[address], buffer, len);

    //while (nvram_store());

    return 0;
}

void nvram_init(void)
{
    nvram_load();
}

#define TEST_DATA_SIZE  2048
uint8_t test_wdata[TEST_DATA_SIZE];
uint8_t test_rdata[TEST_DATA_SIZE];


void nvram_test(void)
{
    int i;
    uint8_t status, fail;


    nvram_init();

    for (i = 0; i < TEST_DATA_SIZE; i++) {
        test_wdata[i] = 0x33;
        test_rdata[i] = 0xaa;
    }

    status = nvram_write(0x200, test_wdata, 0x200);
    printf("write status = %d\n", status);

    status = nvram_read(0x200, test_rdata, 0x200);
    printf("read status = %d\n", status);


    fail = 0;
    for (i = 0; i < 0x200; i++) {
        if (test_wdata[i] != test_rdata[i]) {
            fail = 1;
            break;
        }
    }

    printf("fail = %d\n", fail);
}


