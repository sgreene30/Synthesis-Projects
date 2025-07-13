/* Includes ------------------------------------------------------------------*/
#include "bsp.h"


/** @defgroup STM32F411E_discovery_LOW_LEVEL_Private_Variables STM32F411E discovery LOW LEVEL Private Variables
  * @{
  */
GPIO_TypeDef* GPIO_PORT[LEDn] = {LD4_GPIO_Port,
                                 LD3_GPIO_Port,
                                 LD5_GPIO_Port,
                                 LD6_GPIO_Port};

const uint16_t GPIO_PIN[LEDn] = {LD4_Pin,
                                 LD3_Pin,
                                 LD5_Pin,
                                 LD6_Pin};

I2S_HandleTypeDef                 hAudioOutI2s;

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on.
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED3
  *     @arg LED5
  *     @arg LED6
  */
void BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_SET);
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off.
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED3
  *     @arg LED5
  *     @arg LED6
  */
void BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled.
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED3
  *     @arg LED5
  *     @arg LED6
  */
void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(GPIO_PORT[Led], GPIO_PIN[Led]);
}

uint8_t BSP_AUDIO_OUT_Play(uint16_t* pBuffer, uint32_t Size)
{

    //HAL_I2S_Transmit_DMA(&hi2s3, pBuffer, DMA_MAX(Size/AUDIODATA_SIZE));
    return 0;

}
