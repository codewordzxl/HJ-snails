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
    GPIO_Pin_Init(PTB0, GPI_UP, 1);
    GPIO_Pin_Init(PTB1, GPI_UP, 1);
    GPIO_Pin_Init(PTB2, GPI_UP, 1);
    GPIO_Pin_Init(PTB3, GPI_UP, 1);
    GPIO_Pin_Init(PTB4, GPI_UP, 1);
}

void KEY_TEST(void)
{
    LED_Init();
    KEY_Init();
    while(1){
        if(GPIO_Pin_Read(PTB0)==0){
            LED_ON(0);
        }
        else{
            LED_OFF(0);
        }
        if(GPIO_Pin_Read(PTB1)==0){
            LED_ON(1);
        }
        else{
            LED_OFF(1);
        }
        if(GPIO_Pin_Read(PTB2)==0){
            LED_ON(2);
        }
        else{
            LED_OFF(2);
        }
        if(GPIO_Pin_Read(PTB3)==0){
            LED_ON(3);
        }
        else{
            LED_OFF(3);
        }
    }
}
