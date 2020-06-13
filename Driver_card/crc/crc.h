#ifndef CRC_H_
#define CRC_H_

#include <stdint.h>

void crc_init(void);
uint32_t crc_calculate(uint8_t* buffer_ptr, uint32_t buffer_size);

#endif
