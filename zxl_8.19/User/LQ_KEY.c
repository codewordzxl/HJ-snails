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
void KEY_Init(void)
{
   /* ����ʱ�� */
    CLOCK_EnableClock(kCLOCK_PortB);
	
	/* ����IO���ù���ΪGPIO */
    PORT_SetPinMux(PORTB, 20U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTB, 21U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTB, 22U, kPORT_MuxAsGpio);
	
	/* GPIO���ýṹ�� */
    gpio_pin_config_t led_config = {
        							kGPIO_DigitalInput,      //����
									1,                       //��Ч����             
    };
	
	/* ��ʼ��ĸ���ϵİ��� */
   GPIO_PinInit(GPIOB, 20, &led_config);
   GPIO_PinInit(GPIOB, 21, &led_config);
   GPIO_PinInit(GPIOB, 22, &led_config);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����  �ߡ�LQ-005
������˵��������������
������汾��V1.0
�������¡�2017��11��24�� 
����������
������ֵ��0���ް������� 1������1����  2:����2����  3:����3����
������ֵ��mode:0,��֧��������;1,֧��������;
������  ��KEY_Read(0);   //������������ĸ�����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8_t KEY_Read(uint8_t mode)
{
    static uint8_t key_up=1;     //�����ɿ���־
    if(mode==1) 
    {
      key_up=1;      //֧������
    }
    if(key_up && (GPIO_PinRead(GPIOB, 20)==0 || GPIO_PinRead(GPIOB, 21)==0) || GPIO_PinRead(GPIOB, 22)==0)
    {
      delayms(100);   //����
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
    return 0;   //�ް�������
     
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����  �ߡ�LQ-005
������˵�������԰���  
������汾��V1.0
�������¡�2017��11��24�� 
����������
������ֵ����
������ֵ����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_KEY(void)
{ 
    LED_Init();
    KEY_Init(); 
    while (1)
    {  
        //���԰���      

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
        //��ʱ
        delayms(50);
    }
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����  �ߡ�LQ-005
������˵�������԰���  
������汾��V1.0
�������¡�2017��11��24�� 
����������
������ֵ����
������ֵ����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
/* �����ⲿ�жϲ��Ա�־λ */
volatile uint8_t key_exti_flag = 0;
void Test_ExtiKEY(void)
{ 
	LED_Init();
	
	
	/* ����ʱ�� */
	CLOCK_EnableClock(kCLOCK_PortB);
	
	/* ����IO���ù���ΪGPIO */
	PORT_SetPinMux(PORTB, 20U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTB, 21U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTB, 22U, kPORT_MuxAsGpio);
	
	/* GPIO���ýṹ�� */
	gpio_pin_config_t led_config = {
		kGPIO_DigitalInput,      //����
		1,                       //��Ч����             
	};
	
	/* ��ʼ��ĸ���ϵİ���Ϊ����ģʽ */
	GPIO_PinInit(GPIOB, 20, &led_config);
	GPIO_PinInit(GPIOB, 21, &led_config);
	GPIO_PinInit(GPIOB, 22, &led_config);
	
	
	const port_pin_config_t porte_pin_config = { /* �ڲ����� */
                                                  kPORT_PullUp,
                                                  /* IO��ת�ٶȿ� */
                                                  kPORT_FastSlewRate,
                                                  /* ������Դ�˲��� */
                                                  kPORT_PassiveFilterDisable,
                                                  /* �رտ�©ģʽ */
                                                  kPORT_OpenDrainDisable,
                                                  /* ���õ�����ǿ�� */
                                                  kPORT_LowDriveStrength,
                                                  /* ����ΪGPIO */
                                                  kPORT_MuxAsGpio,
                                                  /* Pin���ƼĴ����ֶ�[15:0]δ���� */
                                                  kPORT_UnlockRegister};
    /* ����GPIO */
    PORT_SetPinConfig(PORTB, 20U, &porte_pin_config);
	PORT_SetPinConfig(PORTB, 21U, &porte_pin_config);
	PORT_SetPinConfig(PORTB, 22U, &porte_pin_config);
	
	/* ����Ϊ�½����ж� */
	PORT_SetPinInterruptConfig(PORTB, 20, kPORT_InterruptFallingEdge);
	PORT_SetPinInterruptConfig(PORTB, 21, kPORT_InterruptFallingEdge);
	PORT_SetPinInterruptConfig(PORTB, 22, kPORT_InterruptFallingEdge);
    
    /* ���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж� */
    NVIC_SetPriority(PORTB_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    NVIC_EnableIRQ(PORTB_IRQn);			         //ʹ��PORTB_IRQn���ж�  
    while (1)
    {  
        //���԰���      

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
        //��ʱ
        delayms(50);
    }
}
