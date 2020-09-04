#ifndef HRTIM_INTERFACE_H
#define HRTIM_INTERFACE_H

#include "types.h"

void change_frequency(uint32_t frequency);
uint32_t get_frequency();
void run_hrtim_output(void);

#endif
