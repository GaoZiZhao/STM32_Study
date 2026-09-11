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
  * @brief   Provide the HAL APIs for LED and corresponding operations.
  * 
  * Processing flow:
  * 
  * call directly.
  * 
  * @version V1.0 2026-1-1
  * 
  * @note 1 tab == 4 spaces!
  * 
  *****************************************************************************/

#ifndef __BSP_LED_H__
#define __BSP_LED_H__

/*********************************** Includes**********************************/

#include <stdint.h> //the common library include part provided by the
#include <stdio.h>  //compiler

#include "cmsis_os.h"
#include "main.h" //core / os layer include part

#include "queue.h" //specific file for operation include part
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
	LED_OK = 0,               /* Operation completed successfully.   */
	LED_ERROR = 1,            /* Run-time error without case matched.*/
	LED_ERRORTIMEOUT = 2,     /* Operation timeout.                  */
	LED_ERRORRESOURCE = 3,    /* Resource not available.             */
	LED_ERRORPARAMETER = 4,   /* Parameter error.                    */
	LED_ERRORNOMEMORY = 5,    /* Out of memory.                      */
	LED_ERRORISR = 6,         /* Not allowed in ISR context.         */
	LED_RESERVED = 0X7FFFFFFF /* Reserved                            */
} led_status_t;

typedef enum
{
	LED_ON = 0,              /* Turn the LED on.                    */
	LED_OFF = 1,             /* Turn the LED off.                   */
	LED_TOGGLE = 2,          /* Toggle the LED state.               */
	LED_BLINK_3_TIMES = 3,   /* Blink the LED three times.          */
	LED_INITED_VALUE = 0XFF, /* Initial value, not a real command.  */
} led_operation_t;

/************************************ Defines *********************************/

/*********************************** Declaring ********************************/
/**
 * @brief  Execute the given LED operation.
 * 
 * @param  led_operation: LED_ON, LED_OFF, LED_TOGGLE or LED_BLINK_3_TIMES.
 * 
 * @retval LED_OK: the operation completed successfully.
 */
led_status_t led_on_off(led_operation_t led_operation);

/**
  * @brief  LED thread function.
  * @param  void *argument : Not used
  * @retval None
  */
void led_task_func(void *argument);

/*********************************** Declaring ********************************/

#endif /* End of __BSP_LED_H__ */
