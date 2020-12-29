#ifndef USART_INTERFACE_H
#define USART_INTERFACE_H

#include "types.h"

void uart1_send(uint8_t *data, uint16_t size);
uint32_t is_usart1_idle_interrupt();
void reset_usart1_dma_idle_interrupt();

uint32_t is_usart1_dma_tx_tci();
void reset_usart1_dma_tx_tci();

uint32_t is_usart1_dma_rx_tci();
void reset_usart1_dma_rx_tci();



#endif
