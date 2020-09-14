/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������KV58F24���ܳ�VDĸ��
����    д��CHIUSIR
��E-mail  ��chiusir@163.com
�������汾��V1.0
�������¡�2017��12��15��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR7.80.4
��Target  ��MKV58F1M0VLQ24
��Crystal �� 50.000Mhz
��busclock��120.000MHz
��pllclock��240.000MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef __GPIO_H__
#define __GPIO_H__

#include "common.h"

extern GPIO_MemMapPtr GPIOX[5] ; //�������ָ�����鱣�� GPIOX �ĵ�ַ
extern PORT_MemMapPtr PORTX[5] ;


//����ܽŷ���
typedef enum GPIO_CFG
{
    //�����ֵ���ܸģ�����
    GPI         = 0,                          //����ܽ����뷽��      GPIOx_PDDRn�0��ʾ���룬1��ʾ���
    GPO         = 1,                          //����ܽ��������

    GPI_DOWN    = 0x02,                       //��������              PORTx_PCRn��ҪPE=1��PS=0
    GPI_UP      = 0x03,                       //��������              PORTx_PCRn��ҪPE=1��PS=1
    GPI_PF      = 0x10,                       //���룬����Դ�˲���,�˲���Χ��10 MHz ~ 30 MHz ����֧�ָ��ٽӿڣ�>=2MHz��  0b10000           Passive Filter Enable
    GPI_DOWN_PF = GPI_DOWN | GPI_PF ,         //��������������Դ�˲���
    GPI_UP_PF   = GPI_UP   | GPI_PF ,         //��������������Դ�˲���

    GPO_HDS     = 0x41,                        //�������������   0b100 0001    High drive strength
    GPO_SSR     = 0x05,                        //������仯��          0b101     Slow slew rate
    GPO_HDS_SSR = GPO_HDS | GPO_SSR,           //������������������仯��
} GPIO_CFG;  //���λΪ0���϶������룻GPI_UP �� GPI_UP_PF�����λΪ1������Ϊ���



typedef enum exti_cfg
{
    rising_DMA        = 0x01,     //�����ش���DMA�ж�
    falling_DMA       = 0x02,     //�½��ش���DMA�ж�
    either_down_DMA   = 0x03,     //�����ش���DMA�ж�

    //�����λ��־����������
    zero          = 0x08,     //�͵�ƽ����
    rising        = 0x09,     //�����ش���
    falling       = 0x0A,     //�½��ش���
    either        = 0x0B,     //�����ش���
    one           = 0x0C,     //�ߵ�ƽ����
      
    zero_down     = 0x08u,     //�͵�ƽ�������ڲ�����
    rising_down   = 0x09u,     //�����ش������ڲ�����
    falling_down  = 0x0Au,     //�½��ش������ڲ�����
    either_down   = 0x0Bu,     //�����ش������ڲ�����
    one_down      = 0x0Cu,     //�ߵ�ƽ�������ڲ�����

    //�����λ��־����������
    zero_up       = 0x88u,     //�͵�ƽ�������ڲ�����
    rising_up     = 0x89u,     //�����ش������ڲ�����
    falling_up    = 0x8Au,     //�½��ش������ڲ�����
    either_up     = 0x8Bu,     //�����ش������ڲ�����
    one_up        = 0x8Cu      //�ߵ�ƽ�������ڲ�����
} exti_cfg;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************************************
                                                    GPIO���ú���
**************************************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*------------------------------------------------------------------------------------------------------
����    ����GPIO_Init
����    �ܡ���ʼ��GPIO ������GPIOģʽ
����    ����ptx_n �� Ҫ��ʼ����GPIO�� ��common.h�ж���
����    ����dir   �� GPIO��������ã� ������GPIO.h��
����    ����data  �� GPIOĬ��״̬  1���ߵ�ƽ  0���͵�ƽ
���� �� ֵ����
��ʵ    ����GPIO_Init(PTA17, GPO, 1); //����PTA17Ϊ���ģʽ ������Ϊ�ߵ�ƽ
��ע�����
--------------------------------------------------------------------------------------------------------*/
void GPIO_Pin_Init(PTXn_e ptx_n, GPIO_CFG dir, uint8_t data);



/*------------------------------------------------------------------------------------------------------
����    ����GPIO_PinWrite
����    �ܡ�����IO�����
����    ����ptx_n �� Ҫ��ʼ����GPIO�� ��common.h�ж���
����    ����data  �� GPIO���״̬  1���ߵ�ƽ  0���͵�ƽ
���� �� ֵ����
��ʵ    ����GPIO_PinWrite(PTA17, 1); //����PTA17����ߵ�ƽ
��ע�����ע��Ҫʹ��GPIO��ʼ������
--------------------------------------------------------------------------------------------------------*/
void GPIO_Pin_Write(PTXn_e ptx_n, uint8_t data);



/*------------------------------------------------------------------------------------------------------
����    ����GPIO_PinSetDir
����    �ܡ�����IO�����뻹�����
����    ����ptx_n �� Ҫ��ʼ����GPIO�� ��common.h�ж���
����    ����dir   �� GPIO����   1�����  0������
���� �� ֵ����
��ʵ    ����GPIO_PinSetDir(PTA17, 1); //����PTA17���
��ע�����ע��Ҫʹ��GPIO��ʼ������
--------------------------------------------------------------------------------------------------------*/
void GPIO_Pin_SetDir(PTXn_e ptx_n, uint8_t dir);


/*------------------------------------------------------------------------------------------------------
����    ����GPIO_PinReverse
����    �ܡ�GPIOȡ��
����    ����ptx_n �� Ҫ��ʼ����GPIO�� ��common.h�ж���
���� �� ֵ����
��ʵ    ����GPIO_PinReverse(PTA17); //ȡ��PTA17�ܽŵ�ƽ
��ע�����ע��Ҫʹ��GPIO��ʼ������
--------------------------------------------------------------------------------------------------------*/
void GPIO_Pin_Reverse(PTXn_e ptx_n);



/*------------------------------------------------------------------------------------------------------
����    ����GPIO_PinRead
����    �ܡ���ȡIO�ڵ�ƽ
����    ����ptx_n �� Ҫ��ʼ����GPIO�� ��common.h�ж���
���� �� ֵ��1�� �ߵ�ƽ   0���͵�ƽ
��ʵ    ����GPIO_PinRead(PTA17); //��ȡPTA17�ܽŵ�ƽ
��ע�����ע��Ҫʹ��GPIO��ʼ������
--------------------------------------------------------------------------------------------------------*/
uint8_t GPIO_Pin_Read(PTXn_e ptx_n);


/*------------------------------------------------------------------------------------------------------
����    ����GPIO_PinReverse
����    �ܡ�GPIOȡ��
����    ����ptx_n �� Ҫ��ʼ����GPIO�� ��common.h�ж���
���� �� ֵ����
��ʵ    ����GPIO_ExtiInit(PTA17, rising_down); //PTA17�ܽ������ش����ж�
��ע�����ע����Ҫʹ��NVIC_SetPriority������PIT�ж����ȼ�   NVIC_EnableIRQ��ʹ���ж� 
��ע�����
��ע��������ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж� 
��ע�����NVIC_SetPriority(PORTA_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
��ע�����NVIC_EnableIRQ(PORTA_IRQn);			         //ʹ��PORTA_IRQn���ж�  
--------------------------------------------------------------------------------------------------------*/
void GPIO_ExtiInit(PTXn_e ptx_n, exti_cfg cfg);


#endif