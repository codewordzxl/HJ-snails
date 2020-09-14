#include "include.h"


/*-------------------------------------------------------------------------
*功能：计算速度增量式PID
*输入：设定速度 speed_set
*输出：PWM波占空比
*-------------------------------------------------------------------------------*/
void PID_left(float speed)
{
  float erroo[3] = {0,0,0};//误差数组
  float derro = 0.0f;
  
  float speed_p = 0.0f;//比例 P
  float speed_i = 0.0f;//积分 I
  float pwm_out_old = 0.0f;
  
  float fp = 0.0f;

  //------------变量保护-果然还是要用-----------------------------------//
  
  erroo[1] = Controlvar.Error[1];//前次
  pwm_out_old=Controlvar.Pwm1_Out_Old;
  //------------变量保护-结束-----------------------------------//  
  erroo[2]=speed-Controlvar.speed1_get;//偏差计算
  if(erroo[2]>=15)                //大范围加速用比较慢的pid 避免超调太多  start_pid_flag<=200||
  {
  speed_p = Controlvar.Speed_PS;
  speed_i = Controlvar.Speed_IS;
  }
  if(erroo[2]<15&&erroo[2]>=0)                //小范围加速
  {
  speed_p = Controlvar.Speed_PX;
  speed_i = Controlvar.Speed_IX;
  }
  if(erroo[2]<0)                                   //减速  用这个   钢的一逼
  {
    speed_p = Controlvar.Speed_PU;
    speed_i = Controlvar.Speed_IU;
  }

  
  Controlvar.Error[0]=erroo[1];
  Controlvar.Error[1]=erroo[2];       
  //--------积分计算-------------------------------------------//
  
  if((erroo[2]<OFFSE_I_MAX)&&(erroo[2]>OFFSE_I_MIN))    //积分范围  
  {
  Controlvar.Speed_cal_I=speed_i*erroo[2];
  //-------你觉得要写限幅你就写------------------------------//
  
  if(Controlvar.Speed_cal_I<SPEED_I_MIN) Controlvar.Speed_cal_I=SPEED_I_MIN;     //积分结果限幅
  
  if(Controlvar.Speed_cal_I>SPEED_I_MAX) Controlvar.Speed_cal_I=SPEED_I_MAX;
  if(Controlvar.speed1_get==0&&Controlvar.speed1_set<11) Controlvar.Speed_cal_I=0;           //积分清零
  
  
  }
  else  Controlvar.Speed_cal_I=0;//积分范围之外  不积分
  
  //----------------------------比例计算--------------------------------//
  derro=erroo[2]-erroo[1];           //两次偏差的差值
  fp=speed_p*derro;            //比例结果
  
  if(derro<OFFSE_P_MIN) {fp=speed_p*OFFSE_P_MIN;}   //两次偏差差值限幅
  
  if(derro>OFFSE_P_MAX) {fp=speed_p*OFFSE_P_MAX;}  
  
  //------------终于输出数值了-------------------------------------------------//
  Controlvar.Pid1_Out=(fp+Controlvar.Speed_cal_I);//pid计算输出   比例+积分
     //
  Controlvar.Pwm1_Out_New=pwm_out_old+Controlvar.Pid1_Out;//PWM输出   =上一次pwm输出+pid输出   平滑输出
  
  Controlvar.Pwm1_Out_Old=Controlvar.Pwm1_Out_New; 
  
  //-----------输出限幅--------------------------------------------------------// 
  Controlvar.Pwm1_Out = Controlvar.Pwm1_Out_New;
  if( Controlvar.Pwm1_Out<PWM_OUT_MIN) {Controlvar.Pwm1_Out = PWM_OUT_MIN; Controlvar.Pwm1_Out_New= PWM_OUT_MIN;}
  
  if( Controlvar.Pwm1_Out>PWM_OUT_MAX) {Controlvar.Pwm1_Out = PWM_OUT_MAX; Controlvar.Pwm1_Out_New= PWM_OUT_MAX;} 
}

void PID_right(float speed)
{
  float erroo[3] = {0,0,0};//误差数组
  float derro = 0.0f;
  
  float speed_p = 0.0f;//比例 P
  float speed_i = 0.0f;//积分 I
  float pwm_out_old = 0.0f;
  
  float fp = 0.0f;

  //------------变量保护-果然还是要用-----------------------------------//
  
  erroo[1] = Controlvar.Error[3];//前次
  pwm_out_old=Controlvar.Pwm2_Out_Old;
  //------------变量保护-结束-----------------------------------//  
  erroo[2]=speed-Controlvar.speed2_get;//偏差计算
  if(erroo[2]>=15)                //大范围加速用比较慢的pid 避免超调太多  start_pid_flag<=200||
  {
  speed_p = Controlvar.Speed_PS;
  speed_i = Controlvar.Speed_IS;
  }
  if(erroo[2]<15&&erroo[2]>=0)                //小范围加速
  {
  speed_p = Controlvar.Speed_PX;
  speed_i = Controlvar.Speed_IX;
  }
  if(erroo[2]<0)                                   //减速  用这个   钢的一逼
  {
    speed_p = Controlvar.Speed_PU;
    speed_i = Controlvar.Speed_IU;
  }
  
  Controlvar.Error[2]=erroo[1];
  Controlvar.Error[3]=erroo[2];       
  //--------积分计算-------------------------------------------//
  
  if((erroo[2]<OFFSE_I_MAX)&&(erroo[2]>OFFSE_I_MIN))    //积分范围  
  {
  Controlvar.Speed_cal_I=speed_i*erroo[2];
  //-------你觉得要写限幅你就写------------------------------//
  
  if(Controlvar.Speed_cal_I<SPEED_I_MIN) Controlvar.Speed_cal_I=SPEED_I_MIN;     //积分结果限幅
  
  if(Controlvar.Speed_cal_I>SPEED_I_MAX) Controlvar.Speed_cal_I=SPEED_I_MAX;
  if(Controlvar.speed2_get==0&&Controlvar.speed2_set<11) Controlvar.Speed_cal_I=0;           //积分清零
  
  
  }
  else  Controlvar.Speed_cal_I=0;//积分范围之外  不积分
  
  //----------------------------比例计算--------------------------------//
  derro=erroo[2]-erroo[1];           //两次偏差的差值
  fp=speed_p*derro;            //比例结果
  
  if(derro<OFFSE_P_MIN) {fp=speed_p*OFFSE_P_MIN;}   //两次偏差差值限幅
  
  if(derro>OFFSE_P_MAX) {fp=speed_p*OFFSE_P_MAX;}  
  
  //------------终于输出数值了-------------------------------------------------//
  Controlvar.Pid2_Out=(fp+Controlvar.Speed_cal_I);//pid计算输出   比例+积分
     //
  Controlvar.Pwm2_Out_New=pwm_out_old+Controlvar.Pid2_Out;//PWM输出   =上一次pwm输出+pid输出   平滑输出
  
  Controlvar.Pwm2_Out_Old=Controlvar.Pwm2_Out_New; 
  
  //-----------输出限幅--------------------------------------------------------// 
  Controlvar.Pwm2_Out = Controlvar.Pwm2_Out_New;
  if( Controlvar.Pwm2_Out<PWM_OUT_MIN) {Controlvar.Pwm2_Out = PWM_OUT_MIN; Controlvar.Pwm2_Out_New= PWM_OUT_MIN;}
  
  if( Controlvar.Pwm2_Out>PWM_OUT_MAX) {Controlvar.Pwm2_Out = PWM_OUT_MAX; Controlvar.Pwm2_Out_New= PWM_OUT_MAX;} 
}


//舵机
int Steer_PD(int offse)
{
    float duoji_p=0;      //舵机p
    float duoji_d=0;         //舵机D
    float erro=0;          //误差
    float erro_last=0;     //后一次的误差
    float fP=0;           //
    float fD=0;           //
    float PD_out=0;      //

    duoji_p=Controlvar.Duoji_P;   //舵机p
    duoji_d=Controlvar.Duoji_RD;     // 舵机实时D
    erro_last=Controlvar.Erro;      //
    erro=offse;

    Controlvar.Erro=erro;//储存误差
    fP=duoji_p*erro;          //P
    fD=duoji_d*(erro-erro_last);//D
    PD_out=fP+fD;       //输出舵机pd增减量
    
    if(PD_out>70){PD_out=70;}       //输出限幅
    if(PD_out<(-70)){PD_out=(-70);}    //输出限幅

    return (int)PD_out;
}
