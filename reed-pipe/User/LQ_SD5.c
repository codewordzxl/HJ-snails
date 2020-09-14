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
    KEY_Init();
    OLED_Init();
    OLED_CLS();
    FTM_PwmInit(FTM3, FTM_CH6, 50, 0);
    uint16_t duty = 750;
    char txt[16];
    while(1)
    { 
        if(GPIO_Pin_Read(PTB0)==0){
            duty = 750;
            FTM_PwmDuty(FTM3, FTM_CH6, duty);     //50Hz һ������20ms  1.5 / 20 * FTM_PRECISON  = 75
            OLED_CLS();
            sprintf(txt,"angle:%d",duty);
            OLED_P8x16Str(20,4,(uint8_t*)txt);
        }
        if(GPIO_Pin_Read(PTB1)==0){
            LED_ON(1);
            duty += 50;
            if(duty > 1250)        //�����Լ���ʵ������Զ����ǽ������ƣ���ֹ����
            {
                duty = 1250;
            }
            FTM_PwmDuty(FTM3, FTM_CH6, duty); 
            OLED_CLS();
            sprintf(txt,"angle:%d ",duty);
            OLED_P8x16Str(20,4,(uint8_t*)txt);
        }
        if(GPIO_Pin_Read(PTB2)==0){
            LED_ON(2);
            duty -= 50;
            if(duty < 250)        //�����Լ���ʵ������Զ����ǽ������ƣ���ֹ����
            {
                duty = 250;
            }
			FTM_PwmDuty(FTM3, FTM_CH6, duty); 
            OLED_CLS();
            sprintf(txt,"angle:%d ",duty);
            OLED_P8x16Str(20,4,(uint8_t*)txt);
        }
        delayms(100);

    }

}
