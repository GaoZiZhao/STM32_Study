/******************************************************************************
  * Copyright (C) 2026 SWPE, Inc.(Gmbh) or its affiliates.
  * 
  * ALL Rights Reserved.
  * 
  * @file bsp_key.h
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

#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "cmsis_os.h"

#include "queue.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

/************************************ Includes *********************************/

/************************************ Defines***********************************/
//******************Thread_Func ***************************//
extern osThreadId_t key_TaskHandle;
extern const osThreadAttr_t key_Task_attributes;
//******************Thread_Func ***************************//

//******************Queue_Handle ***************************//
extern QueueHandle_t key_queue;
//******************Queue_Handle ***************************//

/** function return status                      */
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

/************************************ Defines ************************************/

/*********************************** Declaring ***********************************/

/**
 * @brief  Instantiates the bsp_key_handle_t target.
 * 
 * Steps:
 * 1.doing the specific operations.
 * 
 * @param  key_value: pointer to the key press status.
 * 
 * @retval key_status_t: status of the function.
 */
key_status_t key_scan(key_press_status_t *key_value);

/**
  * @brief  key thread function.
  * @param  void *argument : Not used
  * @retval None
  */
void key_task_func(void *argument);

/*********************************** Declaring ***********************************/

#endif /* End of __BSP_KEY_H__ */
