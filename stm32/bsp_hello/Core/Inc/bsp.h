/**
  ******************************************************************************
  * @file    bsp.h
  * @author  Samuel Greene
  * @brief   This file contains a BSP for the STM32F401-Discovery Kit
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F411E_DISCOVERY_H
#define __STM32F411E_DISCOVERY_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "main.h"

//Define LEDs
typedef enum
{
  LED4 = 0,
  LED3 = 1,
  LED5 = 2,
  LED6 = 3
}Led_TypeDef;

#define LEDn                                    4

/* Audio status definition */
#define AUDIO_OK                              0
#define AUDIO_ERROR                           1
#define AUDIO_TIMEOUT                         2

#define AUDIODATA_SIZE      2
#define DMA_MAX_SZE                     0xFFFF
#define DMA_MAX(_X_)                (((_X_) <= DMA_MAX_SZE)? (_X_):DMA_MAX_SZE)

//LED control and init functions
void     BSP_LED_Init(Led_TypeDef Led);
void     BSP_LED_On(Led_TypeDef Led);
void     BSP_LED_Off(Led_TypeDef Led);
void     BSP_LED_Toggle(Led_TypeDef Led);
uint8_t BSP_AUDIO_OUT_Play(uint16_t* pBuffer, uint32_t Size);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F411E_DISCOVERY_H */
