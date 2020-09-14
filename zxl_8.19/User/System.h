#ifndef __SYSTEM_H__
#define __SYSTEM_H__

extern short enc1;//编码器1的值
extern short enc2;//编码器2的值

extern uint16_t g_ValueOfAD[5];//电感值
extern int Deviation[3];//偏差
extern int Level;//水平差比和

void ADC_Deal();//ADC读取

void Track();//赛道识别

void Steer();//舵机控制

void Spd_set();//速度设置(电机控制调用)

void Spd_out();//电机控制

#endif