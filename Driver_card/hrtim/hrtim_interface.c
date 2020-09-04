#include "hrtim_interface.h"
#include "buffers.h"
#include "defines_hrtim.h"
#include "stm32g4xx_hal_dma.h"
#include "stm32g4xx_hal_hrtim.h"

void change_frequency(uint32_t frequency)
{
  uint32_t temp = 0;
  temp = ((uint32_t)(((uint64_t) 170000000 * 16) / frequency));
  __HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_F,
      HRTIM_COMPAREUNIT_2, (25 * temp) / SCALER_COMPARE);
  __HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_F,
      HRTIM_COMPAREUNIT_3, (50 * temp) / SCALER_COMPARE);
  __HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_F,
      HRTIM_COMPAREUNIT_4, (75 * temp) / SCALER_COMPARE);

  __HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_MASTER, HRTIM_COMPAREUNIT_2,
      (25 * temp) / 100);
  __HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_MASTER, HRTIM_COMPAREUNIT_3,
      (75 * temp) / 100);

  //Set new period value for timers
  __HAL_HRTIM_SETPERIOD(&hhrtim1, HRTIM_TIMERINDEX_MASTER, temp);
  __HAL_HRTIM_SETPERIOD(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, temp);
  __HAL_HRTIM_SETPERIOD(&hhrtim1, HRTIM_TIMERINDEX_TIMER_F,
      temp / SCALER_PERIOD);
}

uint32_t get_frequency()
{
  const uint32_t clock_frequency = 170000000;
  const uint32_t multiplier = 16;
  uint32_t period = __HAL_HRTIM_GETPERIOD(&hhrtim1, HRTIM_TIMERINDEX_MASTER);
  uint64_t hrtim_clock = (clock_frequency * multiplier);
  uint32_t frequency = ((uint32_t)(hrtim_clock / period));
  return frequency;
}

void run_hrtim_output(void)
{
  HAL_HRTIM_WaveformOutputStart(&hhrtim1,
      HRTIM_OUTPUT_TA1 + HRTIM_OUTPUT_TA2 + HRTIM_OUTPUT_TD1
          + HRTIM_OUTPUT_TD2);
}
