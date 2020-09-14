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
void LED_Init(void)
{
	
	/* 开启时钟 */
    CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_PortE);
	
    /* 设置IO复用功能为GPIO */
    PORT_SetPinMux(PORTA, 17U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTC,  0U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTD, 15U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTE, 29U, kPORT_MuxAsGpio);
   
	PORT_SetPinMux(PORTC, 18U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTC, 19U, kPORT_MuxAsGpio);
	
	/* GPIO配置结构体 */
	gpio_pin_config_t led_config = {
		kGPIO_DigitalOutput,     //输出
		1,                       //默认高电平                
	};
	
	/* 初始化母板上的LED */
	GPIO_PinInit(GPIOA, 17, &led_config);
	GPIO_PinInit(GPIOC,  0, &led_config);
	GPIO_PinInit(GPIOD, 15, &led_config);
	GPIO_PinInit(GPIOE, 29, &led_config);
	GPIO_PinInit(GPIOC, 18, &led_config);
	GPIO_PinInit(GPIOC, 19, &led_config);
}

/*------------------------------------------------------------------------------------------------------
【函    数】LED_ON
【功    能】开灯
【参    数】led   :  LED号
【返 回 值】无
【实    例】LED_ON(1); //开LED
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void LED_ON(uint8_t led)
{
    switch(led)
    {
      case 0:
        GPIO_PinWrite(GPIOA, 17, 0);
        break;
        
      case 1:
        GPIO_PinWrite(GPIOC,  0, 0);
        break;
        
      case 2:
        GPIO_PinWrite(GPIOD, 15, 0);
        break;
        
      case 3:
        GPIO_PinWrite(GPIOE, 29, 0);
        break;
        
      case 4:
        GPIO_PinWrite(GPIOC, 18, 0);
        break;
        
      case 5:
        GPIO_PinWrite(GPIOC, 19, 0);
        break;
        
      default:
        break;
    }
}


/*------------------------------------------------------------------------------------------------------
【函    数】LED_OFF
【功    能】关灯
【参    数】led   :  LED号
【返 回 值】无
【实    例】LED_OFF(1); //关LED
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void LED_OFF(uint8_t led)
{

    switch(led)
    {
      case 0:
        GPIO_PinWrite(GPIOA, 17, 1);
        break;
        
      case 1:
        GPIO_PinWrite(GPIOC,  0, 1);
        break;
        
      case 2:
        GPIO_PinWrite(GPIOD, 15, 1);
        break;
        
      case 3:
        GPIO_PinWrite(GPIOE, 29, 1);
        break;
        
      case 4:
        GPIO_PinWrite(GPIOC, 18, 1);
        break;
        
      case 5:
        GPIO_PinWrite(GPIOC, 19, 1);
        break;
        
      default:
        break;
    }
}
/*------------------------------------------------------------------------------------------------------
【函    数】LED_Reverse
【功    能】翻转LED
【参    数】led   :  LED号
【返 回 值】无
【实    例】LED_Reverse(1); //翻转LED
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void LED_Reverse(uint8_t led)
{
    /* 记录LED状态 */
    static uint8_t led_state[6];
    led_state[led]++;
    
    if(0 == led_state[led]%2)
    {
        LED_ON(led);
    }
    else
    {
        LED_OFF(led);
    }
}

/*------------------------------------------------------------------------------------------------------
【函    数】LED_Test
【功    能】翻转LED
【参    数】led   :  LED号
【返 回 值】无
【实    例】LED_Test(); //测试LED
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void Test_LED(void)
{
    LED_Init();
    while(1)
    {
        LED_ON(0);
        LED_OFF(1);
        LED_OFF(2);
        LED_OFF(3);
        delayms(100);
        
        LED_OFF(0);
        LED_ON(1);
        LED_OFF(2);
        LED_OFF(3);
        delayms(100);
        
        LED_OFF(0);
        LED_OFF(1);
        LED_ON(2);
        LED_OFF(3);
        delayms(100);
        
        LED_OFF(0);
        LED_OFF(1);
        LED_OFF(2);
        LED_ON(3);
        delayms(100);
        
        LED_Reverse(4);
        LED_Reverse(5);
    }
}
