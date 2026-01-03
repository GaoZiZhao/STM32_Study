/******************************************************************************
  * Copyright (C) 2026 SWPE, Inc.(Gmbh) or its affiliates.
  * 
  * ALL Rights Reserved.
  * 
  * @file key.c
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
