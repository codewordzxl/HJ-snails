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
#include "fsl_smc.h"
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
#include "MKV58_PIT.h"             /* PIT       驱动 */
#include "LQ_UART.h"               /* 串口      例子 */
#include "LQ_12864.h"              /* OLED      驱动 */
#include "MKV58_ADC.h"             /* ADC       驱动 */
#include "MKV58_FTM.h"             /* FTM       驱动 */
#include "MKV58_DMA.h"             /* DMA       驱动 */
#include "LQ_MT9V034.h"            /* 神眼      驱动 */
#include "LQ_CAMERA.h"             /* 摄像头    例子 */

#include "Starting_line.h"
#include "System_Init.h"
#include "System.h"
#include "Pid.h"

#define LQ_OLED     

#endif
