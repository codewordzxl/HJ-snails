#ifndef INCLUDE_H_
#define INCLUDE_H_

/*
 * 包含Cortex-M内核的通用头文件
 */
#include    <stdio.h>                       //printf
#include    <string.h>                      //memcpy
#include    <stdlib.h>                      //malloc

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"
/*----------------------------------------------------------------
                   底层驱动文件
------------------------------------------------------------------*/
#include "fsl_common.h"
#include "fsl_flash.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_uart.h"
/*----------------------------------------------------------------
                    龙邱模块驱动
------------------------------------------------------------------*/
#include "MKV58_GPIO_Cfg.h"
#include "MKV58_GPIO.h"           
#include "delay.h"
#include "system_init.h"
#include "system.h"
#include "LQ_12864.h"
#include "LQ_UART.h"
#include "MKV58_ADC.h"            
#include "ZXL_ADC.h"                
#include "MKV58_FTM.h"  
#include "MKV58_PIT.h"         
#include "pid.h"
#include "Round.h"
#include "Element.h"

#endif
