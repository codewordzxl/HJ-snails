#ifndef __SYSTEM_H__
#define __SYSTEM_H__

extern short enc1;//������1��ֵ
extern short enc2;//������2��ֵ

extern uint16_t g_ValueOfAD[5];//���ֵ
extern int Deviation[3];//ƫ��
extern int Level;//ˮƽ��Ⱥ�

void ADC_Deal();//ADC��ȡ

void Track();//����ʶ��

void Steer();//�������

void Spd_set();//�ٶ�����(������Ƶ���)

void Spd_out();//�������

#endif