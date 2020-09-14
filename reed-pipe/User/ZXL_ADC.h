#ifndef __ZXL_ADC_H
#define __ZXL_ADC_H

extern char txt[16];//OLED显示储存数据
extern uint16_t g_ValueOfAD[5];//电感值
extern int Deviation[3];
extern int Deviation_d[3];

extern int Limit[5];//限幅
extern int Normalized[5];//归一化
extern int Bias[3];//偏差

extern int Level;
extern int Level_2;
extern int Level_3;


void ADC_get(void);//获取ADC值

void ADC_deal(void);//处理ADC值


#endif
