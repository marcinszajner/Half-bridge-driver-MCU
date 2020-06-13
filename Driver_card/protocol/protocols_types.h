#ifndef PROTOCOLS_TYPES_H_
#define PROTOCOLS_TYPES_H_

#include <stdint.h>

//bitfields
#define FREQUENCY_BIT (1 << 31)

//type
#define UPDATE    (0x1)
#define DATA_REQ  (0x2)
#define DATA_RESP (0x3)

#define MAX_ADC_PACKET_SIZE  256
#define MAX_NUMBER_OF_PACKET 50
#define END_SIGNATURE_SIZE   4
#define SAMPLE_SIZE          2

typedef struct head_t
{
    uint8_t signature;
    uint8_t type;
    uint16_t size;
    uint32_t crc;
} __attribute__ ((packed)) head_t;

typedef struct protocol_change
{
    head_t head;
    uint32_t bitfield;
    uint32_t payload[];
} __attribute__ ((packed)) protocol_change;

typedef struct protocol_data_req
{
    head_t head;
    uint32_t  packet_size;
    uint32_t  n_packet;
    int32_t   fequency_step;
    uint32_t  actual_frequency;
}__attribute__ ((packed)) protocol_data_req;

typedef struct protocol_data_resp
{
    head_t head;
    uint32_t  packet_size;
    uint32_t  n_packet;
    int32_t   fequency_step;
    uint32_t  actual_frequency;
    uint8_t payload[];
}__attribute__ ((packed)) protocol_data_resp;

#endif
