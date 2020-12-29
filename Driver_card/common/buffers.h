#ifndef BUFFERS_H_
#define BUFFERS_H_

#include "stm32g4xx_hal.h"
#include "types.h"

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

extern perypherial_state_t perypherial_state;
extern ADC_MultiModeTypeDef MultiModeConfig;
extern ADC_InjectionConfTypeDef InjectionConfig;
extern ADC_HandleTypeDef AdcHandle;

extern uint8_t scratchpad[100];

#endif
