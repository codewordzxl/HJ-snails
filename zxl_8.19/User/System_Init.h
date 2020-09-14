#ifndef __SYSTEM_INIT_H__
#define __SYSTEM_INIT_H__


typedef struct _Control
{
    //ƫ��
    int Level;
    int Deviation;
    //���
    int Steer_center;//�����ֵ 670
    int Steer_Left;//������޷�
    int Steer_Right;//������޷�
    int Steer_out;
    int Steer_variety;
    
    //�鿴����ģʽ
    int See_Data;
    
    //��ʱ����
    int Start_time;
    int Start_count;
    int Start_flag;
    
    //����
    int Out_time1;
    int Out_time2;
    int Out_count;
    int Out_flag;
    int Out_speed;
    int Out_Direction;//����
    
    //�����߼��
    int StartLine_time;//������StartLine_time*5ms��ʼ���������
    int StartLine_count;
    int StartLine_flag;//StartLine_flag=1  ��ʼ���������
    
    //��⣨ͣ����
    int Stop_flag;
    int Stop_time1;
    int Stop_time2;
    int Stop_count;
    int Stop_speed;
    int Stop_cs;//����
      
    //�������ͣ��
    int Leave_flag;
    int Leave_time;
    int Leave_count;
    
    //�ٶ�
    int speed;
    /****************************�ٶ�PID*************************************/
    float Error[4];     //�������
    
    float Speed_PX;      //���� P
    float Speed_PS;      //���� P
    
    float Speed_IX;      //���� I
    float Speed_IS;      //���� I
    
    float Speed_PU;
    float Speed_IU;
    
    float Speed_cal_I; //������Ļ���ֵ
    float Pwm1_Out_Old ; //��һ���ٶ����
    float Pwm1_Out_New;  //��һ���ٶ����
    float Pwm1_Out;      //�ٶȼ���������
    float Pid1_Out;      //PID���
    
    float Pwm2_Out_Old ; //��һ���ٶ����
    float Pwm2_Out_New;  //��һ���ٶ����
    float Pwm2_Out;      //�ٶȼ���������
    float Pid2_Out;      //PID���
    
    float speed1_get;
    float speed2_get;
    
    float speed1_set;
    float speed2_set;
    /*********************************************************************************/
    
    /**************************************���PID************************************/
    float Duoji_P;
    float Duoji_RD;   //ʵʱD
    float Erro;//�������
    /*********************************************************************************/
    //ֱ��
    int Straight_flag;
    int Straight_keep;
    int Straight_delay;
    int Straight_plus;
    
    //���
    int Cruve_less;//�������
    float Cruve_Dif;//�������ϵ��
    
    //�µ�
    int Ramp_flag;
 
    //����
    int Round_left;//�����󻷵�
    int Round_right;//�����һ���
    int Round_delay;//���ƻ�������ʱ�����ֵ
    int Roundleft_keep;//�����󻷵�����ʱ��
    int Roundright_keep;//�����һ�������ʱ��
    int Roundleft_flag;//�󻷵���־
    int Roundright_flag;//�һ�����־
    int RoundEnter;
    int RoundEnter_left;
    int RoundEnter_right;
    int Round_speed;
    int Round_Dif;//��������
    int Round_coeff;//ƫ��ϵ��
    int Round_QD;//����ֵ
    int Round_FD;//
    
    //����
    int Lostline_left;
    int Lostline_right; 
    //���߲���
    int Lostline_Threshold;//������ֵ
    int Lostline_Restore;//���߻ָ���ֵ
    int Lostline_Dif;//���߲���
    
    //���PID����
    float Straight_P;//ֱ��P
    float Straight_D;//ֱ��D
    float Curve_P;//���P
    float Curve_D;//���D
    float Round_P;//����P
    float Round_D;//����D

}Control;

extern Control Controlvar;

void System_Init();//�����ʼ��

void Data_Init();//���ݳ�ʼ��

void Key_set();//���뿪������

#endif