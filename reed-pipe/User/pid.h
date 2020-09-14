#ifndef __PID_H__
#define __PID_H__

#define OFFSE_I_MAX  80.0f//积分偏差的范围MAX 记得自己修改值   
#define OFFSE_I_MIN -80.0f//在这个范围才积分 ，否则积分会超调
#define OFFSE_P_MAX  100.0f//比例偏差的范围MAX 记得自己修改值
#define OFFSE_P_MIN -100.0f

#define SPEED_I_MAX 500.0f           //积分结果限幅
#define SPEED_I_MIN -500.0f
#define SPEED_P_MAX 500.0f           //比例结果限幅
#define SPEED_P_MIN -500.0f

#define PWM_OUT_MAX 10000.0f//速度输出范围1000    -500
#define PWM_OUT_MIN -10000.0f


void PID_left(int speed);

void PID_right(int speed);

void speed_output(void);

int Steer_PD(int offse);

extern short enc1;//编码器1的值
extern short enc2;//编码器2的值
#endif
