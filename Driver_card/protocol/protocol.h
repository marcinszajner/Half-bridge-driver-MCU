#ifndef PROTCOL_H
#define PROTCOL_H

#include "adc_interface.h"
#include "protocols_types.h"

void execute_protocol(void* protocol_ptr);
void init_data_protocol_buffer(data_acq_con_ar_t* data_acq_con_ar);
void calculate_and_add_crc(void* protocol_ptr);

#endif
