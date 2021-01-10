#include "msg_interface.h"
#include "msg_data.h"
#include "command_types.h"
#include "crc.h"
#include "usart_interface.h"

void init_data_req_msg(uint32_t packet_size,
                       uint32_t n_packet,
                       uint32_t frequency_step,
                       uint32_t actual_frequency)
{
  cmd_data_resp *adc_data_resp =
      (cmd_data_resp*) adc_data_acq_resp;
  adc_data_resp->head.signature = 0xAA;
  adc_data_resp->head.size = sizeof(cmd_data_resp)
      + packet_size * SAMPLE_SIZE * n_packet
      + END_SIGNATURE_SIZE + 2; // TODO check why + 2 needed
  adc_data_resp->head.type = DATA_RESP;
  adc_data_resp->head.crc = 0;
  adc_data_resp->actual_frequency = actual_frequency
      + (n_packet) * frequency_step;

  adc_data_resp->packet_size = packet_size;
  adc_data_resp->n_packet = n_packet;
  adc_data_resp->fequency_step = frequency_step;

  uint32_t *end_singature_ptr = (uint32_t*) &adc_data_resp->payload[(packet_size
      * SAMPLE_SIZE * n_packet) + 2];
  *end_singature_ptr = (uint32_t) 0x55555555;
}

uint16_t get_data_buffer_size()
{
  head_t *head = (head_t*) adc_data_acq_resp;
  uint16_t size = head->size;
  return size;
}

uint8_t* get_adc_data_acq_resp_buf()
{
  return adc_data_acq_resp;
}

void calculate_and_add_crc()
{
  head_t *head_ptr = (head_t*) adc_data_acq_resp;
  uint32_t size = head_ptr->size;
  uint32_t crc = crc_calculate((uint8_t*) adc_data_acq_resp, size);
  head_ptr->crc = crc;
}

void send_data_acq_resp()
{
  uint16_t buffer_size = get_data_buffer_size();
  uint8_t *buffer_data_ptr = get_adc_data_acq_resp_buf();
  calculate_and_add_crc();
  uart1_send(buffer_data_ptr, buffer_size);
}
