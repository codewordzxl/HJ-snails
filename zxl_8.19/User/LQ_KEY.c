/*-----------------------------------------------------------------------------------------------------
【平    台】龙邱MKV58核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年04月02日
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【编译平台】IAR 8.2
【功    能】
【注意事项】
-------------------------------------------------------------------------------------------------------*/
#include "include.h"

/*------------------------------------------------------------------------------------------------------
【函    数】LED_Init
【功    能】初始化核心板和母板上的LED
【参    数】无
【返 回 值】无
【实    例】LED_Init(); //初始化LED
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void KEY_Init(void)
{
   /* 开启时钟 */
    CLOCK_EnableClock(kCLOCK_PortB);
	
	/* 设置IO复用功能为GPIO */
    PORT_SetPinMux(PORTB, 20U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTB, 21U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTB, 22U, kPORT_MuxAsGpio);
	
	/* GPIO配置结构体 */
    gpio_pin_config_t led_config = {
        							kGPIO_DigitalInput,      //输入
									1,                       //无效配置             
    };
	
	/* 初始化母板上的按键 */
   GPIO_PinInit(GPIOB, 20, &led_config);
   GPIO_PinInit(GPIOB, 21, &led_config);
   GPIO_PinInit(GPIOB, 22, &led_config);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】LQ-005
【功能说明】按键处理函数
【软件版本】V1.0
【最后更新】2017年11月24日 
【函数名】
【返回值】0：无按键按下 1：按键1按下  2:按键2按下  3:按键3按下
【参数值】mode:0,不支持连续按;1,支持连续按;
【例子  】KEY_Read(0);   //检测三个按键哪个按下
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8_t KEY_Read(uint8_t mode)
{
    static uint8_t key_up=1;     //按键松开标志
    if(mode==1) 
    {
      key_up=1;      //支持连按
    }
    if(key_up && (GPIO_PinRead(GPIOB, 20)==0 || GPIO_PinRead(GPIOB, 21)==0) || GPIO_PinRead(GPIOB, 22)==0)
    {
      delayms(100);   //消抖
      key_up=0;
      if(GPIO_PinRead(GPIOB, 20)==0)      
      {
          return 1;
      }
      
      else if(GPIO_PinRead(GPIOB, 21)==0) 
      {
          return 2;    
      }
      
      else if(GPIO_PinRead(GPIOB, 22)==0) 
      {
          return 3;    
      }
      
    }
    if(GPIO_PinRead(GPIOB, 20)==1 && GPIO_PinRead(GPIOB, 21)==1 && GPIO_PinRead(GPIOB, 22)==1) 
    {
     key_up=1;   
    }
    return 0;   //无按键按下
     
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】LQ-005
【功能说明】测试按键  
【软件版本】V1.0
【最后更新】2017年11月24日 
【函数名】
【返回值】无
【参数值】无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_KEY(void)
{ 
    LED_Init();
    KEY_Init(); 
    while (1)
    {  
        //测试按键      

        switch(KEY_Read(1))  //
        {
            case 1:
                LED_Reverse(0);
                break;           
            case 2:      
                LED_Reverse(1);
                break;
            case 3:      
                LED_Reverse(2);
                break;
            default:
                LED_Reverse(3);
                break;
        }
        //延时
        delayms(50);
    }
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】LQ-005
【功能说明】测试按键  
【软件版本】V1.0
【最后更新】2017年11月24日 
【函数名】
【返回值】无
【参数值】无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
/* 按键外部中断测试标志位 */
volatile uint8_t key_exti_flag = 0;
void Test_ExtiKEY(void)
{ 
	LED_Init();
	
	
	/* 开启时钟 */
	CLOCK_EnableClock(kCLOCK_PortB);
	
	/* 设置IO复用功能为GPIO */
	PORT_SetPinMux(PORTB, 20U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTB, 21U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTB, 22U, kPORT_MuxAsGpio);
	
	/* GPIO配置结构体 */
	gpio_pin_config_t led_config = {
		kGPIO_DigitalInput,      //输入
		1,                       //无效配置             
	};
	
	/* 初始化母板上的按键为输入模式 */
	GPIO_PinInit(GPIOB, 20, &led_config);
	GPIO_PinInit(GPIOB, 21, &led_config);
	GPIO_PinInit(GPIOB, 22, &led_config);
	
	
	const port_pin_config_t porte_pin_config = { /* 内部上拉 */
                                                  kPORT_PullUp,
                                                  /* IO翻转速度快 */
                                                  kPORT_FastSlewRate,
                                                  /* 禁用无源滤波器 */
                                                  kPORT_PassiveFilterDisable,
                                                  /* 关闭开漏模式 */
                                                  kPORT_OpenDrainDisable,
                                                  /* 配置低驱动强度 */
                                                  kPORT_LowDriveStrength,
                                                  /* 配置为GPIO */
                                                  kPORT_MuxAsGpio,
                                                  /* Pin控制寄存器字段[15:0]未锁定 */
                                                  kPORT_UnlockRegister};
    /* 配置GPIO */
    PORT_SetPinConfig(PORTB, 20U, &porte_pin_config);
	PORT_SetPinConfig(PORTB, 21U, &porte_pin_config);
	PORT_SetPinConfig(PORTB, 22U, &porte_pin_config);
	
	/* 配置为下降沿中断 */
	PORT_SetPinInterruptConfig(PORTB, 20, kPORT_InterruptFallingEdge);
	PORT_SetPinInterruptConfig(PORTB, 21, kPORT_InterruptFallingEdge);
	PORT_SetPinInterruptConfig(PORTB, 22, kPORT_InterruptFallingEdge);
    
    /* 优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断 */
    NVIC_SetPriority(PORTB_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    NVIC_EnableIRQ(PORTB_IRQn);			         //使能PORTB_IRQn的中断  
    while (1)
    {  
        //测试按键      

        switch(key_exti_flag)  //
        {
            case 1:
                LED_Reverse(0);
                break;           
            case 2:      
                LED_Reverse(1);
                break;
            case 3:      
                LED_Reverse(2);
                break;
            default:
                LED_Reverse(3);
                break;
        }
        //延时
        delayms(50);
    }
}
