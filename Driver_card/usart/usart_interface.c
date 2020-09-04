#include "usart_interface.h"
#include "usart_error_handle.h"
#include "stm32g4xx_hal_dma.h"
#include "stm32g4xx_hal_hrtim.h"
#include "stm32g4xx_hal_uart.h"
#include "buffers.h"

void uart_send(uint8_t *data, uint16_t size)
{
  if (HAL_UART_Transmit_DMA(&huart2, data, size) != HAL_OK)
  {
    /* Transfer error in transmission process */
    usart_error_handler();
  }
}
