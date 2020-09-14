/*-----------------------------------------------------------------------------------------------------
【平    台】龙邱KV58核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，基于官方SDK修改，仅供参考，后果自负
【最后更新】2019年04月02日
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【编译平台】IAR 8.2
【功    能】
【注意事项】
-------------------------------------------------------------*/
#include "include.h"

char txt[16];//OLED显示储存数据
/******************延时****************/
void delayms(uint16_t ms)
{
	volatile uint32_t i = 0;
	while(ms--)
	{
		for (i = 0; i < 20000; ++i)
		{
			__asm("NOP"); /* delay */
		}
	}	
}

int main(void)
{
    /* Init hardware */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    
    /* 设置中断优先级组  0: 0个抢占优先级16位个子优先级 
     * 1: 2个抢占优先级 8个子优先级 2: 4个抢占优先级 4个子优先级 
     * 3: 8个抢占优先级 2个子优先级 4: 16个抢占优先级 0个子优先级
     */
    NVIC_SetPriorityGrouping(2);  
    
    System_Init();//初始化
    Key_set();//拨码开关检测
    
    while(Controlvar.See_Data)//查看数据模式
    {
        ADC_Deal();//ADC读取及偏差计算
        OLED_CLS();
        int i;
        for(i=0;i<5;i++)
        {
            sprintf(txt,"adc%d:%d",i,g_ValueOfAD[i]);
            OLED_P6x8Str(0,i,(uint8_t *)txt);
        }
        sprintf(txt,"SP:%d",Deviation[0]);
        OLED_P6x8Str(0,5,(uint8_t *)txt);
        sprintf(txt,"X:%d",Deviation[1]);
        OLED_P6x8Str(0,6,(uint8_t *)txt);
        sprintf(txt,"L:%d",Level);
        OLED_P6x8Str(0,7,(uint8_t *)txt);
        delayms(1000);
    }
    
    while(1){
    
    }
	
    return 0;
}
