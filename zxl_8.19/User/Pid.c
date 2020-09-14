#include "include.h"


/*-------------------------------------------------------------------------
*���ܣ������ٶ�����ʽPID
*���룺�趨�ٶ� speed_set
*�����PWM��ռ�ձ�
*-------------------------------------------------------------------------------*/
void PID_left(float speed)
{
  float erroo[3] = {0,0,0};//�������
  float derro = 0.0f;
  
  float speed_p = 0.0f;//���� P
  float speed_i = 0.0f;//���� I
  float pwm_out_old = 0.0f;
  
  float fp = 0.0f;

  //------------��������-��Ȼ����Ҫ��-----------------------------------//
  
  erroo[1] = Controlvar.Error[1];//ǰ��
  pwm_out_old=Controlvar.Pwm1_Out_Old;
  //------------��������-����-----------------------------------//  
  erroo[2]=speed-Controlvar.speed1_get;//ƫ�����
  if(erroo[2]>=15)                //��Χ�����ñȽ�����pid ���ⳬ��̫��  start_pid_flag<=200||
  {
  speed_p = Controlvar.Speed_PS;
  speed_i = Controlvar.Speed_IS;
  }
  if(erroo[2]<15&&erroo[2]>=0)                //С��Χ����
  {
  speed_p = Controlvar.Speed_PX;
  speed_i = Controlvar.Speed_IX;
  }
  if(erroo[2]<0)                                   //����  �����   �ֵ�һ��
  {
    speed_p = Controlvar.Speed_PU;
    speed_i = Controlvar.Speed_IU;
  }

  
  Controlvar.Error[0]=erroo[1];
  Controlvar.Error[1]=erroo[2];       
  //--------���ּ���-------------------------------------------//
  
  if((erroo[2]<OFFSE_I_MAX)&&(erroo[2]>OFFSE_I_MIN))    //���ַ�Χ  
  {
  Controlvar.Speed_cal_I=speed_i*erroo[2];
  //-------�����Ҫд�޷����д------------------------------//
  
  if(Controlvar.Speed_cal_I<SPEED_I_MIN) Controlvar.Speed_cal_I=SPEED_I_MIN;     //���ֽ���޷�
  
  if(Controlvar.Speed_cal_I>SPEED_I_MAX) Controlvar.Speed_cal_I=SPEED_I_MAX;
  if(Controlvar.speed1_get==0&&Controlvar.speed1_set<11) Controlvar.Speed_cal_I=0;           //��������
  
  
  }
  else  Controlvar.Speed_cal_I=0;//���ַ�Χ֮��  ������
  
  //----------------------------��������--------------------------------//
  derro=erroo[2]-erroo[1];           //����ƫ��Ĳ�ֵ
  fp=speed_p*derro;            //�������
  
  if(derro<OFFSE_P_MIN) {fp=speed_p*OFFSE_P_MIN;}   //����ƫ���ֵ�޷�
  
  if(derro>OFFSE_P_MAX) {fp=speed_p*OFFSE_P_MAX;}  
  
  //------------���������ֵ��-------------------------------------------------//
  Controlvar.Pid1_Out=(fp+Controlvar.Speed_cal_I);//pid�������   ����+����
     //
  Controlvar.Pwm1_Out_New=pwm_out_old+Controlvar.Pid1_Out;//PWM���   =��һ��pwm���+pid���   ƽ�����
  
  Controlvar.Pwm1_Out_Old=Controlvar.Pwm1_Out_New; 
  
  //-----------����޷�--------------------------------------------------------// 
  Controlvar.Pwm1_Out = Controlvar.Pwm1_Out_New;
  if( Controlvar.Pwm1_Out<PWM_OUT_MIN) {Controlvar.Pwm1_Out = PWM_OUT_MIN; Controlvar.Pwm1_Out_New= PWM_OUT_MIN;}
  
  if( Controlvar.Pwm1_Out>PWM_OUT_MAX) {Controlvar.Pwm1_Out = PWM_OUT_MAX; Controlvar.Pwm1_Out_New= PWM_OUT_MAX;} 
}

void PID_right(float speed)
{
  float erroo[3] = {0,0,0};//�������
  float derro = 0.0f;
  
  float speed_p = 0.0f;//���� P
  float speed_i = 0.0f;//���� I
  float pwm_out_old = 0.0f;
  
  float fp = 0.0f;

  //------------��������-��Ȼ����Ҫ��-----------------------------------//
  
  erroo[1] = Controlvar.Error[3];//ǰ��
  pwm_out_old=Controlvar.Pwm2_Out_Old;
  //------------��������-����-----------------------------------//  
  erroo[2]=speed-Controlvar.speed2_get;//ƫ�����
  if(erroo[2]>=15)                //��Χ�����ñȽ�����pid ���ⳬ��̫��  start_pid_flag<=200||
  {
  speed_p = Controlvar.Speed_PS;
  speed_i = Controlvar.Speed_IS;
  }
  if(erroo[2]<15&&erroo[2]>=0)                //С��Χ����
  {
  speed_p = Controlvar.Speed_PX;
  speed_i = Controlvar.Speed_IX;
  }
  if(erroo[2]<0)                                   //����  �����   �ֵ�һ��
  {
    speed_p = Controlvar.Speed_PU;
    speed_i = Controlvar.Speed_IU;
  }
  
  Controlvar.Error[2]=erroo[1];
  Controlvar.Error[3]=erroo[2];       
  //--------���ּ���-------------------------------------------//
  
  if((erroo[2]<OFFSE_I_MAX)&&(erroo[2]>OFFSE_I_MIN))    //���ַ�Χ  
  {
  Controlvar.Speed_cal_I=speed_i*erroo[2];
  //-------�����Ҫд�޷����д------------------------------//
  
  if(Controlvar.Speed_cal_I<SPEED_I_MIN) Controlvar.Speed_cal_I=SPEED_I_MIN;     //���ֽ���޷�
  
  if(Controlvar.Speed_cal_I>SPEED_I_MAX) Controlvar.Speed_cal_I=SPEED_I_MAX;
  if(Controlvar.speed2_get==0&&Controlvar.speed2_set<11) Controlvar.Speed_cal_I=0;           //��������
  
  
  }
  else  Controlvar.Speed_cal_I=0;//���ַ�Χ֮��  ������
  
  //----------------------------��������--------------------------------//
  derro=erroo[2]-erroo[1];           //����ƫ��Ĳ�ֵ
  fp=speed_p*derro;            //�������
  
  if(derro<OFFSE_P_MIN) {fp=speed_p*OFFSE_P_MIN;}   //����ƫ���ֵ�޷�
  
  if(derro>OFFSE_P_MAX) {fp=speed_p*OFFSE_P_MAX;}  
  
  //------------���������ֵ��-------------------------------------------------//
  Controlvar.Pid2_Out=(fp+Controlvar.Speed_cal_I);//pid�������   ����+����
     //
  Controlvar.Pwm2_Out_New=pwm_out_old+Controlvar.Pid2_Out;//PWM���   =��һ��pwm���+pid���   ƽ�����
  
  Controlvar.Pwm2_Out_Old=Controlvar.Pwm2_Out_New; 
  
  //-----------����޷�--------------------------------------------------------// 
  Controlvar.Pwm2_Out = Controlvar.Pwm2_Out_New;
  if( Controlvar.Pwm2_Out<PWM_OUT_MIN) {Controlvar.Pwm2_Out = PWM_OUT_MIN; Controlvar.Pwm2_Out_New= PWM_OUT_MIN;}
  
  if( Controlvar.Pwm2_Out>PWM_OUT_MAX) {Controlvar.Pwm2_Out = PWM_OUT_MAX; Controlvar.Pwm2_Out_New= PWM_OUT_MAX;} 
}


//���
int Steer_PD(int offse)
{
    float duoji_p=0;      //���p
    float duoji_d=0;         //���D
    float erro=0;          //���
    float erro_last=0;     //��һ�ε����
    float fP=0;           //
    float fD=0;           //
    float PD_out=0;      //

    duoji_p=Controlvar.Duoji_P;   //���p
    duoji_d=Controlvar.Duoji_RD;     // ���ʵʱD
    erro_last=Controlvar.Erro;      //
    erro=offse;

    Controlvar.Erro=erro;//�������
    fP=duoji_p*erro;          //P
    fD=duoji_d*(erro-erro_last);//D
    PD_out=fP+fD;       //������pd������
    
    if(PD_out>70){PD_out=70;}       //����޷�
    if(PD_out<(-70)){PD_out=(-70);}    //����޷�

    return (int)PD_out;
}
