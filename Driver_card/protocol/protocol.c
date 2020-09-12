#include "protocol.h"
#include "stm32g4xx_hal_dma.h"
#include "stm32g4xx_hal_hrtim.h"
#include "hrtim_interface.h"
#include "usart_interface.h"
#include "buffers.h"
#include "crc.h"

#include <string.h>
#include "stdbool.h"

char Message_unknown[] = "unknown message\n";

static bool crc_check(void *ptr)
{
  head_t *protocol_ptr = (head_t*) ptr;
  uint32_t crc_ref = protocol_ptr->crc;
  protocol_ptr->crc = 0;
  volatile uint32_t crc_result = crc_calculate(ptr, protocol_ptr->size);
  return crc_ref == crc_result;
}

void execute_protocol(void *ptr)
{
  head_t *protocol_ptr = (head_t*) ptr;
  switch (protocol_ptr->signature)
  {
  case 0xaa:
    switch (protocol_ptr->type)
    {
    case UPDATE:
      if ((((protocol_change*) protocol_ptr)->bitfield & FREQUENCY_BIT)
          == FREQUENCY_BIT)
      {
        change_frequency(((protocol_change*) protocol_ptr)->payload[0]);
      }
      break;

    case DATA_REQ:
    {
      if (crc_check(ptr))
      {
        change_frequency(((protocol_data_req*) protocol_ptr)->actual_frequency);
        init_data_acq_struct((protocol_data_req*) protocol_ptr);
      } else
      {
        //TODO send inv crc resp
      }
    }

      break;
    default:
      uart1_send((uint8_t*) Message_unknown, strlen(Message_unknown));
    }
    break;

  default:
    uart1_send((uint8_t*) Message_unknown, strlen(Message_unknown));
  }
}

void init_data_protocol_buffer(data_acq_con_ar_t *data_acq_con_ar)
{
  protocol_data_resp *adc_data_resp =
      (protocol_data_resp*) get_adc_data_orgin();
  adc_data_resp->head.signature = 0xAA;
  adc_data_resp->head.size = sizeof(protocol_data_resp)
      + data_acq_con_ar->packet_size * SAMPLE_SIZE * data_acq_con_ar->n_packet
      + END_SIGNATURE_SIZE + 2; // TODO check why + 2 needed
  adc_data_resp->head.type = DATA_RESP;
  adc_data_resp->head.crc = 0;

  uint32_t packet_size = data_acq_con_ar->packet_size;
  uint32_t n_packet = data_acq_con_ar->n_packet;
  int32_t frequency_step = data_acq_con_ar->fequency_step;
  adc_data_resp->actual_frequency = data_acq_con_ar->actual_frequency
      + (n_packet) * frequency_step;

  adc_data_resp->packet_size = packet_size;
  adc_data_resp->n_packet = n_packet;
  adc_data_resp->fequency_step = frequency_step;

  uint32_t *end_singature_ptr = (uint32_t*) &adc_data_resp->payload[(packet_size
      * SAMPLE_SIZE * n_packet) + 2];
  *end_singature_ptr = (uint32_t) 0x55555555;
}

void calculate_and_add_crc(void *protocol_ptr)
{
  head_t *head_ptr = (head_t*) protocol_ptr;
  uint32_t size = head_ptr->size;
  uint32_t crc = crc_calculate((uint8_t*) protocol_ptr, size);
  head_ptr->crc = crc;
}
