#ifndef MSG_DATA_H_
#define MSG_DATA_H_

#include "command_types.h"
#include <stdint.h>

extern uint8_t adc_data_acq_resp[sizeof(cmd_data_resp)
                                 + (MAX_ADC_PACKET_SIZE * SAMPLE_SIZE * MAX_NUMBER_OF_PACKET)
                                 + END_SIGNATURE_SIZE]; // header+payload(256 sample *2 byte * max of packet) + 4 byte (signature)

#endif
