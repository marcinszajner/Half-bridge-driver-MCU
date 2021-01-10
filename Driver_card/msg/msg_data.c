#include "msg_data.h"

uint8_t adc_data_acq_resp[sizeof(cmd_data_resp)
    + (MAX_ADC_PACKET_SIZE * SAMPLE_SIZE * MAX_NUMBER_OF_PACKET)
    + END_SIGNATURE_SIZE]; // header+payload(256 sample *2 byte * max of packet)+0x55555555
