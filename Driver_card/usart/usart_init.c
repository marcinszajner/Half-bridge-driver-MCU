#include "usart_init.h"
#include "usart_error_handle.h"
#include "usart_structs.h"
#include "common_macros.h"
#include "stm32g474xx.h"
#include "stm32g4xx_hal_cortex.h"

void USART1_DMA_Init(uint32_t baudRate)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart1ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    usart_error_handler();
  }

  huart1.Instance = USART1;
  huart1.Init.BaudRate = baudRate;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  HAL_UART_MspInit(&huart1);

  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    usart_error_handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8)
      != HAL_OK)
  {
    usart_error_handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8)
      != HAL_OK)
  {
    usart_error_handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    usart_error_handler();
  }

  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

  if (HAL_UART_Receive_DMA(&huart1, (uint8_t*) usart_rx_dma_buffer,
      ARRAY_LEN(usart_rx_dma_buffer)) != HAL_OK)
  {
    /* Transfer error in reception process */
    usart_error_handler();
  }

  //perypherial_state.uart_state.uart_status = RUNNING;
}
