#ifndef __MAIN_H
#define __MAIN_H

#include "stm32g4xx_hal.h"
#include "init_hrtim.h"
#include "hrtim_interface.h"
#include "usart_init.h"
#include "buffers.h"

#define HRTIM_INPUT_CLOCK 170000000
#define TIMA_PWM_FREQ 200000
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define USART1_TX_Pin GPIO_PIN_4
#define USART1_TX_GPIO_Port GPIOC
#define USART1_RX_Pin GPIO_PIN_5
#define USART1_RX_GPIO_Port GPIOC
#define T_SWDIO_Pin GPIO_PIN_13
#define T_SWDIO_GPIO_Port GPIOA
#define T_SWCLK_Pin GPIO_PIN_14
#define T_SWCLK_GPIO_Port GPIOA
#define T_SWO_Pin GPIO_PIN_3
#define T_SWO_GPIO_Port GPIOB

void Error_Handler(void);

#endif /* __MAIN_H */
