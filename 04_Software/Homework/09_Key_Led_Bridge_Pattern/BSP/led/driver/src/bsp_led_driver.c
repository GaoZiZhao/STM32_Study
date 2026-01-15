/******************************************************************************
  * Copyright (C) 2026 SWPE, Inc.(Gmbh) or its affiliates.
  * 
  * ALL Rights Reserved.
  * 
  * @file bsp_led_driver.c
  * 
  * @par dependencies
  * - bsp_led_driver.h
  * - stdio.h
  * 
  * @author  GaoZizhao | SWPE. | Chengdu
  * 
  * @brief   Provide the HAL APIs for key and corresponding operations.
  * 
  * Processing flow:
  * 
  * call directly.
  * 
  * @version V1.0 2026-1-11
  * 
  * @note 1 tab == 4 spaces!
  * 
  *****************************************************************************/

/********************************** Includes  *********************************/
#include "bsp_led_driver.h"

/********************************** Includes  *********************************/

/*********************************** Defines **********************************/
/**
 * @brief  init the target of bsp_led_driver_t.
 * 
 * Steps:
 * 1. make the target at a specific status.
 * 
 * @param[in] self : Pointer to the input data.
 *
 * @return led_status_t : The status of running.
 * 
 **/

led_status_t led_driver_init(bsp_led_driver_t * const self)
{
    led_status_t ret = LED_OK;
#ifdef DEBUG
    DEBUG_OUT("led_driver_init Start...\r\n");
#endif // DEBUG
    if(NULL == self)
    {
    #ifdef DEBUG
        DEBUG_OUT("LED_ERRORPARAMETER\r\n");
        return LED_ERRORPARAMETER;
    #endif // DEBUG
    }
    self -> p_led_ops_inst -> pf_led_off();
	self -> p_os_time_delay -> pf_os_delay_ms(600);
	uint32_t time_stamp = 0;
	self -> p_time_base_ms -> pf_get_time_ms(&time_stamp);

    return ret;
}

/**
 * @brief  instantiate the target of bsp_led_driver_t.
 * 
 * Steps:
 * 1. Adding the Core interfaces into target of bsp_led_driver instance.
 * 2. Adding the OS interfaces into target of bsp_led_driver instance.
 * 3. Adding the timebase interfaces into target of bsp_led_driver instance.
 * 
 * @param[in] self : Pointer to the input data.
 * @param[in] led_ops : Length of the input data.
 * @param[in] os_delay : Pointer to the input data.
 * @param[in] time_base : Pointer to the input data.
 * 
 * @return led_status_t : The status of running.
 * 
 **/

led_status_t led_driver_inst (
								    bsp_led_driver_t * const      self,
						            led_operations_t * const   led_ops,
#ifdef OS_SUPPORTING
                                    os_delay_t       * const  os_delay,
#endif  // OS_SUPPORTING
                                    time_base_ms_t   * const time_base         )
{
    led_status_t ret = LED_OK;
    DEBUG_OUT("led_inst Kick-off\r\n");
/*********** checking the input parameters ***************/
    if(NULL == self     ||
       NULL == led_ops  ||
       NULL == os_delay ||
       NULL == time_base
                        )
        {
        #ifdef DEBUG
            DEBUG_OUT("LED_ERRORPARAMETER\r\n");
            return LED_ERRORPARAMETER;
        #endif // DEBUG
        }

/************* checking the resources ********************/
    if( INITED == self->is_inited)
    {
    #ifdef DEBUG
        DEBUG_OUT("LED_ERRORRESOURCE");
        return LED_ERRORRESOURCE;
    #endif // DEBUG
    }

#ifdef DEBUG
    DEBUG_OUT("led inst Start\r\n");
#endif // DEBUG

/*************** Adding the interface ********************/
    self -> p_led_ops_inst  =   led_ops;
    self -> p_os_time_delay =  os_delay;
    self -> p_time_base_ms  = time_base;

/****************** Init the target **********************/
    self -> blink_times        =              0;
    self -> cycle_time_ms      =              0;
    self -> proportion_on_off  = PROPORTION_x_x;

    ret = led_driver_init(self);
	if(LED_OK != ret)
	{
	#ifdef DEBUG
		DEBUG_OUT("LED init failed !\r\n");
	#endif // DEBUG
		self -> p_led_ops_inst  = NULL;
		self -> p_os_time_delay = NULL;
		self -> p_time_base_ms  = NULL;
		return ret;
	}
    
	self -> is_inited = INITED;
#ifdef DEBUG
	DEBUG_OUT("LED init finished !\r\n");
#endif // DEBUG
	
	return ret;
}


/*********************************** Defines **********************************/

