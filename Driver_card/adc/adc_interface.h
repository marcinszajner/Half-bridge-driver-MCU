#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_

#include "stdbool.h"
#include "buffers.h"
#include <stdint.h>

#include "command_types.h"
#include "stm32g474xx.h"

void init_data_acq_struct(cmd_data_req* req);
uint8_t* get_actual_data_buffer();
bool get_data_request_status();
bool is_acquisition_done();
void increment_actual_packet();
void set_request_as_done();
uint32_t get_packet_size();
int32_t get_frequency_step();
uint32_t get_actual_frequency();
void set_actual_frequency(uint32_t actual_frequency);
HAL_StatusTypeDef prepare_acquisite_n_samples_with_dma(ADC_HandleTypeDef *hadc, uint32_t *pData, uint32_t Length);
void acquisite_samples();

uint32_t is_adc1_dma_tci();
void reset_adc_dma_tci();
void packet_acq_complete_handler();

#endif
