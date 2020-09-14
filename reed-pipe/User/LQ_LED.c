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
   GPIO_Pin_Init(PTA17, GPO, 1);
   GPIO_Pin_Init(PTC0 , GPO, 1);
   GPIO_Pin_Init(PTD15, GPO, 1);
   GPIO_Pin_Init(PTE29, GPO, 1);
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
    static uint8_t led_state[4];
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
