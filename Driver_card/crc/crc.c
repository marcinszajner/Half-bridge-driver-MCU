#include "crc.h"
#include "stm32g4xx_hal_crc.h"

CRC_HandleTypeDef hcrc;

static void crc_error_handler()
{
    while(1)
    {

    }
}

void crc_init(void)
{
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_DISABLE;
  hcrc.Init.GeneratingPolynomial = 4129;
  hcrc.Init.CRCLength = CRC_POLYLENGTH_16B;
  hcrc.Init.InitValue = 0x5ABE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    crc_error_handler();
  }
}

uint32_t crc_calculate(uint8_t* buffer_ptr, uint32_t buffer_size)
{
   uint32_t crc_result = HAL_CRC_Calculate(&hcrc, (uint32_t *)buffer_ptr, buffer_size);

   return crc_result;
}
