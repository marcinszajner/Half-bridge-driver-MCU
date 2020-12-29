#ifndef USART_STRUCTS_H
#define USART_STRUCTS_H

#include "stm32g4xx_hal.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

extern uint8_t usart_rx_dma_buffer[64];

#endif
