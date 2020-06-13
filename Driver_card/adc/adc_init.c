#include "adc_init.h"
#include "adc_error_handle.h"
#include "stm32g4xx_hal.h"
#include "buffers.h"
#include "msp_adc.h"
#include "protocol.h"

void adc_init(void)
{
  AdcHandle.Instance = ADC1;
  ADC_MultiModeTypeDef multimode = {0};

  HAL_ADC_MspInit(&AdcHandle);

  /* ADC1 global initialization */
  /* 12-bit right-aligned format, discontinuous scan mode, running from PLL */
  AdcHandle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  AdcHandle.Init.Resolution = ADC_RESOLUTION_12B;
  AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ScanConvMode = ADC_SCAN_ENABLE;
  AdcHandle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  AdcHandle.Init.LowPowerAutoWait = DISABLE;
  AdcHandle.Init.ContinuousConvMode = DISABLE;
  AdcHandle.Init.NbrOfConversion = 1;                           // set number of conversation if number of active RANK's are changed
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle.Init.NbrOfDiscConversion = 1;
  AdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.DMAContinuousRequests = DISABLE;
  AdcHandle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  AdcHandle.Init.OversamplingMode = DISABLE;
  HAL_ADC_Init(&AdcHandle);

  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&AdcHandle, &multimode) != HAL_OK)
  {
    adc_error_handler();
  }

  /* Discontinuous injected mode: 1st injected conversion for Vin on Ch2 */
  InjectionConfig.AutoInjectedConv = DISABLE;
  InjectionConfig.ExternalTrigInjecConv = ADC_EXTERNALTRIGINJEC_HRTIM_TRG2;
  InjectionConfig.ExternalTrigInjecConvEdge = ADC_EXTERNALTRIGINJECCONV_EDGE_RISING;
  InjectionConfig.InjectedChannel = ADC_CHANNEL_2;              // ADC2_CH2 PA1
  InjectionConfig.InjectedDiscontinuousConvMode = DISABLE;
  InjectionConfig.InjectedNbrOfConversion = 1;
  InjectionConfig.InjectedOffset = 0;
  InjectionConfig.InjectedOffsetNumber = ADC_OFFSET_NONE;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_1;
  InjectionConfig.InjectedSamplingTime = ADC_SAMPLETIME_12CYCLES_5;
  InjectionConfig.InjectedSingleDiff = ADC_SINGLE_ENDED;
  InjectionConfig.QueueInjectedContext = DISABLE;
  HAL_ADCEx_InjectedConfigChannel(&AdcHandle, &InjectionConfig);

  /* Configure and enable ADC1_2_IRQHandler interrupt channel in NVIC */
  HAL_NVIC_SetPriority(ADC1_2_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(ADC1_2_IRQn);

  /* Run the ADC calibration in single-ended mode */
  HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED);

  /* Start ADC1 Injected Conversions */
  HAL_ADCEx_InjectedStart_IT(&AdcHandle);
}
