/******************************************************************************
  * Copyright (C) 2026 SWPE, Inc.(Gmbh) or its affiliates.
  * 
  * ALL Rights Reserved.
  * 
  * @file bsp_key_irq.c
  * 
  * @par dependencies
  * - bsp_key_irq.h
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

#include "bsp_key_irq.h"

/************************************ Defines**********************************/

//******************Thread_Func ***************************//
osThreadId_t key_TaskHandle;

const osThreadAttr_t key_Task_attributes = {
	.name = "key_Task",
	.stack_size = 128 * 4,
	.priority = (osPriority_t)osPriorityNormal,
};
//******************Thread_Func ***************************//

//******************Queue_Handle ***************************//
QueueHandle_t key_queue = NULL;
QueueHandle_t inter_key_queue = NULL;
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
	key_status_t       key_status = KEY_OK;              /* return value      */
	key_press_status_t key_value_temp = KEY_NOT_PRESSED; /* key status value  */
	uint32_t           counter_tick = 0;                 /* counter           */
	/** Variables                                                             */

	//1. check if the key is pressed
	key_status = key_scan(&key_value_temp);

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
	/******************** Variables (in task stack) ***************************/
	uint32_t          event_index = 0;
	uint32_t          first_trigger_tick = 0;
	uint32_t          short_press_time = SHORT_PRESS_TIME;
	key_press_event_t key_press_event = 
	{
		.edge_type = KEY_EDGE_RISING,
		.trigger_tick = 0,
	};
	/******************** Variables (in task stack) ***************************/

	/********************* Variables (in  OS heap) ****************************/
	key_queue = xQueueCreate(10, sizeof(key_press_status_t));
	inter_key_queue = xQueueCreate(10, sizeof(key_press_event_t));
	/********************* Variables (in  OS heap) ****************************/

	//check if the queue has been created successfully.
	if(NULL == key_queue || NULL == inter_key_queue)
	{
		printf("Key Queue Create Failed!\r\n");
		return;
	}
	else
	{
		printf("Key Queue Created Successfully!\r\n");
	}
	for(;;)
	{
		printf("key_task_func is running "
		       "at [%d] tick\r\n",
		       HAL_GetTick());
		//1. check if there is new data about the key press in the queue.
		if(pdTRUE == xQueueReceive(inter_key_queue,
							  &(key_press_event),
							  (TickType_t)0))
		{
			printf("key_press_event.trigger_tick: [%d]\r\n",
							  key_press_event.trigger_tick);

			printf("inter_key_queue received key event "
			       "send at [%d] tick. \r\n",
			       HAL_GetTick());
			//1.1 if there is new data about the key,
			// then update it in the state machine.
			if(KEY_EDGE_RISING == key_press_event.edge_type && 0 == event_index)
			{
				printf("Key RISING fetched! Error! \r\n");
			}
			if(KEY_EDGE_FALLING == key_press_event.edge_type && 0 == event_index)
			{
				printf("Key first FALLING fetched! \r\n");
				//change the index for changing the state machine.
				event_index += 1;

				//Mark the first tick when event coming.
				first_trigger_tick = key_press_event.trigger_tick;
			}
			if(KEY_EDGE_RISING == key_press_event.edge_type && \
										   1 == event_index    )
			{
				printf("Key RISING after the FALLING! \r\n");

				//1.1.1 if the interval in new key event between two key data
				//is less than 10ms
				if(key_press_event.trigger_tick - first_trigger_tick < 10)
				{
					//1.1.1.1 the new key press event is not valid.
					printf("Invalid key event fetched "
					       "at [%d] tick! \r\n",
					       HAL_GetTick());
					continue;
				}

				//1.1.2 if the interval is more than 10ms
				//1.1.2.1 if the new key press event is valid.
				//1.1.2.1.1 if the interval is less than the short_press time
				// then it should be short press.
				if(key_press_event.trigger_tick - first_trigger_tick \
												  < short_press_time )
				{
					//1.1.2.1.1.1 send the short press message to the key_queue.
					key_press_status_t key_result = KEY_SHORT_PRESSED;
					if(pdTRUE == xQueueSend(key_queue, &key_result, 0))
					{
						printf("key_result send short press successfully "
						       "at [%d] tick! \r\n",
						       HAL_GetTick());
						event_index = 0; //reset the index for next key event.
					}
					else
					{
						printf("key_result send short press failed "
						       "at [%d] tick! \r\n",
						       HAL_GetTick());
					}
				}

				//1.1.2.1.2 if the interval is more than the short_press time
				// then it should be long press.
				if(key_press_event.trigger_tick - first_trigger_tick \
												  > short_press_time )
				{
					//1.1.2.1.2.1 send the long press message to the key_queue.
					key_press_status_t key_result = KEY_LONG_PRESSED;
					if(pdTRUE == xQueueSend(key_queue, &key_result, 0))
					{
						printf("key_result send long press successfully"
						       "at [%d] tick! \r\n",
						       HAL_GetTick());
						event_index = 0; //reset the index for next key event.
					}
					else
					{
						printf("key_result send long press failed"
						       "at [%d] tick! \r\n",
						       HAL_GetTick());
					}
				}
			}
		}
		osDelay(100);
	}
}

/**
  * @brief  key_interrupt callback function.
  *
  * Steps
  * 1. if trigger first time with falling type,
  * send the event to the inter_key_queue.
  * changing the interruption type to rising.
  * 2. if trigger second time with rising type,
  * send the event to the inter_key_queue.
  * changing the interruption type to falling.
  * 
  * @param  void *argument : Not used
  * @retval None
  */
KEY_CALLBACK_IRQ
{
	static key_trigger_edge_t irq_type = KEY_EDGE_FALLING;
	/* 1. if trigger first time with falling type,
	* send the event to the inter_key_queue.
	* changing the interruption type to raising.
	*/
	BaseType_t      xHigherPriorityTaskWoken;
	if(KEY_EDGE_FALLING == irq_type)
	{
		key_press_event_t key_press_event_1 = 
		{
			.edge_type = KEY_EDGE_FALLING,
			.trigger_tick = HAL_GetTick()
		};

		if(NULL == inter_key_queue)
		{
			printf("inter_key_queue not created"
			       " at [%d] tick.\r\n",
			       HAL_GetTick());
		}

		if(pdTRUE ==xQueueSendFromISR(			 inter_key_queue, 
											  &key_press_event_1,
									  &xHigherPriorityTaskWoken))
		{
			printf("inter_key_result send FALLING event successfully "
			       "at [%d] tick! \r\n",
			       HAL_GetTick());
		}
		irq_type = KEY_EDGE_RISING;
		
		/* 
		 * 1.1 changing the interruption type to rising.
		*/
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		GPIO_InitStruct.Pin = Key_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(Key_GPIO_Port, &GPIO_InitStruct);
	}

	/* 2. if trigger second time with rising type,
	* send the event to the inter_key_queue.
	* changing the interruption type to falling.
	*/
	else if(KEY_EDGE_RISING == irq_type)
	{
		key_press_event_t key_press_event_2 = 
		{
			.edge_type = KEY_EDGE_RISING,
			.trigger_tick = HAL_GetTick()
		};

		if(NULL == inter_key_queue)
		{
			printf("inter_key_queue not created"
			       " at [%d] tick.\r\n",
			       HAL_GetTick());
		}

		if(pdTRUE ==xQueueSendFromISR(		  	 inter_key_queue,
											  &key_press_event_2,
									  &xHigherPriorityTaskWoken))
		{
			printf("inter_key_result send RISING event successfully "
			       "at [%d] tick! \r\n",
			       HAL_GetTick());
		}
		irq_type = KEY_EDGE_FALLING;
		
		/* 
		 * 1.2 changing the interruption type to falling.
		*/
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		GPIO_InitStruct.Pin = Key_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(Key_GPIO_Port, &GPIO_InitStruct);
	}
}

/*********************************** Defines***********************************/
