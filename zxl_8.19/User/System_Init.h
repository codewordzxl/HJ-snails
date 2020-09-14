#ifndef __SYSTEM_INIT_H__
#define __SYSTEM_INIT_H__


typedef struct _Control
{
    //偏差
    int Level;
    int Deviation;
    //舵机
    int Steer_center;//舵机中值 670
    int Steer_Left;//舵机左限幅
    int Steer_Right;//舵机右限幅
    int Steer_out;
    int Steer_variety;
    
    //查看数据模式
    int See_Data;
    
    //延时发车
    int Start_time;
    int Start_count;
    int Start_flag;
    
    //出库
    int Out_time1;
    int Out_time2;
    int Out_count;
    int Out_flag;
    int Out_speed;
    int Out_Direction;//方向
    
    //起跑线检测
    int StartLine_time;//发车后StartLine_time*5ms开始检测起跑线
    int StartLine_count;
    int StartLine_flag;//StartLine_flag=1  开始检测起跑线
    
    //入库（停车）
    int Stop_flag;
    int Stop_time1;
    int Stop_time2;
    int Stop_count;
    int Stop_speed;
    int Stop_cs;//差速
      
    //冲出赛道停车
    int Leave_flag;
    int Leave_time;
    int Leave_count;
    
    //速度
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
    float Pwm1_Out_New;  //这一个速度输出
    float Pwm1_Out;      //速度计算输出结果
    float Pid1_Out;      //PID输出
    
    float Pwm2_Out_Old ; //上一个速度输出
    float Pwm2_Out_New;  //这一个速度输出
    float Pwm2_Out;      //速度计算输出结果
    float Pid2_Out;      //PID输出
    
    float speed1_get;
    float speed2_get;
    
    float speed1_set;
    float speed2_set;
    /*********************************************************************************/
    
    /**************************************舵机PID************************************/
    float Duoji_P;
    float Duoji_RD;   //实时D
    float Erro;//误差数组
    /*********************************************************************************/
    //直道
    int Straight_flag;
    int Straight_keep;
    int Straight_delay;
    int Straight_plus;
    
    //弯道
    int Cruve_less;//弯道减速
    float Cruve_Dif;//弯道差速系数
    
    //坡道
    int Ramp_flag;
 
    //环岛
    int Round_left;//疑似左环岛
    int Round_right;//疑似右环岛
    int Round_delay;//疑似环岛保持时间的阈值
    int Roundleft_keep;//疑似左环岛持续时间
    int Roundright_keep;//疑似右环岛持续时间
    int Roundleft_flag;//左环岛标志
    int Roundright_flag;//右环岛标志
    int RoundEnter;
    int RoundEnter_left;
    int RoundEnter_right;
    int Round_speed;
    int Round_Dif;//进环差速
    int Round_coeff;//偏差系数
    int Round_QD;//回正值
    int Round_FD;//
    
    //丢线
    int Lostline_left;
    int Lostline_right; 
    //丢线参数
    int Lostline_Threshold;//丢线阈值
    int Lostline_Restore;//丢线恢复阈值
    int Lostline_Dif;//丢线差速
    
    //舵机PID参数
    float Straight_P;//直道P
    float Straight_D;//直道D
    float Curve_P;//弯道P
    float Curve_D;//弯道D
    float Round_P;//环岛P
    float Round_D;//环岛D

}Control;

extern Control Controlvar;

void System_Init();//程序初始化

void Data_Init();//数据初始化

void Key_set();//拨码开关设置

#endif