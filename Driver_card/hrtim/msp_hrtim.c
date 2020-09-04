#include "msp_hrtim.h"

/**
 * @brief HRTIM MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hhrtim: HRTIM handle pointer
 * @retval None
 */
void HAL_HRTIM_MspInit(HRTIM_HandleTypeDef *hhrtim)
{
  if (hhrtim->Instance == HRTIM1)
  {
    /* Peripheral clock enable */
    __HAL_RCC_HRTIM1_CLK_ENABLE();

    HAL_NVIC_SetPriority(HRTIM1_TIMF_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(HRTIM1_TIMF_IRQn);
  }

}
