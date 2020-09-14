/*-----------------------------------------------------------------------------------------------------
【平    台】龙邱K60核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年04月02日
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【编译平台】IAR 8.2
【功    能】LPTMR例子
【注意事项】
-------------------------------------------------------------------------------------------------------*/
#include "include.h"
#include "LQ_LPTMR.h"



/*------------------------------------------------------------------------------------------------------
【函    数】Test_LptmrTimer
【功    能】测试LPTMR
【参    数】无
【返 回 值】无
【实    例】Test_LptmrTimer(); //测试Lptmr定时器
【注意事项】同一时间只能使用LPTMR的一种功能  delayms一般是用的LPTMR 
--------------------------------------------------------------------------------------------------------*/
void Test_LptmrTimer(void)
{
    LED_Init();
    
    UART4_Init();
    
    printf("LPTMR程序计时器测试例程  \n ");
    printf("同一时间只能使用LPTMR的一种功能  \n ");
    
    LPTMR_TimeStartus();
    while(1)
    {
        if(LPTMR_TimeGetus() >= 5000)
        {
            LED_Reverse(0);              //用示波器测A17频率 100Hz 用于测试LPTMR计时器是否准确
            LPTMR_TimeStartus();
        }   
    }

}


/*------------------------------------------------------------------------------------------------------
【函    数】Test_LptmrPulse
【功    能】测试LPTMR脉冲计数
【参    数】无
【返 回 值】无
【实    例】Test_LptmrPulse(); //测试Lptmr脉冲计数
【注意事项】同一时间只能使用LPTMR的一种功能  delayms一般是用的LPTMR 
【注意事项】可以使用一个普通引脚和带方向编码器的方向相连，PTC5和编码器的脉冲相连 
【注意事项】同一时间只能使用LPTMR的一种功能
--------------------------------------------------------------------------------------------------------*/
void Test_LptmrPulse(void)
{
    LED_Init();
    
    UART4_Init();
    
    printf("LPTMR脉冲计数测试例程  \n ");
    printf("同一时间只能使用LPTMR的一种功能  \n ");

    /* 使用PTC5 捕获上升沿脉冲 */
    LPTMR_PulseInit(LPT0_ALT2, 0xFFFF, LPT_Rising);
    
    while(1)
    {
        PIT_Delayms(PIT0, 10);
        
        printf("捕获到脉冲 %d \n ",LPTMR_PulseGet());
        
        LPTMR_PulseClean();
    }
}









