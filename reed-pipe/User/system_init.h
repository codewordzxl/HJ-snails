#ifndef __SYTSEM_INIT_H__
#define __SYSTEM_INIT_H__

typedef struct _Control
{
    //归一最大值
    int SPDG_max;//水平max
    int XDG_max;//斜max
    int ZZ_max;//中max

    //偏差
    int SPDG_error;
    int XDG_error;
    int ZZ_error;
    //舵机
    int Steer_center;//舵机中值 670
    int Steer_left;//舵机左打角最大值
    int Steer_right;//舵机右打角最大值
    int Steer_out;
    int Steer_variety;
    
    int start_time;
    int start_delay;
    
    int stop_flag;
    
    int speed;
    /****************************速度PID*************************************/
    float Error[4];     //误差数组
    
    float Speed_PX;      //比例 P
    float Speed_PS;      //比例 P
    
    float Speed_IX;      //积分 I
    float Speed_IS;      //积分 I
    
    float Speed_PU;
    float Speed_IU;
    
    float Speed_cal_I; //计算出的积分值
    float Pwm1_Out_Old ; //上一个速度输出
    int Pwm1_Out_New;  //这一个速度输出
    int Pwm1_Out;      //速度计算输出结果
    int Pid1_Out;      //PID输出
    
    int Pwm2_Out_Old ; //上一个速度输出
    int Pwm2_Out_New;  //这一个速度输出
    int Pwm2_Out;      //速度计算输出结果
    int Pid2_Out;      //PID输出
    
    int speed1_get;
    int speed2_get;
    
    int speed1_set;
    int speed2_set;
    /*********************************************************************************/
    
    /**************************************舵机PID************************************/
    float Duoji_P;
    float Duoji_RD;   //实时D
    float Erro;//误差数组
    /*********************************************************************************/
    
    //坡道
    int Ramp_up;//上坡
    int Ramp_down;//下坡
    int Ramp_less;//下坡控速
    
    //环岛
    int Round_left;//疑似左环岛
    int Round_right;//疑似右环岛
    int Round_delay;//环岛识别时间
    int Roundleft_keep;
    int Roundright_keep;
    int Roundlike_flag;
    int Roundright_flag;
    int Roundleft_flag;
    int RoundEnter;
    int RoundEnter_left;
    int RoundEnter_right;
    int Round_inside;
    int Round_less;
    
    //十字
    int Cross_flag;
    
    
    //丢线
    int Lostline_left;
    int Lostline_right;
    int Lostline_less;
    
    //偏离度
    int SPDG;
    int XDG;
    int Deviation;
    
    //直道判断
    int Straight_flag;
    int Straight_keep;
    int Straight_delay;
    int Straight_plus;//直道加速
    
    //弯道减速
    int Curve_less;
    
    
    //出库
    int OUT_time;//出库时间 OUT_delay1+OUT_delay2
    int OUT_time1;//前进时间
    int OUT_time2;//打角时间
    int OUT_speed;//出库速度
    
    //入库
    int Warehousing_flag;//入库标志
    int Reversing_speed;//倒车速度
    int Warehousing_speed;//入库速度
    int Warehousing_time;
    int Warehousing_time1;
    int Warehousing_time2;
    
}Control;

extern Control Controlvar;
/*****系统初始化************/
void system_init(void);

void data_init(void);//数据初始化

#endif
