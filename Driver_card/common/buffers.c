#include "buffers.h"

// TODO move some buffers to correspondent component
HRTIM_HandleTypeDef hhrtim1;
HRTIM_CompareCfgTypeDef compare_config;
HRTIM_ADCTriggerCfgTypeDef adc_trigger_config;

perypherial_state_t perypherial_state;
ADC_MultiModeTypeDef MultiModeConfig;
ADC_InjectionConfTypeDef InjectionConfig;
ADC_HandleTypeDef AdcHandle;

uint8_t scratchpad[100];

