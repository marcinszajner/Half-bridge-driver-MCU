#include "main.h"
#include "stm32g4xx_it.h"

#include "../Driver_card/crc/crc.h"
#include "init_hrtim.h"
#include "stdbool.h"

#include "protocol.h"
#include "adc_interface.h"
#include "usart_interface.h"
#include "hrtim_interface.h"
#include "stm32g4xx_ll_dma.h"

uint32_t frequency = 200000;
int32_t temp = 0;

bool adc_data_ready = false;
bool adc_sync = false;

uint32_t buffer_idx = 0;
bool buffer_lock = true;


void NMI_Handler(void)
{

}

void HardFault_Handler(void)
{
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  while (1)
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

uint32_t temp2 =0;
uint8_t aTxStartMessage[] = "przerwanie z timera\n";

uint16_t* sample_buffer_pointer;
uint32_t  buffer_size;
void SysTick_Handler(void)
{
  HAL_IncTick();

  if(get_request_status() && buffer_lock)
  {
    if(is_acquisition_done())
    {
      buffer_lock = true;
      set_request_as_done();
      uint16_t buffer_size = get_data_buffer_size();
      uint8_t* buffer_data_ptr = get_adc_data_orgin();
      calculate_and_add_crc(buffer_data_ptr);
      if(HAL_UART_Transmit_DMA(&huart2, buffer_data_ptr, buffer_size) != HAL_OK)
      {
        /* Transfer error in transmission process */
        Error_Handler();
      }

    }
    else
    {
      uint32_t frequency = get_actual_frequency();
      int32_t frequency_step =  get_frequency_step();
      if(frequency_step != 0)
      {
        uint32_t new_frequency = frequency + frequency_step;
        change_frequency(new_frequency);
        set_actual_frequency(new_frequency);
      }
      sample_buffer_pointer = (uint16_t*)get_next_data_buffer();
      buffer_size = get_packet_size();
      increment_actual_packet();
      buffer_lock = false;
    }
  }

  hhrtim1.Instance->sMasterRegs.MICR |= HRTIM_MICR_MREP;
}

void EXTI15_10_IRQHandler(void)
{
}

void DMA1_Channel1_IRQHandler(void)
{
  uint32_t source_it = hdma_usart2_tx.Instance->CCR;
  uint32_t flag_it = hdma_usart2_tx.DmaBaseAddress->ISR;

  uint32_t tc_ie = (READ_BIT(source_it, DMA_CCR_TCIE) == DMA_CCR_TCIE) ? 1UL : 0UL;
  uint32_t dma_isr_tcie1 = (READ_BIT(flag_it, DMA_ISR_TCIF1) == (DMA_ISR_TCIF1)) ? 1UL : 0UL;

  if(tc_ie && dma_isr_tcie1)
  {
	hdma_usart2_tx.State = HAL_DMA_STATE_READY;
	huart2.gState = HAL_UART_STATE_READY;
	__HAL_UNLOCK(&hdma_usart2_tx);
	hdma_usart2_tx.DmaBaseAddress->IFCR = DMA_IFCR_CTCIF1;
  }
}


void DMA1_Channel2_IRQHandler(void)
{

  uint32_t source_it = hdma_usart2_rx.Instance->CCR;
  uint32_t flag_it = hdma_usart2_rx.DmaBaseAddress->ISR;

  uint32_t tc_ie = (READ_BIT(source_it, DMA_CCR_TCIE) == DMA_CCR_TCIE) ? 1UL : 0UL;
  uint32_t dma_isr_tcie2 = (READ_BIT(flag_it, DMA_ISR_TCIF2) == (DMA_ISR_TCIF2)) ? 1UL : 0UL;

  if(tc_ie && dma_isr_tcie2)
  {
	hdma_usart2_rx.DmaBaseAddress->IFCR = DMA_IFCR_CTCIF2;
  }
}

void USART2_IRQHandler(void)
{
  uint32_t isrflags   = huart2.Instance->ISR;
  uint32_t cr1its     = huart2.Instance->CR1;

  uint32_t idle_ie = ((READ_BIT(cr1its, USART_CR1_IDLEIE) == (USART_CR1_IDLEIE)) ? 1UL : 0UL);
  uint32_t usart_isr_idle = ((READ_BIT(isrflags, USART_ISR_IDLE) == (USART_ISR_IDLE)) ? 1UL : 0UL);

  /* Tx process is ended, restore huart->gState to Ready */
  huart2.gState = HAL_UART_STATE_READY;


  if(idle_ie && usart_isr_idle)
  {
	huart2.Instance->ICR = USART_ICR_IDLECF;

    if((uint32_t)(ARRAY_LEN(usart_rx_dma_buffer) - (huart2.hdmarx->Instance->CNDTR & DMA_CNDTR_NDT)))
    {
        execute_protocol(usart_rx_dma_buffer);
    }

    huart2.hdmarx->Instance->CCR &= ~DMA_CCR_EN;
	huart2.hdmarx->Instance->CMAR = (uint32_t)usart_rx_dma_buffer;
	huart2.hdmarx->Instance->CNDTR = ARRAY_LEN(usart_rx_dma_buffer);
	huart2.hdmarx->Instance->CCR |= DMA_CCR_EN;
  }
}

#define start_timer()    *((volatile uint32_t*)0xE0001000) = 0x40000001  // Enable CYCCNT register
#define stop_timer()   *((volatile uint32_t*)0xE0001000) = 0x40000000  // Disable CYCCNT register
#define get_timer()   *((volatile uint32_t*)0xE0001004)               // Get value from CYCCNT register

void ADC1_2_IRQHandler(void)
{
    //proffiling
    //volatile uint32_t it1, it2;
    //start_timer();
    //it1 = get_timer();
    __HAL_ADC_CLEAR_FLAG(&AdcHandle, ADC_FLAG_JEOS);
    volatile uint32_t value = AdcHandle.Instance->JDR1;

    if(!adc_sync && !buffer_lock)
    {
        if(hhrtim1.Instance->sMasterRegs.MISR & HRTIM_MISR_MREP)
        {
            hhrtim1.Instance->sMasterRegs.MICR |= HRTIM_MICR_MREP;
            adc_sync = true;
        }
    }
    else
    {
        hhrtim1.Instance->sMasterRegs.MICR |= HRTIM_MICR_MREP;
    }

    if(!buffer_lock && adc_sync)
    {
        if(buffer_idx <= buffer_size)
        {
            sample_buffer_pointer[buffer_idx] = (uint16_t)value;
            buffer_idx++;
        }
        else
        {
            buffer_idx = 0;
            buffer_lock = true;
            adc_data_ready = true;
            adc_sync = false;
        }
    }
    //it2 = get_timer() - it1;    // Derive the cycle-count difference
    //stop_timer();
}
