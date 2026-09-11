/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_key_irq.h"
#include "bsp_led.h"
#include "queue.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */


/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t         defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
	.name = "defaultTask",
	.stack_size = 128 * 4,
	.priority = (osPriority_t)osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void)
{
	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of defaultTask */
	defaultTaskHandle = osThreadNew(   StartDefaultTask,
											   NULL,
							   &defaultTask_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	key_TaskHandle = osThreadNew(  key_task_func,
										NULL,
						   &key_Task_attributes);
	led_TaskHandle = osThreadNew(  led_task_func,
									    NULL,
						   &led_Task_attributes);
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */

/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
	/* USER CODE BEGIN StartDefaultTask */
	/******************* APP Task *************************/
	led_operation_t led_ops_event = LED_INITED_VALUE;
	key_press_status_t key_press_event = KEY_NOT_PRESSED;

	osDelay(500);
#if 0  //Test #1 error case fetch.
	key_press_event_t key_press_event = 
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

	if(pdTRUE == xQueueSend(inter_key_queue, &key_press_event, 0))
	{
		printf("inter_key_result send RISING event successfully "
		    								"at [%d] tick! \r\n",
		    									  HAL_GetTick());
	}
#endif //Test #1 error case fetch.

#if 0  //Test #2 filtering fetch and short press.
	key_press_event_t key_press_event1 = 
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

	if(pdTRUE == xQueueSend(inter_key_queue, &key_press_event1, 0))
	{
		printf("inter_key_result send FALLING event successfully "
		    								 "at [%d] tick! \r\n",
		    									   HAL_GetTick());
	}

	osDelay(5);

	key_press_event_t key_press_event2 = 
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

	if(pdTRUE == xQueueSend(inter_key_queue, &key_press_event2, 0))
	{
		printf("inter_key_result send RISING event successfully "
		    								"at [%d] tick! \r\n",
		    									  HAL_GetTick());
	}
#endif //Test #2 filtering fetch and short press.

#if 0  //Test #3 long press.
	key_press_event_t key_press_event1 = 
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

	if(pdTRUE == xQueueSend(inter_key_queue, &key_press_event1, 0))
	{
		printf("inter_key_result send FALLING event successfully "
		    								 "at [%d] tick! \r\n",
		    									   HAL_GetTick());
	}

	osDelay(600);

	key_press_event_t key_press_event2 =
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

	if(pdTRUE == xQueueSend(inter_key_queue, &key_press_event2, 0))
	{
		printf("inter_key_result send RISING event successfully "
		    								"at [%d] tick! \r\n",
		    									  HAL_GetTick());
	}
#endif //Test #3 long press.

	/* Infinite loop */
	for(;;)
	{
		printf("APP Task is living ...\r\n");
		if(pdTRUE == xQueueReceive(  key_queue,
						  &(key_press_event),
						  (TickType_t)0))
		{
			//1. if the key is pressed with short press.
			if(KEY_SHORT_PRESSED == key_press_event)
			{
				printf("Key Short Pressed! \r\n");
				//1.1 toggle the LED.
				led_ops_event = LED_TOGGLE;
				if(pdTRUE == xQueueSend(led_queue, &led_ops_event, 0))
				{
					printf("led_ops_event send toggle successfully "
					       "at [%d] tick! \r\n",
					       HAL_GetTick());
				}
				else
				{
					printf("led_ops_event send toggle failed "
					       "at [%d] tick! \r\n",
					       HAL_GetTick());
				}
			}
			//2. if the key is pressed with long press.
			else if(KEY_LONG_PRESSED == key_press_event)
			{
				printf("Key Long Pressed! \r\n");
				//2.1 LED blink for 3 times.
				led_ops_event = LED_BLINK_3_TIMES;
				if(pdTRUE == xQueueSend(led_queue, &led_ops_event, 0))
				{
					printf("led_ops_event send blink 3 times successfully "
					       "at [%d] tick! \r\n",
					       HAL_GetTick());
				}
				else
				{
					printf("led_ops_event send blink 3 times failed "
					       "at [%d] tick! \r\n",
					       HAL_GetTick());
				}

			}
			else
			{
				printf("Invalid key event fetched "
				       "at [%d] tick! \r\n",
				       HAL_GetTick());
			}
		}
		osDelay(100);
	}
	/* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
