/*-----------------------------------------------------------------------------------------------------
【平    台】龙邱K60核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年04月02日
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【编译平台】IAR 8.2
【功    能】看门狗例子
【注意事项】
-------------------------------------------------------------------------------------------------------*/
#include "include.h"
#include "fsl_wdog.h"


/*------------------------------------------------------------------------------------------------------
【函    数】Test_Wdog
【功    能】测试看门狗
【参    数】无
【返 回 值】无
【实    例】Test_Wdog(); //测试看门狗
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void Test_Wdog(void)
{
    wdog_config_t config;
    
    LED_Init();
    
    UART4_Init();
    
    
    printf("看门狗测试例程 \n");
    
    WDOG_GetDefaultConfig(&config);   //获取默认参数
    
    config.clockSource = kWDOG_LpoClockSource;    //看门狗时钟 1KHz
    config.enableWindowMode = 0;                  //只设置超时时间
    config.timeoutValue     = 500;                //设置500ms  
    
    WDOG_Init(WDOG, &config);    //初始化看门狗
    
    delayms(200); 
    WDOG_Refresh(WDOG);        //喂狗
    printf("喂狗成功 200ms 不复位 \n");
    
    delayms(300);
    WDOG_Refresh(WDOG);        //喂狗
    printf("喂狗成功 300ms 不复位 \n");
    
    delayms(510);
    WDOG_Refresh(WDOG);        //喂狗
    printf("喂狗成功 510ms 不复位 \n");
    
    while(1);


}