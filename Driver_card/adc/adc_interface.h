#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_

#include "stdbool.h"
#include "protocols_types.h"
#include "buffers.h"
#include <stdint.h>

#include "stm32g474xx.h"

typedef struct data_acq_con_ar_t
{
    bool request_active;
    bool data_ready_send;
    bool buffer_lock;
    uint32_t packet_size;
    uint32_t n_packet;
    uint32_t actual_packet;
    uint32_t actual_frequency;
    int32_t  fequency_step;
} data_acq_con_ar_t;

void init_data_acq_struct(protocol_data_req* req);
uint8_t* get_adc_data_orgin();
uint8_t* get_actual_data_buffer();
bool get_request_status();
bool is_acquisition_done();
void increment_actual_packet();
void set_request_as_done();
uint32_t get_packet_size();
uint16_t get_data_buffer_size();
int32_t get_frequency_step();
uint32_t get_actual_frequency();
void set_actual_frequency(uint32_t actual_frequency);
void set_buffer_lock(bool status);
bool get_buffer_lock();
HAL_StatusTypeDef prepare_acquisite_n_samples_with_dma(ADC_HandleTypeDef *hadc, uint32_t *pData, uint32_t Length);
void acquisite_samples();

#endif
