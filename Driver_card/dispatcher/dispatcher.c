#include "dispatcher.h"

#include "hrtim_interface.h"
#include "usart_interface.h"
#include "adc_interface.h"
#include "crc.h"
#include "command_types.h"

#include <string.h>
#include <stdbool.h>

char Message_unknown[] = "unknown message\n";

static bool crc_check(void *ptr)
{
  head_t *cmd_ptr = (head_t*) ptr;
  uint32_t crc_ref = cmd_ptr->crc;
  cmd_ptr->crc = 0;
  volatile uint32_t crc_result = crc_calculate(ptr, cmd_ptr->size);
  return crc_ref == crc_result;
}

void execute_cmd()
{
  uint8_t* uart_rx_buff = get_uart_rx_buff();
  head_t *cmd_header = (head_t*)uart_rx_buff;
  if (crc_check(uart_rx_buff))
  {
    switch (cmd_header->type)
    {
    case UPDATE:
      if ((((cmd_change*) cmd_header)->bitfield & FREQUENCY_BIT)
          == FREQUENCY_BIT)
      {
        change_frequency(((cmd_change*) cmd_header)->payload[0]);
      }
    break;

    case DATA_REQ:
    {
      if (!get_data_request_status())
      {
        change_frequency(((cmd_data_req*) cmd_header)->actual_frequency);
        init_data_acq_struct((cmd_data_req*) cmd_header);
      }
    }
    break;

    default:
      uart1_send((uint8_t*) Message_unknown, strlen(Message_unknown));
    }
  }
  else
  {
    //TODO send inv crc resp
  }
}
