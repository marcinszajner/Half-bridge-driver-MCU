#ifndef BUFFERS_H_
#define BUFFERS_H_

#include "stm32g4xx_hal.h"
#include "types.h"

#define ARRAY_LEN(x)            (sizeof(x) / sizeof((x)[0]))

typedef struct uart_state_t
{
    status_t uart_status;
} uart_state_t;

typedef struct hrtim_state_t
{
    status_t hrtim_status;
} hrtim_state_t;

typedef struct perypherial_state_t
{
    uart_state_t  uart_state;
    hrtim_state_t hrtim_state;

} perypherial_state_t;


extern HRTIM_HandleTypeDef hhrtim1;
extern HRTIM_CompareCfgTypeDef compare_config;
extern HRTIM_ADCTriggerCfgTypeDef adc_trigger_config;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern perypherial_state_t perypherial_state;
extern ADC_MultiModeTypeDef MultiModeConfig;
extern ADC_InjectionConfTypeDef InjectionConfig;
extern ADC_HandleTypeDef AdcHandle;

extern uint8_t usart_rx_dma_buffer[64];
extern uint8_t scratchpad[100];

extern DMA_HandleTypeDef hdma_adc1;

#endif
