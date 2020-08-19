#include "main.h"
#include "stm32g4xx_ll_dma.h"
#include "rcc_init.h"
#include "dma_init.h"
#include "adc_init.h"
#include "crc.h"

#define TIMD_PWM_FREQ (uint32_t)(TIMA_PWM_FREQ/3)

/* Formula below works for PWM frequency > 83kHz (with prescaler ratio = 1) */
#define TIMA_PERIOD ((uint16_t)(((uint64_t)HRTIM_INPUT_CLOCK * 16) / TIMA_PWM_FREQ))

/* Formula below works for PWM frequency > 20.8kHz (with prescaler ratio = 4) */
#define TIMD_PERIOD ((uint16_t)(((uint64_t)HRTIM_INPUT_CLOCK * 4) / TIMD_PWM_FREQ))

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

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

uint16_t temp_array[30];

int main(void)
{
  rcc_init();
  MX_GPIO_Init();
  DC_HRTIM1_Init();
  dma_init();
  adc_init();
  USART2_DMA_Init();
  crc_init();

  run();

  while (1)
  {

    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    /* Insert delay 100 ms */
    HAL_Delay(100);

//    if (prepare_acquisite_n_samples_with_dma(&AdcHandle,
//                          (uint32_t *)&temp_array,
//                          15
//                         ) != HAL_OK)
//    {
//  	  while(0){}
//    }
//
//    volatile uint32_t a = 0;
//    hhrtim1.Instance->sMasterRegs.MICR |= HRTIM_MICR_MREP;
//    while(!(hhrtim1.Instance->sMasterRegs.MISR & HRTIM_MISR_MREP))
//    {
//      a++;
//    }
//
//    LL_ADC_REG_StartConversion(AdcHandle.Instance);

  }
}

void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
}
