#include "adc_interface.h"
#include "hrtim_interface.h"
#include "protocol.h"

uint8_t adc_data_buffer[sizeof(protocol_data_resp)+(MAX_ADC_PACKET_SIZE*SAMPLE_SIZE*MAX_NUMBER_OF_PACKET)+END_SIGNATURE_SIZE]; // header+payload(256 sample *2 byte * max of packet)+0x55555555

data_acq_con_ar_t data_acq_con_ar;

uint8_t* get_adc_data_orgin()
{
    return adc_data_buffer;
}

void init_data_acq_struct(init_data_acq_t* init_data_acq)
{
    data_acq_con_ar.fequency_step = init_data_acq->fequency_step;
    data_acq_con_ar.n_packet = init_data_acq->n_packet;
    data_acq_con_ar.packet_size = init_data_acq->packet_size;
    data_acq_con_ar.actual_frequency = init_data_acq->actual_frequency;
    data_acq_con_ar.actual_packet = 0;
    data_acq_con_ar.data_ready_send = false;
    data_acq_con_ar.request_active = true;
    data_acq_con_ar.buffer_lock = false;

    init_data_protocol_buffer(&data_acq_con_ar);
}

uint8_t* get_actual_data_buffer()
{
    return &adc_data_buffer[sizeof(protocol_data_resp)+data_acq_con_ar.packet_size*SAMPLE_SIZE*data_acq_con_ar.actual_packet];
}

bool get_request_status()
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

uint16_t get_data_buffer_size()
{
    head_t* head = (head_t*)adc_data_buffer;
    uint16_t size = head->size;
    return size;
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

void set_buffer_lock(bool status)
{
    data_acq_con_ar.buffer_lock = status;
}

bool get_buffer_lock()
{
   return data_acq_con_ar.buffer_lock;
}

HAL_StatusTypeDef prepare_acquisite_n_samples_with_dma(ADC_HandleTypeDef *hadc, uint32_t *pData, uint32_t Length)
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
	tmp_hal_status = HAL_DMA_Start_IT(hadc->DMA_Handle, (uint32_t)&hadc->Instance->DR, (uint32_t)pData, Length);

  }
  else
  {
	tmp_hal_status = HAL_BUSY;
  }
  return tmp_hal_status;
}

void acquisite_samples()
{
    uint32_t frequency = get_actual_frequency();
    int32_t frequency_step =  get_frequency_step();
    if(frequency_step != 0)
    {
      uint32_t new_frequency = frequency + frequency_step;
      change_frequency(new_frequency);
      set_actual_frequency(new_frequency);
    }

    uint16_t* sample_buffer_pointer;
    uint32_t  buffer_size;

    sample_buffer_pointer = (uint16_t*)get_actual_data_buffer();
    buffer_size = get_packet_size();
    increment_actual_packet();
    data_acq_con_ar.buffer_lock = true;

    if (prepare_acquisite_n_samples_with_dma(&AdcHandle,
                          	  	  	  	  	  (uint32_t*)sample_buffer_pointer,
											  buffer_size) != HAL_OK)
    {
  	  while(1){}
    }

    /* Enter critical section: Disable interrupts to avoid any interruption during the loop */
    uint32_t primask_bit;
    primask_bit = __get_PRIMASK();
    __disable_irq();

    hhrtim1.Instance->sMasterRegs.MICR |= HRTIM_MICR_MREP;
    while(!(hhrtim1.Instance->sMasterRegs.MISR & HRTIM_MISR_MREP)){}
    LL_ADC_REG_StartConversion(AdcHandle.Instance);

    /* Exit critical section: restore previous priority mask */
    __set_PRIMASK(primask_bit);
}
