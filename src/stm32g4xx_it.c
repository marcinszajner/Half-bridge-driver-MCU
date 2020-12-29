#include "stm32g4xx_it.h"

#include "crc.h"
#include "adc_interface.h"
#include "hrtim_interface.h"
#include "init_hrtim.h"
#include "protocol.h"
#include "common_macros.h"
#include "stdbool.h"
#include "stm32g4xx_ll_dma.h"
#include "usart_interface.h"

// TODO create proffiling module
#define start_timer() \
  *((volatile uint32_t *)0xE0001000) = 0x40000001  // Enable CYCCNT register
#define stop_timer() \
  *((volatile uint32_t *)0xE0001000) = 0x40000000  // Disable CYCCNT register
#define get_timer() \
  *((volatile uint32_t *)0xE0001004)  // Get value from CYCCNT register

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

//   proffiling
//   volatile uint32_t it1, it2;

  if (get_request_status() && !get_buffer_lock())
  {
    if (is_acquisition_done())
    {
      set_buffer_lock(true);
      set_request_as_done();
      uint16_t buffer_size = get_data_buffer_size();
      uint8_t *buffer_data_ptr = get_adc_data_orgin();
      calculate_and_add_crc(buffer_data_ptr);
//      start_timer();
//      it1 = get_timer();
      uart1_send(buffer_data_ptr, buffer_size);
//      it2 = get_timer() - it1;    // Derive the cycle-count difference
//      stop_timer();
    } else
    {
      acquisite_samples();
    }
  }

  hhrtim1.Instance->sMasterRegs.MICR |= HRTIM_MICR_MREP;
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
    set_buffer_lock(false);
  }
}

void USART1_IRQHandler(void)
{
  if (is_usart1_idle_interrupt())
  {
    execute_protocol();
    reset_usart1_dma_idle_interrupt();
  }
}
