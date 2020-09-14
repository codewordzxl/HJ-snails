/*-----------------------------------------------------------------------------------------------------
��ƽ    ̨������K60���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0������Դ���룬�����ο�������Ը�
�������¡�2019��04��02��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
������ƽ̨��IAR 8.2
����    �ܡ�����CMT���PWM����SD5���
��ע�����
-------------------------------------------------------------------------------------------------------*/
#include "include.h"
#include "LQ_SD5.h"


/*------------------------------------------------------------------------------------------------------
����    ����Test_FTMSD5
����    �ܡ�����FTM���PWM����SD5���
����    ������
���� �� ֵ����
��ʵ    ����Test_FTMSD5(); //����SD5���
��ע�����ע�⣬һ��Ҫ�Զ����ǽ�������
--------------------------------------------------------------------------------------------------------*/
void Test_FTMSD5(void)
{
    LED_Init();
    KEY_Init();
    UART4_Init();
    FTM_PwmInit(FTM3, FTM_CH6, 50, 0);
	FTM_PwmInit(FTM3, FTM_CH7, 50, 0);
    printf("SD5����������� \n");
    printf("��һ��ʹ�ö����ͬѧ������֮ǰ��ò�Ҫװ����ֹ���������ת�ջ� \n");
    printf("����ʹ��50Hz��PWM�źſ��� \n");
    printf("ʹ��1.5ms��Ϊ�������ֵ \n");
    printf("����K0����������� \n");
    printf("����K1����������� \n");
    printf("����K2����������� \n");
    
    uint16_t duty = 750;
    while(1)
    {
        switch(KEY_Read(1))     
        {
          case 1:
            LED_Reverse(1); 
            duty = 750;
            FTM_PwmDuty(FTM3, FTM_CH6, duty);     //50Hz һ������20ms  1.5 / 20 * FTM_PRECISON  = 75
			FTM_PwmDuty(FTM3, FTM_CH7, duty);     //50Hz һ������20ms  1.5 / 20 * FTM_PRECISON  = 75
            break;           
          case 2: 
            LED_Reverse(2); 
            duty += 10;
            if(duty > 850)        //�����Լ���ʵ������Զ����ǽ������ƣ���ֹ����
            {
                duty = 850;
            }
            FTM_PwmDuty(FTM3, FTM_CH6, duty); 
			FTM_PwmDuty(FTM3, FTM_CH7, duty); 
            break;
          case 3: 
            LED_Reverse(3); 
            duty -= 10;
            if(duty < 650)        //�����Լ���ʵ������Զ����ǽ������ƣ���ֹ����
            {
                duty = 650;
            }
			FTM_PwmDuty(FTM3, FTM_CH6, duty); 
			FTM_PwmDuty(FTM3, FTM_CH7, duty); 
            break;
          default:
            
            break;
        }
        LED_Reverse(0);    
        delayms(100);
    }

}