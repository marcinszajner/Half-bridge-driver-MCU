#include "main.h"
#include "init_periph.h"
#include "hrtim_interface.h"

int main(void)
{
  init_periph();
  run_hrtim_output();

  while(1)
  {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    /* Insert delay 100 ms */
    HAL_Delay(100);
  }
}

void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
}
