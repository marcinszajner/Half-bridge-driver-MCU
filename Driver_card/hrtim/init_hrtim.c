#include "init_hrtim.h"
#include "buffers.h"
#include "defines_hrtim.h"

#define FREQUENCY 50000
#define TIMA_PERIOD ((uint16_t)(((uint64_t)170000000 * 16) / FREQUENCY))

static void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
}

void DC_HRTIM1_Init(void)
{
  HRTIM_TimeBaseCfgTypeDef pTimeBaseCfg = {0};
  HRTIM_TimerCfgTypeDef pTimerCfg = {0};
  HRTIM_OutputCfgTypeDef pOutputCfg = {0};
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_HRTIM1_CLK_ENABLE();

  /* USER CODE END HRTIM1_Init 1 */
  hhrtim1.Instance = HRTIM1;
  hhrtim1.Init.HRTIMInterruptResquests = HRTIM_IT_NONE;
  hhrtim1.Init.SyncOptions = HRTIM_SYNCOPTION_NONE;
  if (HAL_HRTIM_Init(&hhrtim1) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_HRTIM_DLLCalibrationStart(&hhrtim1, HRTIM_CALIBRATIONRATE_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_HRTIM_PollForDLLCalibration(&hhrtim1, 10) != HAL_OK)
  {
    Error_Handler();
  }
  pTimeBaseCfg.Period = TIMA_PERIOD;
  pTimeBaseCfg.RepetitionCounter = 0x00;
  pTimeBaseCfg.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL16;
  pTimeBaseCfg.Mode = HRTIM_MODE_CONTINUOUS;

  if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_MASTER, &pTimeBaseCfg) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, &pTimeBaseCfg) != HAL_OK)
  {
    Error_Handler();
  }

  // timer as adc trigger with 2x frequency
  pTimeBaseCfg.Period = TIMA_PERIOD/SCALER_PERIOD;
  pTimeBaseCfg.Mode = HRTIM_MODE_SINGLESHOT_RETRIGGERABLE;
  if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_F, &pTimeBaseCfg) != HAL_OK)
  {
    Error_Handler();
  }

  pTimerCfg.InterruptRequests = HRTIM_MASTER_IT_NONE;
  pTimerCfg.DMARequests = HRTIM_TIM_DMA_NONE;
  pTimerCfg.DMASrcAddress = 0x0000;
  pTimerCfg.DMADstAddress = 0x0000;
  pTimerCfg.DMASize = 0x0;
  pTimerCfg.HalfModeEnable = HRTIM_HALFMODE_ENABLED;
  pTimerCfg.InterleavedMode = HRTIM_INTERLEAVED_MODE_DISABLED;
  pTimerCfg.StartOnSync = HRTIM_SYNCSTART_DISABLED;
  pTimerCfg.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
  pTimerCfg.DACSynchro = HRTIM_DACSYNC_NONE;
  pTimerCfg.PreloadEnable = HRTIM_PRELOAD_ENABLED;
  pTimerCfg.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
  pTimerCfg.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
  pTimerCfg.RepetitionUpdate = HRTIM_UPDATEONREPETITION_ENABLED;
  pTimerCfg.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
  pTimerCfg.FaultEnable = HRTIM_TIMFAULTENABLE_NONE;
  pTimerCfg.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
  pTimerCfg.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
  pTimerCfg.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
  pTimerCfg.UpdateTrigger = HRTIM_TIMUPDATETRIGGER_NONE;
  pTimerCfg.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;
  pTimerCfg.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
  pTimerCfg.ReSyncUpdate = HRTIM_TIMERESYNC_UPDATE_UNCONDITIONAL;
  if (HAL_HRTIM_WaveformTimerConfig(&hhrtim1, HRTIM_TIMERINDEX_MASTER, &pTimerCfg) != HAL_OK)
  {
    Error_Handler();
  }

// For 16 sample per cycle
//  compare_config.CompareValue = (25 * FREQUENCY)/100;
//  HAL_HRTIM_WaveformCompareConfig(&hhrtim1,
//                                  HRTIM_TIMERINDEX_MASTER,
//                                  HRTIM_COMPAREUNIT_2,
//                                  &compare_config);
//
//  /* Compare 3 is used for SR2 turn-on */
//  compare_config.CompareValue = (75 * FREQUENCY)/100;
//  HAL_HRTIM_WaveformCompareConfig(&hhrtim1,
//                                  HRTIM_TIMERINDEX_MASTER,
//                                  HRTIM_COMPAREUNIT_3,
//                                  &compare_config);
  //////////////////////////////


  pTimerCfg.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
  pTimerCfg.StartOnSync = HRTIM_SYNCSTART_DISABLED;
  pTimerCfg.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
  pTimerCfg.DACSynchro = HRTIM_DACSYNC_NONE;
  pTimerCfg.PreloadEnable = HRTIM_PRELOAD_ENABLED;
  pTimerCfg.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
  pTimerCfg.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
  pTimerCfg.RepetitionUpdate = HRTIM_UPDATEONREPETITION_DISABLED;
  pTimerCfg.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
  pTimerCfg.InterruptRequests = HRTIM_TIM_IT_NONE;
  pTimerCfg.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
  pTimerCfg.FaultEnable = HRTIM_TIMFAULTENABLE_NONE;
  pTimerCfg.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
  pTimerCfg.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
  pTimerCfg.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
  pTimerCfg.UpdateTrigger= HRTIM_TIMUPDATETRIGGER_MASTER;
  pTimerCfg.ResetTrigger = HRTIM_TIMRESETTRIGGER_MASTER_PER + HRTIM_TIMRESETTRIGGER_MASTER_CMP1;
  HAL_HRTIM_WaveformTimerConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D,&pTimerCfg);

  pTimerCfg.ResetTrigger = HRTIM_TIMRESETTRIGGER_MASTER_PER + HRTIM_TIMRESETTRIGGER_MASTER_CMP1;// for 16 sample per cycle + HRTIM_TIMRESETTRIGGER_MASTER_CMP2 + HRTIM_TIMRESETTRIGGER_MASTER_CMP3;
  HAL_HRTIM_WaveformTimerConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_F,&pTimerCfg);

  pOutputCfg.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  pOutputCfg.SetSource = HRTIM_OUTPUTSET_MASTERCMP1;
  pOutputCfg.ResetSource  = HRTIM_OUTPUTSET_MASTERPER;
  pOutputCfg.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  pOutputCfg.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  pOutputCfg.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_INACTIVE;
  pOutputCfg.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  pOutputCfg.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim1,
                                 HRTIM_TIMERINDEX_TIMER_D,
                                 HRTIM_OUTPUT_TD1, //HRTIM1_CHD1 PB14
                                 &pOutputCfg);

  // ADC trigger

  /* Set compare registers for turn-off time on TB1 */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = (25 * TIMA_PERIOD)/SCALER_COMPARE; //Set 1st adc triger to 12.5% and 62.5% of period (45 and 225 degrees)
  HAL_HRTIM_WaveformCompareConfig(&hhrtim1,
                                  HRTIM_TIMERINDEX_TIMER_F,
                                  HRTIM_COMPAREUNIT_2,
                                  &compare_config);

  /* Set compare 3 for sampling before turn-on on SR1 */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = (50 * TIMA_PERIOD)/SCALER_COMPARE; //Set 2nd adc triger to 25% and 75% of period (90 and 270 degrees)
  HAL_HRTIM_WaveformCompareConfig(&hhrtim1,
                                  HRTIM_TIMERINDEX_TIMER_F,
                                  HRTIM_COMPAREUNIT_3,
                                  &compare_config);

  /* Set compare 4 for sampling before turn-off on SR1 */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = (75 * TIMA_PERIOD)/SCALER_COMPARE; //Set 3th adc triger to 37.5% and 87.5% of period (135 and 315 degrees)
  HAL_HRTIM_WaveformCompareConfig(&hhrtim1,
                                  HRTIM_TIMERINDEX_TIMER_F,
                                  HRTIM_COMPAREUNIT_4,
                                  &compare_config);

  adc_trigger_config.Trigger = HRTIM_ADCTRIGGEREVENT13_TIMERF_CMP2
                             + HRTIM_ADCTRIGGEREVENT13_TIMERF_CMP3
                             + HRTIM_ADCTRIGGEREVENT13_TIMERF_CMP4
                             + HRTIM_ADCTRIGGEREVENT13_TIMERF_PERIOD; //Set period event as adc triger to 50% and 100% of period (180 and 360 degrees)
  adc_trigger_config.UpdateSource = HRTIM_ADCTRIGGERUPDATE_TIMER_F;

  HAL_HRTIM_ADCTriggerConfig(&hhrtim1, HRTIM_ADCTRIGGER_1, &adc_trigger_config);

  HAL_HRTIM_SoftwareUpdate(&hhrtim1,
                        HRTIM_TIMERUPDATE_MASTER
                      + HRTIM_TIMERUPDATE_D
                      + HRTIM_TIMERUPDATE_F);

  if(hhrtim1.Instance==HRTIM1)
  {
	__HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF13_HRTIM1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }

  hhrtim1.Instance->sMasterRegs.MCNTR = 0x0000;
  hhrtim1.Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_F].CNTxR = 0x0000;
  hhrtim1.Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CNTxR = 0x0000;

  /* Start HRTIM's Master timer, TIMER A and B */
  hhrtim1.Instance->sCommonRegs.CR2 = (HRTIM_TIMERRESET_MASTER | HRTIM_TIMERRESET_TIMER_F |
                                       HRTIM_TIMERRESET_TIMER_D | HRTIM_TIMERUPDATE_MASTER |
                                       HRTIM_TIMERUPDATE_F | HRTIM_TIMERUPDATE_D);
  hhrtim1.Instance->sMasterRegs.MCR |= (HRTIM_TIMERID_MASTER | HRTIM_TIMERID_TIMER_F | HRTIM_TIMERID_TIMER_D);

  perypherial_state.hrtim_state.hrtim_status = RUNNING;

}
