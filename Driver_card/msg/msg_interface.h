#ifndef MSG_INTERFACE_H_
#define MSG_INTERFACE_H_

#include <stdint.h>

void init_data_req_msg(uint32_t packet_size,
                       uint32_t n_packet,
                       uint32_t fequency_step,
                       uint32_t actual_frequency);
uint16_t get_data_buffer_size();
uint8_t* get_adc_data_acq_resp_buf();
void calculate_and_add_crc();
void send_data_acq_resp();

#endif
