/*-----------------------------------------------------------------------------------------------------
��ƽ    ̨������MKV58���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0������Դ���룬�����ο�������Ը�
�������¡�2019��04��02��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
������ƽ̨��IAR 8.2
����    �ܡ�
��ע�����
-------------------------------------------------------------------------------------------------------*/
#include "include.h"

/*------------------------------------------------------------------------------------------------------
����    ����LED_Init
����    �ܡ���ʼ�����İ��ĸ���ϵ�LED
����    ������
���� �� ֵ����
��ʵ    ����LED_Init(); //��ʼ��LED
��ע�����
--------------------------------------------------------------------------------------------------------*/
void LED_Init(void)
{
	
	/* ����ʱ�� */
    CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_PortE);
	
    /* ����IO���ù���ΪGPIO */
    PORT_SetPinMux(PORTA, 17U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTC,  0U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTD, 15U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTE, 29U, kPORT_MuxAsGpio);
   
	PORT_SetPinMux(PORTC, 18U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTC, 19U, kPORT_MuxAsGpio);
	
	/* GPIO���ýṹ�� */
	gpio_pin_config_t led_config = {
		kGPIO_DigitalOutput,     //���
		1,                       //Ĭ�ϸߵ�ƽ                
	};
	
	/* ��ʼ��ĸ���ϵ�LED */
	GPIO_PinInit(GPIOA, 17, &led_config);
	GPIO_PinInit(GPIOC,  0, &led_config);
	GPIO_PinInit(GPIOD, 15, &led_config);
	GPIO_PinInit(GPIOE, 29, &led_config);
	GPIO_PinInit(GPIOC, 18, &led_config);
	GPIO_PinInit(GPIOC, 19, &led_config);
}

/*------------------------------------------------------------------------------------------------------
����    ����LED_ON
����    �ܡ�����
����    ����led   :  LED��
���� �� ֵ����
��ʵ    ����LED_ON(1); //��LED
��ע�����
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
����    ����LED_OFF
����    �ܡ��ص�
����    ����led   :  LED��
���� �� ֵ����
��ʵ    ����LED_OFF(1); //��LED
��ע�����
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
����    ����LED_Reverse
����    �ܡ���תLED
����    ����led   :  LED��
���� �� ֵ����
��ʵ    ����LED_Reverse(1); //��תLED
��ע�����
--------------------------------------------------------------------------------------------------------*/
void LED_Reverse(uint8_t led)
{
    /* ��¼LED״̬ */
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
����    ����LED_Test
����    �ܡ���תLED
����    ����led   :  LED��
���� �� ֵ����
��ʵ    ����LED_Test(); //����LED
��ע�����
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
