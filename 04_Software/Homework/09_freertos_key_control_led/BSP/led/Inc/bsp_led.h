/******************************************************************************
  * Copyright (C) 2026 SWPE, Inc.(Gmbh) or its affiliates.
  * 
  * ALL Rights Reserved.
  * 
  * @file bsp_led.h
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

#ifndef __BSP_LED_H__
#define __BSP_LED_H__

/*********************************** Includes**********************************/

#include <stdint.h>           //the common library include part provided by the compiler
#include <stdio.h>

#include "main.h"             //core / os layer include part
#include "cmsis_os.h"

#include "queue.h"            //specific file for operation include part
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

/************************************ Includes *********************************/

/************************************ Defines***********************************/
//******************Thread_Func ***************************//
extern osThreadId_t led_TaskHandle;

extern const osThreadAttr_t led_Task_attributes;
//******************Thread_Func ***************************//

//******************Queue_Handle ***************************//
extern QueueHandle_t led_queue;
//******************Queue_Handle ***************************//

/** function return status                      */
typedef enum
{
    LED_OK                = 0,            /* Operation completed successfully.   */
    LED_ERROR             = 1,            /* Run-time error without case matched.*/
    LED_ERRORTIMEOUT      = 2,            /* Operation timeout.                  */
    LED_ERRORRESOURCE     = 3,            /* Resource not available.             */
    LED_ERRORPARAMETER    = 4,            /* Parameter error.                    */
    LED_ERRORNOMEMORY     = 5,            /* Out of memory.                      */
    LED_ERRORISR          = 6,            /* Not allowed in ISR context.         */
    LED_RESERVED          = 0X7FFFFFFF    /* Reserved                            */
}led_status_t;

typedef enum
{
    LED_ON                = 0,            /* Operation completed successfully.   */
    LED_OFF               = 1,            /* Operation completed successfully.   */
    LED_TOGGLE            = 2,            /* Operation completed successfully.   */
} led_operation_t;

/************************************ Defines **********************************/

/*********************************** Declaring ********************************/
/**
 * @brief  Instantiates the bsp_led_handle_t target.
 * 
 * Steps:
 * 1.doing the specific operations.
 * 
 * @param  led_operation: LED_ON or LED_OFF
 * 
 * @retval led_status_t: status of the function.
 */
led_status_t led_on_off(led_operation_t led_operation);

/**
  * @brief  key thread function.
  * @param  void *argument : Not used
  * @retval None
  */
void led_task_func(void *argument);

/*********************************** Declaring ********************************/

#endif /* End of __BSP_LED_H__ */
