/******************************************************************************
  * Copyright (C) 2026 SWPE, Inc.(Gmbh) or its affiliates.
  * 
  * ALL Rights Reserved.
  * 
  * @file key.h
  * 
  * @par dependencies
  * - stdint.h
  * - stdio.h
  * - main.h
	* - stm32f4xx_hal.h
	* - stm32f4xx_hal_gpio.h
  * 
  * @author  GaoZizhao | SWPE. | Chengdu
  * 
  * @brief   Provide the HAL APIs for key and corresponding operations.
  * 
  * Processing flow:
  * 
  * call directly.
  * 
  * @version V1.0 2026-1-1
  * 
  * @note 1 tab == 4 spaces!
  * 
  ******************************************************************************/

#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

/*********************************** Includes**********************************/

#include <stdint.h>            //编译器提供的通用库包含部分
#include <stdio.h>
#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

/************************************ Includes *********************************/

/************************************ Defines***********************************/

/** 函数返回状态枚举                 */
typedef enum
{
    KEY_OK                = 0,            /* Operation completed successfully.   */
    KEY_ERROR             = 1,            /* Run-time error without case matched.*/
    KEY_ERRORTIMEOUT      = 2,            /* Operation timeout.                  */
    KEY_ERRORRESOURCE     = 3,            /* Resource not available.             */
    KEY_ERRORPARAMETER    = 4,            /* Parameter error.                    */
    KEY_ERRORNOMEMORY     = 5,            /* Out of memory.                      */
    KEY_ERRORISR          = 6,            /* Not allowed in ISR context.         */
    KEY_RESERVED          = 0X7FFFFFFF    /* Reserved                            */
}key_status_t;

typedef enum
{
    KEY_PRESSED           = 0,            /* Operation completed successfully.   */
    KEY_NOT_PRESSED       = 1,            /* Run-time error without case matched.*/
}key_press_status_t;

/************************************ Defines **********************************/

key_status_t key_scan(key_press_status_t *key_value);

/*********************************** Declaring ********************************/

#endif /* End of __BSP_KEY_H__ */
