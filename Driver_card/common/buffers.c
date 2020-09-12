#include "buffers.h"

// TODO move some buffers to correspondent component
HRTIM_HandleTypeDef hhrtim1;
HRTIM_CompareCfgTypeDef compare_config;
HRTIM_ADCTriggerCfgTypeDef adc_trigger_config;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

perypherial_state_t perypherial_state;
ADC_MultiModeTypeDef MultiModeConfig;
ADC_InjectionConfTypeDef InjectionConfig;
ADC_HandleTypeDef AdcHandle;

DMA_HandleTypeDef hdma_adc1;

uint8_t usart_rx_dma_buffer[64];

uint8_t scratchpad[100];

