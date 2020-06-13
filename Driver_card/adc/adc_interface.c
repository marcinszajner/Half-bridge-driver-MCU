#include "adc_interface.h"
#include "protocol.h"

uint8_t adc_data_buffer[sizeof(protocol_data_resp)+(MAX_ADC_PACKET_SIZE*SAMPLE_SIZE*MAX_NUMBER_OF_PACKET)+END_SIGNATURE_SIZE]; // header+payload(256 sample *2 byte * max of packet)+0x55555555

data_acq_con_ar_t data_acq_con_ar;

uint8_t* get_adc_data_orgin()
{
    return adc_data_buffer;
}

void init_data_acq_struct(init_data_acq_t* init_data_acq)
{
    data_acq_con_ar.fequency_step = init_data_acq->fequency_step;
    data_acq_con_ar.n_packet = init_data_acq->n_packet;
    data_acq_con_ar.packet_size = init_data_acq->packet_size;
    data_acq_con_ar.actual_frequency = init_data_acq->actual_frequency;
    data_acq_con_ar.actual_packet = 0;
    data_acq_con_ar.data_ready_send = false;
    data_acq_con_ar.request_active = true;

    init_data_protocol_buffer(&data_acq_con_ar);
}

uint8_t* get_next_data_buffer()
{
    return &adc_data_buffer[sizeof(protocol_data_resp)+data_acq_con_ar.packet_size*SAMPLE_SIZE*data_acq_con_ar.actual_packet];
}

bool get_request_status()
{
    return data_acq_con_ar.request_active;
}

bool is_acquisition_done()
{
    return (data_acq_con_ar.actual_packet == data_acq_con_ar.n_packet);
}

void increment_actual_packet()
{
    data_acq_con_ar.actual_packet++;
}

void set_request_as_done()
{
    data_acq_con_ar.request_active = false;
}

uint32_t get_packet_size()
{
    return data_acq_con_ar.packet_size;
}

uint16_t get_data_buffer_size()
{
    head_t* head = (head_t*)adc_data_buffer;
    uint16_t size = head->size;
    return size;
}

int32_t get_frequency_step()
{
    return data_acq_con_ar.fequency_step;
}

uint32_t get_actual_frequency()
{
    return data_acq_con_ar.actual_frequency;
}

void set_actual_frequency(uint32_t actual_frequency)
{
    data_acq_con_ar.actual_frequency = actual_frequency;
}
