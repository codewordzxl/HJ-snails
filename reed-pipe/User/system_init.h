#ifndef __SYTSEM_INIT_H__
#define __SYSTEM_INIT_H__

typedef struct _Control
{
    //��һ���ֵ
    int SPDG_max;//ˮƽmax
    int XDG_max;//бmax
    int ZZ_max;//��max

    //ƫ��
    int SPDG_error;
    int XDG_error;
    int ZZ_error;
    //���
    int Steer_center;//�����ֵ 670
    int Steer_left;//����������ֵ
    int Steer_right;//����Ҵ�����ֵ
    int Steer_out;
    int Steer_variety;
    
    int start_time;
    int start_delay;
    
    int stop_flag;
    
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
    int Pwm1_Out_New;  //��һ���ٶ����
    int Pwm1_Out;      //�ٶȼ���������
    int Pid1_Out;      //PID���
    
    int Pwm2_Out_Old ; //��һ���ٶ����
    int Pwm2_Out_New;  //��һ���ٶ����
    int Pwm2_Out;      //�ٶȼ���������
    int Pid2_Out;      //PID���
    
    int speed1_get;
    int speed2_get;
    
    int speed1_set;
    int speed2_set;
    /*********************************************************************************/
    
    /**************************************���PID************************************/
    float Duoji_P;
    float Duoji_RD;   //ʵʱD
    float Erro;//�������
    /*********************************************************************************/
    
    //�µ�
    int Ramp_up;//����
    int Ramp_down;//����
    int Ramp_less;//���¿���
    
    //����
    int Round_left;//�����󻷵�
    int Round_right;//�����һ���
    int Round_delay;//����ʶ��ʱ��
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
    
    //ʮ��
    int Cross_flag;
    
    
    //����
    int Lostline_left;
    int Lostline_right;
    int Lostline_less;
    
    //ƫ���
    int SPDG;
    int XDG;
    int Deviation;
    
    //ֱ���ж�
    int Straight_flag;
    int Straight_keep;
    int Straight_delay;
    int Straight_plus;//ֱ������
    
    //�������
    int Curve_less;
    
    
    //����
    int OUT_time;//����ʱ�� OUT_delay1+OUT_delay2
    int OUT_time1;//ǰ��ʱ��
    int OUT_time2;//���ʱ��
    int OUT_speed;//�����ٶ�
    
    //���
    int Warehousing_flag;//����־
    int Reversing_speed;//�����ٶ�
    int Warehousing_speed;//����ٶ�
    int Warehousing_time;
    int Warehousing_time1;
    int Warehousing_time2;
    
}Control;

extern Control Controlvar;
/*****ϵͳ��ʼ��************/
void system_init(void);

void data_init(void);//���ݳ�ʼ��

#endif
