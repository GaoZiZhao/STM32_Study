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
  ******************************************************************************/

#include "bsp_key.h"

/************************************ Defines***********************************/
//******************Thread_Func ***************************//
osThreadId_t key_TaskHandle;

const osThreadAttr_t key_Task_attributes = {
  .name = "key_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
//******************Thread_Func ***************************//

//******************Queue_Handle ***************************//
QueueHandle_t key_queue;
//******************Queue_Handle ***************************//

key_status_t key_scan(key_press_status_t *key_value)
{
    uint32_t counter = 0;
    key_press_status_t key_status_value = KEY_NOT_PRESSED;

    while(counter < 1000)
    {
        //1. 先读取按键（PA0）的GPIO的电平，如果是高电平，则说明按键没有被按下。
        //2. 如果按键（PA0）的电平为低电平，则说明按键被按下了。
        //2.1 如果按键被按下，则发送相应的消息队列。
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key_GPIO_Port, Key_Pin))
		{
            key_status_value = KEY_PRESSED; //按键状态为按下
            *key_value = key_status_value; //按键值为1，表示按键被按下
            return KEY_OK; //返回按键扫描成功
		}
        //2.2 如果按键没有被按下，则继续检测。
        counter++;
    }
    *key_value = key_status_value;

    return KEY_ERRORTIMEOUT; //始终没有按键被按下，返回按键扫描超时
}

/**
  * @brief  key thread function.
  * @param  void *argument : Not used
  * @retval None
  */
void key_task_func(void *argument)
{
    key_status_t       key_ret    =          KEY_OK;
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
		counter_tick ++;
        printf("Key Task Running...\r\n");
        key_ret = key_scan(&key_status);
    
        if(KEY_OK == key_ret)
        {
            if(KEY_PRESSED == key_status)
            {
                printf("Key Pressed!\r\n");
                if(pdTRUE == xQueueSend(key_queue, &counter_tick, 0))
                {
                    //队列消息发送成功
                    printf("Key Press Message Sent!\r\n");
                }
                else
                {
                    //队列消息发送失败
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

/************************************ Defines***********************************/
