#include "usart_interface.h"
#include "usart_error_handle.h"
#include "common_macros.h"
#include "stm32g474xx.h"
#include "stm32g4xx_hal_dma.h"
#include "stm32g4xx_hal_uart.h"
#include "usart_structs.h"

void uart1_send(uint8_t *data, uint16_t size)
{
  if (HAL_UART_Transmit_DMA(&huart1, data, size) != HAL_OK)
  {
    /* Transfer error in transmission process */
    usart_error_handler();
  }
}

uint32_t is_usart1_idle_interrupt()
{
  return (READ_BIT(huart1.Instance->ISR, USART_ISR_IDLE) == (USART_ISR_IDLE)) ? 1UL : 0UL;
}

void reset_usart1_dma_idle_interrupt()
{
  huart1.gState = HAL_UART_STATE_READY;
  huart1.Instance->ICR = USART_ICR_IDLECF;
  huart1.hdmarx->Instance->CCR &= ~DMA_CCR_EN;
  huart1.hdmarx->Instance->CMAR = (intptr_t) usart_rx_dma_buffer;
  huart1.hdmarx->Instance->CNDTR = ARRAY_LEN(usart_rx_dma_buffer);
  huart1.hdmarx->Instance->CCR |= DMA_CCR_EN;
}

uint32_t is_usart1_dma_tx_tci()
{
  return (READ_BIT(hdma_usart1_tx.DmaBaseAddress->ISR, DMA_ISR_TCIF1) == (DMA_ISR_TCIF1)) ? 1UL : 0UL;
}

void reset_usart1_dma_tx_tci()
{
  hdma_usart1_tx.State = HAL_DMA_STATE_READY;
  huart1.gState = HAL_UART_STATE_READY;
  __HAL_UNLOCK(&hdma_usart1_tx);
  hdma_usart1_tx.DmaBaseAddress->IFCR = DMA_IFCR_CTCIF1;
}

uint32_t is_usart1_dma_rx_tci()
{
  return (READ_BIT(hdma_usart1_rx.DmaBaseAddress->ISR, DMA_ISR_TCIF2) == (DMA_ISR_TCIF1)) ? 1UL : 0UL;
}

void reset_usart1_dma_rx_tci()
{
  hdma_usart1_rx.DmaBaseAddress->IFCR = DMA_IFCR_CTCIF2;
}

uint8_t* get_uart_rx_buff()
{
  return usart_rx_dma_buffer;
}
