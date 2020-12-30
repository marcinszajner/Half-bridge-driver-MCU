#ifndef COMMON_MACROS_H_
#define COMMON_MACROS_H_

#define ARRAY_LEN(x)            (sizeof(x) / sizeof((x)[0]))

#define start_timer() \
  *((volatile uint32_t *)0xE0001000) = 0x40000001  // Enable CYCCNT register
#define stop_timer() \
  *((volatile uint32_t *)0xE0001000) = 0x40000000  // Disable CYCCNT register
#define get_timer() \
  *((volatile uint32_t *)0xE0001004)  // Get value from CYCCNT register

#endif
