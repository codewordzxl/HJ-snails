#ifndef __PID_H__
#define __PID_H__

#define OFFSE_I_MAX  80.0f//����ƫ��ķ�ΧMAX �ǵ��Լ��޸�ֵ   
#define OFFSE_I_MIN -80.0f//�������Χ�Ż��� ��������ֻᳬ��
#define OFFSE_P_MAX  100.0f//����ƫ��ķ�ΧMAX �ǵ��Լ��޸�ֵ
#define OFFSE_P_MIN -100.0f

#define SPEED_I_MAX 500.0f           //���ֽ���޷�
#define SPEED_I_MIN -500.0f
#define SPEED_P_MAX 500.0f           //��������޷�
#define SPEED_P_MIN -500.0f

#define PWM_OUT_MAX 10000.0f//�ٶ������Χ1000    -500
#define PWM_OUT_MIN -10000.0f


void PID_left(int speed);

void PID_right(int speed);

void speed_output(void);

int Steer_PD(int offse);

extern short enc1;//������1��ֵ
extern short enc2;//������2��ֵ
#endif
