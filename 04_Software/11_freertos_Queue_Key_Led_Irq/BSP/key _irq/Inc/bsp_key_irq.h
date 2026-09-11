/******************************************************************************
  * Copyright (C) 2026 SWPE, Inc.(Gmbh) or its affiliates.
  * 
  * ALL Rights Reserved.
  * 
  * @file bsp_key_irq.h
  * 
  * @par dependencies
  * - stdint.h
  * - stdio.h
  * - main.h
  * - cmsis_os.h
  * - queue.h
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
  * @version V2.0 2026-6-13
  * 
  * @note 1 tab == 4 spaces!
  * 
  *****************************************************************************/

#ifndef __BSP_KEY_IRQ_H__
#define __BSP_KEY_IRQ_H__

/*********************************** Includes**********************************/

#include "cmsis_os.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>

#include "queue.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

/************************************ Includes ********************************/

/************************************ Defines**********************************/

#define SHORT_PRESS_TIME (500) //Specify the short press time.
#define KEY_CALLBACK_IRQ void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)

//******************Thread_Func ***************************//
extern osThreadId_t         key_TaskHandle;
extern const osThreadAttr_t key_Task_attributes;
//******************Thread_Func ***************************//

//******************Queue_Handle ***************************//
extern QueueHandle_t key_queue;
extern QueueHandle_t inter_key_queue;
//******************Queue_Handle ***************************//

/** function return status                      */
typedef enum
{
	KEY_OK = 0,               /* Operation completed successfully.   */
	KEY_ERROR = 1,            /* Run-time error without case matched.*/
	KEY_ERRORTIMEOUT = 2,     /* Operation timeout.                  */
	KEY_ERRORRESOURCE = 3,    /* Resource not available.             */
	KEY_ERRORPARAMETER = 4,   /* Parameter error.                    */
	KEY_ERRORNOMEMORY = 5,    /* Out of memory.                      */
	KEY_ERRORISR = 6,         /* Not allowed in ISR context.         */
	KEY_RESERVED = 0X7FFFFFFF /* Reserved                            */
} key_status_t;

typedef enum
{
	KEY_PRESSED = 0,       /* Key is currently pressed.           */
	KEY_NOT_PRESSED = 1,   /* Key is currently released.          */
	KEY_SHORT_PRESSED = 2, /* Key was pressed for a short time.   */
	KEY_LONG_PRESSED = 3,  /* Key was pressed for a long time.    */
} key_press_status_t;

typedef enum
{
	KEY_EDGE_FALLING = 0, /* FALLING edge trigger. */
	KEY_EDGE_RISING  = 1, /* RISING edge trigger.  */
} key_trigger_edge_t;

typedef struct
{
	key_trigger_edge_t edge_type;    /* Trigger edge type of the event.     */
	uint32_t           trigger_tick; /* Tick value when the event occurred. */
} key_press_event_t;

/************************************ Defines *********************************/

/*********************************** Declaring ********************************/

/**
 * @brief  Scan the key state and store it to the given location.
 * 
 * @param  key_value: pointer to the key press status to be updated.
 * 
 * @retval KEY_OK          : the key is currently pressed.
 *         KEY_ERRORTIMEOUT: the key is currently released.
 */
key_status_t key_scan(key_press_status_t *key_value);

/**
 * @brief  Check whether the key was pressed for a short or a long time.
 * 
 * @param  key_value        : pointer to the key press status to be updated.
 * @param  short_press_time : threshold in ticks between a short and a long press.
 * 
 * @retval KEY_OK: the key press was identified (short or long).
 *         Other key_status_t values: the function failed or timed out.
 */
key_status_t key_scan_short_long_press(key_press_status_t *key_value, uint32_t short_press_time);

/**
  * @brief  key thread function.
  * @param  void *argument : Not used
  * @retval None
  */
void key_task_func(void *argument);

/********************************* Declaring **********************************/

#endif /* End of __BSP_KEY_IRQ_H__ */
