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

//Define LEDs
typedef enum
{
  LED4 = 0,
  LED3 = 1,
  LED5 = 2,
  LED6 = 3
}Led_TypeDef;

#define LEDn                                    6

#define LED4_PIN                                GPIO_PIN_12
#define LED4_GPIO_PORT                          GPIOD
#define LED4_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOD_CLK_DISABLE()


#define LED3_PIN                                GPIO_PIN_13
#define LED3_GPIO_PORT                          GPIOD
#define LED3_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOD_CLK_DISABLE()


#define LED5_PIN                                GPIO_PIN_14
#define LED5_GPIO_PORT                          GPIOD
#define LED5_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED5_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOD_CLK_DISABLE()


#define LED6_PIN                                GPIO_PIN_15
#define LED6_GPIO_PORT                          GPIOD
#define LED6_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED6_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOD_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__) do{if((__INDEX__) == 0) LED4_GPIO_CLK_ENABLE(); else \
                                           if((__INDEX__) == 1) LED3_GPIO_CLK_ENABLE(); else \
                                           if((__INDEX__) == 2) LED5_GPIO_CLK_ENABLE(); else \
                                           if((__INDEX__) == 3) LED6_GPIO_CLK_ENABLE(); \
                                           }while(0)

#define LEDx_GPIO_CLK_DISABLE(__INDEX__) do{if((__INDEX__) == 0) LED4_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 1) LED3_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 2) LED5_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 3) LED6_GPIO_CLK_DISABLE(); \
                                            }while(0)


//LED control and init functions
void     BSP_LED_Init(Led_TypeDef Led);
void     BSP_LED_On(Led_TypeDef Led);
void     BSP_LED_Off(Led_TypeDef Led);
void     BSP_LED_Toggle(Led_TypeDef Led);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F411E_DISCOVERY_H */
