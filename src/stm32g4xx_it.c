#include "stm32g4xx_it.h"

#include "../Driver_card/crc/crc.h"
#include "adc_interface.h"
#include "hrtim_interface.h"
#include "init_hrtim.h"
#include "main.h"
#include "protocol.h"
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
  uint32_t source_it = hdma_usart1_tx.Instance->CCR;
  uint32_t flag_it = hdma_usart1_tx.DmaBaseAddress->ISR;

  uint32_t tc_ie =
      (READ_BIT(source_it, DMA_CCR_TCIE) == DMA_CCR_TCIE) ? 1UL : 0UL;
  uint32_t dma_isr_tcie1 =
      (READ_BIT(flag_it, DMA_ISR_TCIF1) == (DMA_ISR_TCIF1)) ? 1UL : 0UL;

  if (tc_ie && dma_isr_tcie1)
  {
    hdma_usart1_tx.State = HAL_DMA_STATE_READY;
    huart1.gState = HAL_UART_STATE_READY;
    __HAL_UNLOCK(&hdma_usart1_tx);
    hdma_usart1_tx.DmaBaseAddress->IFCR = DMA_IFCR_CTCIF1;
  }
}

void DMA1_Channel2_IRQHandler(void)
{
  uint32_t source_it = hdma_usart1_rx.Instance->CCR;
  uint32_t flag_it = hdma_usart1_rx.DmaBaseAddress->ISR;

  uint32_t tc_ie =
      (READ_BIT(source_it, DMA_CCR_TCIE) == DMA_CCR_TCIE) ? 1UL : 0UL;
  uint32_t dma_isr_tcie2 =
      (READ_BIT(flag_it, DMA_ISR_TCIF2) == (DMA_ISR_TCIF2)) ? 1UL : 0UL;

  if (tc_ie && dma_isr_tcie2)
  {
    hdma_usart1_rx.DmaBaseAddress->IFCR = DMA_IFCR_CTCIF2;
  }
}

void DMA1_Channel3_IRQHandler(void)
{
  uint32_t flag_it = hdma_adc1.DmaBaseAddress->ISR;

  uint32_t dma_isr_tcie3 =
      (READ_BIT(flag_it, DMA_ISR_TCIF3) == (DMA_ISR_TCIF3)) ? 1UL : 0UL;

  if (dma_isr_tcie3)
  {
    uint32_t timeout = 1000;
    hdma_adc1.DmaBaseAddress->IFCR = DMA_IFCR_CTCIF3;
    hdma_adc1.State = HAL_DMA_STATE_READY;
    __HAL_UNLOCK(&hdma_adc1);

    do
    {
      LL_ADC_REG_StopConversion(AdcHandle.Instance);
      LL_ADC_Disable(AdcHandle.Instance);
      while(LL_ADC_IsDisableOngoing(AdcHandle.Instance))
      {
        if (timeout == 0)
        {
          break;
        }
        timeout--;
      }
    } while(LL_ADC_IsDisableOngoing(AdcHandle.Instance));

    set_buffer_lock(false);
  }
}

void USART1_IRQHandler(void)
{
  uint32_t isrflags = huart1.Instance->ISR;
  uint32_t cr1its = huart1.Instance->CR1;

  uint32_t idle_ie = (
      (READ_BIT(cr1its, USART_CR1_IDLEIE) == (USART_CR1_IDLEIE)) ? 1UL : 0UL);
  uint32_t usart_isr_idle = (
      (READ_BIT(isrflags, USART_ISR_IDLE) == (USART_ISR_IDLE)) ? 1UL : 0UL);

  /* Tx process is ended, restore huart->gState to Ready */
  huart1.gState = HAL_UART_STATE_READY;

  if (idle_ie && usart_isr_idle)
  {
    huart1.Instance->ICR = USART_ICR_IDLECF;

    if ((uint32_t)(
        ARRAY_LEN(usart_rx_dma_buffer)
            - (huart1.hdmarx->Instance->CNDTR & DMA_CNDTR_NDT)))
    {
      execute_protocol(usart_rx_dma_buffer);
    }

    huart1.hdmarx->Instance->CCR &= ~DMA_CCR_EN;
    huart1.hdmarx->Instance->CMAR = (uint32_t) usart_rx_dma_buffer;
    huart1.hdmarx->Instance->CNDTR = ARRAY_LEN(usart_rx_dma_buffer);
    huart1.hdmarx->Instance->CCR |= DMA_CCR_EN;
  }
}
