#ifndef __ZXL_ADC_H
#define __ZXL_ADC_H

extern char txt[16];//OLED��ʾ��������
extern uint16_t g_ValueOfAD[5];//���ֵ
extern int Deviation[3];
extern int Deviation_d[3];

extern int Limit[5];//�޷�
extern int Normalized[5];//��һ��
extern int Bias[3];//ƫ��

extern int Level;
extern int Level_2;
extern int Level_3;


void ADC_get(void);//��ȡADCֵ

void ADC_deal(void);//����ADCֵ


#endif
