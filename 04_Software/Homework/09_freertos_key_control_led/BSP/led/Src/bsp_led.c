/******************************************************************************
  * Copyright (C) 2026 SWPE, Inc.(Gmbh) or its affiliates.
  * 
  * ALL Rights Reserved.
  * 
  * @file bsp_led.c
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
  ******************************************************************************/

#include "bsp_led.h"

/************************************ Defines***********************************/

//******************Thread_Func ***************************//
osThreadId_t led_TaskHandle;

const osThreadAttr_t led_Task_attributes = {
  .name = "led_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
//******************Thread_Func ***************************//

//******************Queue_Handle ***************************//
QueueHandle_t led_queue;
//******************Queue_Handle ***************************//

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
led_status_t led_on_off(led_operation_t led_operation)
{   
	led_status_t ret = LED_OK;
    if(LED_ON == led_operation)
    {
        //1.Make the LED On.
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
    }

    if(LED_OFF == led_operation)
    {
        //2.Make the LED Off.
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
    }

    if(LED_TOGGLE == led_operation)
    {
        //3.Make the LED Toggle.
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    }
	return ret;
}

/**
  * @brief  key thread function.
  * @param  void *argument : Not used
  * @retval None
  */
void led_task_func(void *argument)
{
    led_status_t    led_ret    =          LED_OK;
    led_operation_t led_value  =          LED_ON;
    led_queue = xQueueCreate(10, sizeof(led_operation_t));

    if(NULL == led_queue)
    {
        printf("LED Queue Create Failed!\r\n");
    }
    else
    {
        printf("LED Queue Created Successfully!\r\n");
    }

    for(;;)
    {
        printf("LED Task Running...\r\n");
        //led_ret = led_on_off(&key_status);
    
        if (0 != led_queue)
        {
             //Receive a message on the created queue. block for 100 ticks if a message is not immediately available.
            if(xQueueReceive(led_queue, &led_value, (TickType_t)100) == pdTRUE)
            {
                printf("Received led_queue value: %d\r\n", led_value);
                led_ret = led_on_off(led_value);
            }
        }
		osDelay(100);
  }

}

