#include "usart_init.h"
#include "usart_error_handle.h"
#include "buffers.h"
#include "stm32g4xx_hal_rcc_ex.h"
#include "stm32g4xx_hal_rcc.h"
#include "stm32g474xx.h"
#include "stm32g4xx_hal_cortex.h"

void USART2_DMA_Init(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    usart_error_handler();
  }

  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 921600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  HAL_UART_MspInit(&huart2);

  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    usart_error_handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8)
      != HAL_OK)
  {
    usart_error_handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8)
      != HAL_OK)
  {
    usart_error_handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    usart_error_handler();
  }

  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);

  if (HAL_UART_Receive_DMA(&huart2, (uint8_t*) usart_rx_dma_buffer,
      ARRAY_LEN(usart_rx_dma_buffer)) != HAL_OK)
  {
    /* Transfer error in reception process */
    usart_error_handler();
  }

  perypherial_state.uart_state.uart_status = RUNNING;
}
