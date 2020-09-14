/*-----------------------------------------------------------------------------------------------------
【平    台】龙邱K60核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年04月02日
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【编译平台】IAR 8.2
【功    能】测试CMT输出PWM控制SD5舵机
【注意事项】
-------------------------------------------------------------------------------------------------------*/
#include "include.h"
#include "LQ_SD5.h"


/*------------------------------------------------------------------------------------------------------
【函    数】Test_FTMSD5
【功    能】测试FTM输出PWM控制SD5舵机
【参    数】无
【返 回 值】无
【实    例】Test_FTMSD5(); //测试SD5舵机
【注意事项】注意，一定要对舵机打角进行限制
--------------------------------------------------------------------------------------------------------*/
void Test_FTMSD5(void)
{
    KEY_Init();
    OLED_Init();
    OLED_CLS();
    FTM_PwmInit(FTM3, FTM_CH6, 50, 0);
    uint16_t duty = 750;
    char txt[16];
    while(1)
    { 
        if(GPIO_Pin_Read(PTB0)==0){
            duty = 750;
            FTM_PwmDuty(FTM3, FTM_CH6, duty);     //50Hz 一个周期20ms  1.5 / 20 * FTM_PRECISON  = 75
            OLED_CLS();
            sprintf(txt,"angle:%d",duty);
            OLED_P8x16Str(20,4,(uint8_t*)txt);
        }
        if(GPIO_Pin_Read(PTB1)==0){
            LED_ON(1);
            duty += 50;
            if(duty > 1250)        //根据自己的实际情况对舵机打角进行限制，防止卡死
            {
                duty = 1250;
            }
            FTM_PwmDuty(FTM3, FTM_CH6, duty); 
            OLED_CLS();
            sprintf(txt,"angle:%d ",duty);
            OLED_P8x16Str(20,4,(uint8_t*)txt);
        }
        if(GPIO_Pin_Read(PTB2)==0){
            LED_ON(2);
            duty -= 50;
            if(duty < 250)        //根据自己的实际情况对舵机打角进行限制，防止卡死
            {
                duty = 250;
            }
			FTM_PwmDuty(FTM3, FTM_CH6, duty); 
            OLED_CLS();
            sprintf(txt,"angle:%d ",duty);
            OLED_P8x16Str(20,4,(uint8_t*)txt);
        }
        delayms(100);

    }

}
