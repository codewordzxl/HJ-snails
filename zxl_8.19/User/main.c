/*-----------------------------------------------------------------------------------------------------
��ƽ    ̨������KV58���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0������Դ���룬���ڹٷ�SDK�޸ģ������ο�������Ը�
�������¡�2019��04��02��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
������ƽ̨��IAR 8.2
����    �ܡ�
��ע�����
-------------------------------------------------------------*/
#include "include.h"

char txt[16];//OLED��ʾ��������
/******************��ʱ****************/
void delayms(uint16_t ms)
{
	volatile uint32_t i = 0;
	while(ms--)
	{
		for (i = 0; i < 20000; ++i)
		{
			__asm("NOP"); /* delay */
		}
	}	
}

int main(void)
{
    /* Init hardware */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    
    /* �����ж����ȼ���  0: 0����ռ���ȼ�16λ�������ȼ� 
     * 1: 2����ռ���ȼ� 8�������ȼ� 2: 4����ռ���ȼ� 4�������ȼ� 
     * 3: 8����ռ���ȼ� 2�������ȼ� 4: 16����ռ���ȼ� 0�������ȼ�
     */
    NVIC_SetPriorityGrouping(2);  
    
    System_Init();//��ʼ��
    Key_set();//���뿪�ؼ��
    
    while(Controlvar.See_Data)//�鿴����ģʽ
    {
        ADC_Deal();//ADC��ȡ��ƫ�����
        OLED_CLS();
        int i;
        for(i=0;i<5;i++)
        {
            sprintf(txt,"adc%d:%d",i,g_ValueOfAD[i]);
            OLED_P6x8Str(0,i,(uint8_t *)txt);
        }
        sprintf(txt,"SP:%d",Deviation[0]);
        OLED_P6x8Str(0,5,(uint8_t *)txt);
        sprintf(txt,"X:%d",Deviation[1]);
        OLED_P6x8Str(0,6,(uint8_t *)txt);
        sprintf(txt,"L:%d",Level);
        OLED_P6x8Str(0,7,(uint8_t *)txt);
        delayms(1000);
    }
    
    while(1){
    
    }
	
    return 0;
}
