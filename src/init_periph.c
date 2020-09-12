#include "init_periph.h"
#include "stm32g4xx_ll_dma.h"
#include "init_hrtim.h"
#include "usart_init.h"
#include "usart_interface.h"
#include "rcc_init.h"
#include "dma_init.h"
#include "adc_init.h"
#include "crc.h"
#include <string.h>

char AT[] = "AT+BAUD8"; // set baud rate to 115200

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

void init_periph()
{
  rcc_init();
  MX_GPIO_Init();
  DC_HRTIM1_Init();
  dma_init();
  adc_init();
  USART1_DMA_Init(9600); // HC-06 default baud rate
  uart1_send((uint8_t*) AT, strlen(AT));
  USART1_DMA_Init(115200); // reinicjalize with new baud rate
  crc_init();
}
