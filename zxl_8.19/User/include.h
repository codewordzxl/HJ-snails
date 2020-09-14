#ifndef INCLUDE_H_
#define INCLUDE_H_

/*
 * ����Cortex-M�ں˵�ͨ��ͷ�ļ�
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
                   �ײ������ļ�
------------------------------------------------------------------*/
#include "fsl_smc.h"
#include "fsl_common.h"
#include "fsl_flash.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_uart.h"
/*----------------------------------------------------------------
                    ����ģ������
------------------------------------------------------------------*/
#include "MKV58_GPIO_Cfg.h"
#include "MKV58_GPIO.h"
#include "MKV58_PIT.h"             /* PIT       ���� */
#include "LQ_UART.h"               /* ����      ���� */
#include "LQ_12864.h"              /* OLED      ���� */
#include "MKV58_ADC.h"             /* ADC       ���� */
#include "MKV58_FTM.h"             /* FTM       ���� */
#include "MKV58_DMA.h"             /* DMA       ���� */
#include "LQ_MT9V034.h"            /* ����      ���� */
#include "LQ_CAMERA.h"             /* ����ͷ    ���� */

#include "Starting_line.h"
#include "System_Init.h"
#include "System.h"
#include "Pid.h"

#define LQ_OLED     

#endif
