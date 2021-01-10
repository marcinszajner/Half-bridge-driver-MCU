#include "stm32g4xx_it.h"

#include "dispatcher.h"
#include "adc_interface.h"
#include "common_macros.h"
#include "stdbool.h"
#include "usart_interface.h"
#include "msg_interface.h"



void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  while(1)
  {
  }
}

void MemManage_Handler(void)
{
  while(1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}

void EXTI15_10_IRQHandler(void)
{
}

void DMA1_Channel1_IRQHandler(void)
{
  if (is_usart1_dma_tx_tci())
  {
    reset_usart1_dma_tx_tci();
  }
}

void DMA1_Channel2_IRQHandler(void)
{
  if (is_usart1_dma_rx_tci())
  {
    reset_usart1_dma_rx_tci();
  }
}

void DMA1_Channel3_IRQHandler(void)
{
  if (is_adc1_dma_tci())
  {
    reset_adc_dma_tci();
    packet_acq_complete_handler();
  }
}

void USART1_IRQHandler(void)
{
  if (is_usart1_idle_interrupt())
  {
    //recived data is cmd and it is in uart recive buffer, copied by hardware
    execute_cmd();
    reset_usart1_dma_idle_interrupt();
  }
}
