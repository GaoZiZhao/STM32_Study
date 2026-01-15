/*******************************************************************************
  * Copyright (C) 2026 SWPE, Inc.(Gmbh) or its affiliates.
  * 
  * ALL Rights Reserved.
  * 
  * @file bsp_led_driver.h
  * 
  * @par dependencies
  * - stdint.h
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

#ifndef BSP_LED_DRIVER_H
#define BSP_LED_DRIVER_H

/********************************** Includes  *********************************/

#include <stdint.h>
#include <stdio.h>

/*********************************** Includes *********************************/

/*********************************** Defines **********************************/

#define INITED             1        /*LED is Inited.                          */
#define NOT_INITED         0        /*LED is NotInited.                       */

#define OS_SUPPORTING               /*OS_SUPPORTING depending on OS avaliable */

#define DEBUG                       /*Enable DEBUG                            */
#define DEBUG_OUT(x)  printf(x)     /*DEBUG output infoto indicate statues    */

typedef enum
{
    LED_OK                = 0,         /* Operation completed successfully.   */
    LED_ERROR             = 1,         /* Run-time error without case matched.*/
    LED_ERRORTIMEOUT      = 2,         /* Operation timeout.                  */
    LED_ERRORRESOURCE     = 3,         /* Resource not available.             */
    LED_ERRORPARAMETER    = 4,         /* Parameter error.                    */
    LED_ERRORNOMEMORY     = 5,         /* Out of memory.                      */
    LED_ERRORISR          = 6,         /* Not allowed in ISR context.         */
    LED_RESERVED          = 0X7FFFFFFF /* Reserved                            */
}led_status_t;

typedef enum
{
    PROPORTION_1_3 = 0,                /* Operation completed successfully.   */
    PROPORTION_1_2 = 1,                /* Operation completed successfully.   */
    PROPORTION_1_1 = 2,                /* Operation completed successfully.   */
    PROPORTION_x_x = 3,                /* Reserved                            */
} propotion_t;

typedef struct
{
    led_status_t (*pf_led_on)                 (void);
	led_status_t (*pf_led_off)                (void);
} led_operations_t;

typedef struct
{
    led_status_t (*pf_get_time_ms) (uint32_t * const);
} time_base_ms_t;

#ifdef OS_SUPPORTING
typedef struct
{
    led_status_t (*pf_os_delay_ms)      (const uint32_t);
} os_delay_t;
#endif  //OS_SUPPORTING

typedef led_status_t (*pf_led_control_t) (
	                                        uint32_t ,      //   Cyccle_time[ms]
	                                        uint32_t ,      //Blink_times[times]
	                                        propotion_t     //proporation_on_off
	                                                   );
	                 

typedef struct
{
    /**** **Target of Internal Status********/
    uint8_t                         is_inited;
    /***********Target of Features***********/
    /* The whole time of blink              */
    uint32_t                    cycle_time_ms;
    /* The times of blink                   */
    uint32_t                      blink_times;
    /* The retationship of light on and off */
    uint32_t                proportion_on_off;

	/***********Target of IOs needed*********/
	/* The interface form core layer        */
	led_operations_t          *p_led_ops_inst;
	time_base_ms_t            *p_time_base_ms;
	/* The interface form OS layer          */
#ifdef OS_SUPPORTING
	os_delay_t                *p_os_time_delay;
#endif // OS_SUPPORTING

	/*************Target of APIs*************/
	pf_led_control_t        *pf_led_controler;                                
} bsp_led_driver_t;

/*********************************** Defines **********************************/
//bsp_led_driver_t led;


/********************************** Declaring *********************************/
/**
 * @brief  instantiate the target of bsp_led_driver_t.
 * 
 * Steps:
 * 1. Adding the Core interfaces into target of bsp_led_driver instance.
 * 2. Adding the OS interfaces into target of bsp_led_driver instance.
 * 3. Adding the timebase interfaces into target of bsp_led_driver instance.
 * 
 * @param[in] p_data : Pointer to the input data.
 * @param[in] length : Length of the input data.
 * @param[in] p_data : Pointer to the input data.
 * @param[in] p_data : Pointer to the input data.
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
                                    time_base_ms_t   * const time_base        
                                                                      );


/********************************** Declaring *********************************/


#endif /* BSP_LED_DRIVER_H */

