#include "msp_adc.h"
#include "buffers.h"
#include "stm32g4xx_hal.h"
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef          GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock ****************************************/
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* ADC2 Periph clock enable */
  __HAL_RCC_ADC12_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* Configure PA1 and PA3 (ADC1 Channel2 and 4) as analog inputs */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
