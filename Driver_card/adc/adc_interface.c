#include "adc_interface.h"
#include "adc_structs.h"
#include "hrtim_interface.h"
#include "msg_interface.h"
#include "adc_error_handle.h"

typedef struct data_acq_con_ar_t
{
    bool request_active;
    bool data_ready_send;
    uint32_t packet_size;
    uint32_t n_packet;
    uint32_t actual_packet;
    uint32_t actual_frequency;
    int32_t  fequency_step;
} data_acq_con_ar_t;

data_acq_con_ar_t data_acq_con_ar;

void init_data_acq_struct(cmd_data_req *req)
{
  data_acq_con_ar.fequency_step = req->fequency_step;
  data_acq_con_ar.n_packet = req->n_packet;
  data_acq_con_ar.packet_size = req->packet_size;
  data_acq_con_ar.actual_frequency = req->actual_frequency;
  data_acq_con_ar.actual_packet = 0;
  data_acq_con_ar.data_ready_send = false;
  data_acq_con_ar.request_active = true;

  init_data_req_msg(data_acq_con_ar.packet_size,
                    data_acq_con_ar.n_packet,
                    data_acq_con_ar.fequency_step,
                    data_acq_con_ar.actual_frequency);
  acquisite_samples();
}

bool get_data_request_status()
{
  return data_acq_con_ar.request_active;
}

bool is_acquisition_done()
{
  return (data_acq_con_ar.actual_packet == data_acq_con_ar.n_packet);
}

void increment_actual_packet()
{
  data_acq_con_ar.actual_packet++;
}

void set_request_as_done()
{
  data_acq_con_ar.request_active = false;
}

uint32_t get_packet_size()
{
  return data_acq_con_ar.packet_size;
}

int32_t get_frequency_step()
{
  return data_acq_con_ar.fequency_step;
}

uint32_t get_actual_frequency()
{
  return data_acq_con_ar.actual_frequency;
}

void set_actual_frequency(uint32_t actual_frequency)
{
  data_acq_con_ar.actual_frequency = actual_frequency;
}

HAL_StatusTypeDef prepare_acquisite_n_samples_with_dma(ADC_HandleTypeDef *hadc,
    uint32_t *pData, uint32_t Length)
{
  HAL_StatusTypeDef tmp_hal_status;

  if (LL_ADC_REG_IsConversionOngoing(hadc->Instance) == 0UL)
  {
    tmp_hal_status = ADC_Enable(hadc);

    /* Manage ADC and DMA start: ADC overrun interruption, DMA start,     */
    /* ADC start (in case of SW start):                                   */

    /* Clear regular group conversion flag and overrun flag               */
    /* (To ensure of no unknown state from potential previous ADC         */
    /* operations)                                                        */
    __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR));

    /* With DMA, overrun event is always considered as an error even if
     hadc->Init.Overrun is set to ADC_OVR_DATA_OVERWRITTEN. Therefore,
     ADC_IT_OVR is enabled. */
    __HAL_ADC_ENABLE_IT(hadc, ADC_IT_OVR);

    /* Enable ADC DMA mode */
    SET_BIT(hadc->Instance->CFGR, ADC_CFGR_DMAEN);

    /* Start the DMA channel */
    tmp_hal_status = HAL_DMA_Start_IT(hadc->DMA_Handle,
        (uint32_t) & hadc->Instance->DR, (uint32_t) pData, Length);

  } else
  {
    tmp_hal_status = HAL_BUSY;
  }
  return tmp_hal_status;
}

uint8_t* get_actual_position()
{
  return &get_adc_data_acq_resp_buf()[sizeof(cmd_data_resp) + data_acq_con_ar.packet_size * SAMPLE_SIZE
                                     * data_acq_con_ar.actual_packet];
}

void acquisite_samples()
{
  uint32_t frequency = get_actual_frequency();
  int32_t frequency_step = get_frequency_step();
  if (frequency_step != 0)
  {
    uint32_t new_frequency = frequency + frequency_step;
    change_frequency(new_frequency);
    set_actual_frequency(new_frequency);
  }

  uint8_t *sample_buffer_pointer;
  uint32_t buffer_size;

  sample_buffer_pointer = get_actual_position();
  buffer_size = get_packet_size();
  increment_actual_packet();

  if (prepare_acquisite_n_samples_with_dma(&AdcHandle,
      (uint32_t*) sample_buffer_pointer, buffer_size) != HAL_OK)
  {
    adc_error_handler();
  }

  /* Enter critical section: Disable interrupts to avoid any interruption during the loop */
  uint32_t primask_bit;
  primask_bit = __get_PRIMASK();
  __disable_irq();

  hhrtim1.Instance->sMasterRegs.MICR |= HRTIM_MICR_MREP;
  while(!(hhrtim1.Instance->sMasterRegs.MISR & HRTIM_MISR_MREP))
  {
  }
  LL_ADC_REG_StartConversion(AdcHandle.Instance);

  /* Exit critical section: restore previous priority mask */
  __set_PRIMASK(primask_bit);
}

uint32_t is_adc1_dma_tci()
{
  return (READ_BIT(hdma_adc1.DmaBaseAddress->ISR, DMA_ISR_TCIF3) == (DMA_ISR_TCIF3)) ? 1UL : 0UL;
}

void reset_adc_dma_tci()
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

}

void packet_acq_complete_handler()
{
  if (get_data_request_status())
  {
    if (is_acquisition_done())
    {
      send_data_acq_resp();
      set_request_as_done();
    }
    else
    {
      acquisite_samples();
    }
  }

  hhrtim1.Instance->sMasterRegs.MICR |= HRTIM_MICR_MREP;
}
