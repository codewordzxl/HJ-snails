/*-----------------------------------------------------------------------------------------------------
��ƽ    ̨������K60���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0������Դ���룬�����ο�������Ը�
�������¡�2019��04��02��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
������ƽ̨��IAR 8.2
����    �ܡ����Գ�����
��ע�����
-------------------------------------------------------------
�������ӿ�
              
GND            GND 
VCC            5V
Trig           B0
Echo           B1           
-------------------------------------------------------------
-------------------------------------------------------------------------------------------------------*/
#include "include.h"

#define HC_TRIG   PTB0   //���������ź�����
#define HC_ECHO   PTB1   //����ߵ�ƽ����


/*------------------------------------------------------------------------------------------------------
����    ����Test_HCSR04
����    �ܡ����Գ�����ģ��
����    ������
���� �� ֵ����
��ʵ    ����Test_HCSR04();
��ע�����
--------------------------------------------------------------------------------------------------------*/
void Test_HCSR04(void)
{
	/* ��ʼ����������IO */
	GPIO_Pin_Init(HC_TRIG, GPO, 0);  
  	GPIO_Pin_Init(HC_ECHO, GPI, 0);

	LED_Init();
    UART4_Init();
	systime.init();
	printf("\r\nLQ HC-SR04 Test");
	
#ifdef LQ_OLED
	OLED_Init();
    OLED_CLS();
	OLED_P8x16Str(5,0,"LQ HC-SR04 Test");
#else
	TFTSPI_Init(1);                //TFT1.8��ʼ��  0��������ʾ  1��������ʾ  
    TFTSPI_CLS(u16BLUE);           //����
	TFTSPI_P8X16Str(0,0,"LQ HC-SR04 Test",u16RED,u16BLUE);
#endif
	char txt[16];
	uint64_t now_time = systime.get_time_us();
	static uint8_t count = 0;
	uint64_t hc_time;
	float dis = 0;
	while(1)
	{
		if(systime.get_time_us() - now_time >= 20000)
		{
			now_time = systime.get_time_us();
			
			/* HC_TRIG ����8-15us�ĸߵ�ƽ */
			GPIO_Pin_Write(HC_TRIG, 1);
			systime.delay_us(10);
			GPIO_Pin_Write(HC_TRIG, 0);
			
			/* ������ģ�鿪ʼ���ͳ����� */
			while(!GPIO_Pin_Read(HC_ECHO));
			
			/* ��¼��ǰʱ�� */
			hc_time = systime.get_time_us();
			
			/* ������ģ���յ����������� */
			while(GPIO_Pin_Read(HC_ECHO));
		
			/* ����ߵ�ƽʱ�� */
			hc_time = systime.get_time_us() - hc_time;
			
			/* ������� hc_time/2 * 340m/s */
			dis = (float)hc_time * 0.017;          //�������dis
			
			if(count++ % 5 == 0)
			{
				sprintf(txt, "Dis:%5.2fcm", dis);
#ifdef LQ_OLED
				OLED_P8x16Str(0,3,txt);
#else
				TFTSPI_P8X16Str(2,3,txt,u16RED,u16BLUE);
#endif
			}
		}
	}
}

//---------------------------------------------------------------------------
//hc_starttime ��¼���������ECHO �ߵ�ƽ��ʼʱ��  hc_time ��¼ECHO�ߵ�ƽ����ʱ��
//--------------------------------------------------------------------------- 
volatile uint64_t hc_starttime; 
volatile uint16_t hc_time;

/*------------------------------------------------------------------------------------------------------
����    ����Test_HCSR04
����    �ܡ����Գ�����ģ��  ���ж��ж�ȡʱ��
����    ������
���� �� ֵ����
��ʵ    ����Test_HCSR04();
��ע�����
--------------------------------------------------------------------------------------------------------*/
void Test_HCSR(void)
{
	/* ��ʼ����������IO */
	GPIO_Pin_Init(HC_TRIG, GPO, 0); 
	
  	GPIO_ExtiInit(HC_ECHO, either_down);

	 /* ���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж� */
    NVIC_SetPriority(PORTB_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    NVIC_EnableIRQ(PORTB_IRQn);			         //ʹ��PORTB_IRQn���ж�  
	
	
	LED_Init();
    UART4_Init();
	systime.init();
	printf("\r\nLQ HC-SR04 Test");
	
#ifdef LQ_OLED
	OLED_Init();
    OLED_CLS();
	OLED_P8x16Str(5,0,"LQ HC-SR04 Test");
#else
	TFTSPI_Init(1);                //TFT1.8��ʼ��  0��������ʾ  1��������ʾ  
    TFTSPI_CLS(u16BLUE);           //����
	TFTSPI_P8X16Str(0,0,"LQ HC-SR04 Test",u16RED,u16BLUE);
#endif
	char txt[16];
	uint64_t now_time = systime.get_time_us();
	static uint8_t count = 0;
	float dis = 0;
	while(1)
	{
		if(systime.get_time_us() - now_time >= 20000)
		{
			now_time = systime.get_time_us();
			
			/* HC_TRIG ����8-15us�ĸߵ�ƽ */
			GPIO_Pin_Write(HC_TRIG, 1);
			systime.delay_us(10);
			GPIO_Pin_Write(HC_TRIG, 0);
			
			/* ������� hc_time/2 * 340m/s */
			dis = (float)hc_time * 0.017;          //�������dis
			
			if(count++ % 5 == 0)
			{
				sprintf(txt, "Dis:%5.2fcm", dis);
#ifdef LQ_OLED
				OLED_P8x16Str(0,3,txt);
#else
				TFTSPI_P8X16Str(2,3,txt,u16RED,u16BLUE);
#endif
			}
		}
	}
}