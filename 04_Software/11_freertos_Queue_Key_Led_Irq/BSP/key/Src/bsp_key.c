/******************************************************************************
  * Copyright (C) 2026 SWPE, Inc.(Gmbh) or its affiliates.
  * 
  * ALL Rights Reserved.
  * 
  * @file bsp_key.c
  * 
  * @par dependencies
  * - bsp_key.h
  * - stdio.h
  * - stdint.h
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
  *****************************************************************************/

#include "bsp_key.h"

/************************************ Defines**********************************/
//******************Thread_Func ***************************//
osThreadId_t key_TaskHandle;

const osThreadAttr_t key_Task_attributes = {
	.name = "key_Task",
	.stack_size = 128 * 4,
	.priority = (osPriority_t)osPriorityHigh,
};
//******************Thread_Func ***************************//

//******************Queue_Handle ***************************//
QueueHandle_t key_queue;
//******************Queue_Handle ***************************//

/**
 * @brief  Scan the key state and store it to the given location.
 * 
 * @param  key_value: pointer to the key press status to be updated.
 * 
 * @retval KEY_OK          : the key is currently pressed.
 *         KEY_ERRORTIMEOUT: the key is currently released.
 */
key_status_t key_scan(key_press_status_t *key_value)
{
	key_press_status_t key_status_value = KEY_NOT_PRESSED;

	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key_GPIO_Port, Key_Pin))
	{
		key_status_value = KEY_PRESSED;
		*key_value = key_status_value;
		return KEY_OK;
	}

	*key_value = key_status_value;

	return KEY_ERRORTIMEOUT; //No key pressed, return timeout.
}

/**
 * @brief  Check whether the key was pressed for a short or a long time.
 * 
 * @param  key_value        : pointer to the key press status to be updated.
 * @param  short_press_time : threshold in ticks between a short and a long press.
 * 
 * @retval KEY_OK: the key press was identified (short or long).
 *         Other key_status_t values: the function failed or timed out.
 */
key_status_t key_scan_short_long_press(key_press_status_t *key_value, uint32_t short_press_time)
{
	/** Variables                                                             */
	key_status_t       key_status = KEY_OK;              /* return value     */
	key_press_status_t key_value_temp = KEY_NOT_PRESSED; /* key status value  */
	uint32_t           counter_tick = 0;                 /* counter           */
	/** Variables                                                             */

	//1. check if the key is pressed
	key_status = key_scan(&key_value_temp);
	//TODO: critical section

	//1.1 if the key is pressed, then check if it is short pressed
	if(KEY_OK == key_status)
	{
		if(KEY_PRESSED == key_value_temp)
		{
			//get the timestamp of the key press.
			counter_tick = HAL_GetTick();

			//1.1.1 check the short press time.
			while(HAL_GetTick() < counter_tick + short_press_time)
				;

			//1.1.2 check the key state again.
			key_status = key_scan(&key_value_temp);

			if(KEY_NOT_PRESSED == key_value_temp)
			{
				//1.1.3 the key is pressed with short press.
				*key_value = KEY_SHORT_PRESSED;
				return KEY_OK;
			}
			else
			{
				//1.2 the key is pressed with long press.
				*key_value = KEY_LONG_PRESSED;

				//1.2.1 keep focusing on the status of key to avoid the short
				//press
				while(KEY_OK == key_scan(&key_value_temp))
					;
				return KEY_OK;
			}
		}
	}
	return key_status;
}

/**
  * @brief  key thread function.
  * @param  void *argument : Not used
  * @retval None
  */
void key_task_func(void *argument)
{
	key_status_t       key_ret = KEY_OK;
	key_press_status_t key_status = KEY_NOT_PRESSED;
	key_queue = xQueueCreate(10, sizeof(key_press_status_t));
	uint32_t counter_tick = 0;
	if(NULL == key_queue)
	{
		printf("Key Queue Create Failed!\r\n");
	}
	else
	{
		printf("Key Queue Created Successfully!\r\n");
	}
	for(;;)
	{
		counter_tick++;
		printf("Key Task Running...\r\n");
		key_ret = key_scan(&key_status);

		if(KEY_OK == key_ret)
		{
			if(KEY_PRESSED == key_status)
			{
				printf("Key Pressed!\r\n");
				if(pdTRUE == xQueueSend(key_queue, &counter_tick, 0))
				{
					//Key press message sent successfully.
					printf("Key Press Message Sent!\r\n");
				}
				else
				{
					//Key press message send failed.
					printf("Key Press Message Send Failed!\r\n");
				}
			}
			else
			{
				printf("Key Not Pressed!\r\n");
			}
		}
		else if(KEY_ERRORTIMEOUT == key_ret)
		{
			printf("Key Scan Timeout!\r\n");
		}
		else
		{
			printf("Key Scan Error!\r\n");
		}
		osDelay(100);
	}
}

/*********************************** Defines***********************************/
