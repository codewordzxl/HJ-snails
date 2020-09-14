#include "include.h"

Control Controlvar;


void system_init(void)
{
    //���ݳ�ʼ��
    data_init();
    
    /************��ά������ʼ��*************/
    GPIO_Pin_Init(PTA24, GPI, 1);//A24
    GPIO_Pin_Init(PTA25, GPI, 1);//A25
    GPIO_Pin_Init(PTA26, GPI, 1);//A26
    GPIO_Pin_Init(PTA27, GPI, 1);//A27
    GPIO_Pin_Init(PTA28, GPI, 1);//A28
    
    /************���뿪�س�ʼ��*************/
    GPIO_Pin_Init(PTB0, GPI, 1);//B0
    GPIO_Pin_Init(PTB1, GPI, 1);//B1
    GPIO_Pin_Init(PTB2, GPI, 1);//B2
    GPIO_Pin_Init(PTB3, GPI, 1);//B3
    GPIO_Pin_Init(PTB4, GPI, 1);//B4
    GPIO_Pin_Init(PTB5, GPI, 1);//B5
    
    /************�ɻɹ�*********************/
    GPIO_Pin_Init(PTB6, GPI, 1);//B6
    GPIO_Pin_Init(PTB7, GPI, 1);//B7
    
    /************LED�Ƴ�ʼ��****************/
    GPIO_Pin_Init(PTA17, GPO, 1);//A17
    GPIO_Pin_Init(PTC0 , GPO, 1);//C0
    GPIO_Pin_Init(PTD15, GPO, 1);//D15
    GPIO_Pin_Init(PTE29, GPO, 1);//E29
    
    /************OLED��ʼ��*****************/
    OLED_Init();
    //���İ�C16��C17��C18��C19
    //���ذ�C13��C14��C15��C16
    
    /************������ʼ��*****************/
    UART4_Init();//E24��E25
    
    /************ADC��ʼ��******************/
    ADC_Init();
    //ADC�ӿ�
    /*
    *E4
    *E5
    *E6
    *E11
    *E12
    *E18
    *E19
    *E20
    */
    
    /************�����ʼ��*****************/
    FTM_PwmInit(FTM0, FTM_CH0, 16000, 0);//C1
    FTM_PwmInit(FTM0, FTM_CH1, 16000, 0);//C2
    FTM_PwmInit(FTM0, FTM_CH2, 16000, 0);//C3
    FTM_PwmInit(FTM0, FTM_CH3, 16000, 0);//C4
    
    /************��������ʼ��***************/
    FTM_ABInit(FTM1);//A12��A13
    FTM_ABInit(FTM2);//B18��B19
    
    /************�����ʼ��******************/
    FTM_PwmInit(FTM3, FTM_CH6, 50,Controlvar.Steer_center);//C10
    
    
    //PIT��ʱ����ʼ��
    PIT_Init(PIT0,5);
    //PIT_Init(PIT1, 20);

    /* ���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж� */
    NVIC_SetPriority(PIT0_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    //NVIC_SetPriority(PIT1_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0,2));
    
    //ʹ��PIT0_IRQn���ж�
    NVIC_EnableIRQ(PIT0_IRQn);			
    //NVIC_EnableIRQ(PIT1_IRQn);			          //ʹ��PIT1_IRQn���ж�
}

void data_init(void)
{
    //��һ����ֵ
    Controlvar.SPDG_max=1500;//ˮƽmax
    Controlvar.XDG_max=1500;//бmax
    Controlvar.ZZ_max=1400;//�м�max
    
    //���
    Controlvar.Steer_center=670;//�����ֵ
    Controlvar.Steer_left=750;
    Controlvar.Steer_right=590;
    
    /*********************************�ٶ�PID****************************************/
    Controlvar.Error[0]=0.0;    //�������1
    Controlvar.Error[1]=0.0;    //�������2
    Controlvar.Error[2]=0.0;    //�������3
    Controlvar.Error[3]=0.0;    //�������4
    Controlvar.Pwm1_Out_Old=0;    //��һ���ٶ����
    Controlvar.Pwm1_Out_New=0;    //��һ���ٶ����
    Controlvar.Pwm1_Out=0;        //���
    Controlvar.Pid1_Out=0;
    
    Controlvar.Pwm2_Out_Old=0;    //��һ���ٶ����
    Controlvar.Pwm2_Out_New=0;    //��һ���ٶ����
    Controlvar.Pwm2_Out=0;        //���
    Controlvar.Pid2_Out=0;
    
    Controlvar.Speed_PS=300;
    Controlvar.Speed_PX=400; 
    Controlvar.Speed_PU=550;
    Controlvar.Speed_IS=18;
    Controlvar.Speed_IX=40;
    Controlvar.Speed_IU=55;
    
    Controlvar.speed1_set=20;
    Controlvar.speed2_set=20;
    /*****************************************************************************/
    
    /************************************���PID**********************************/
    
    Controlvar.Duoji_P=1.f;
    Controlvar.Duoji_RD=9.f;
    Controlvar.Erro=0.f;
    
    /*****************************************************************************/
    Controlvar.speed=25;
    
    Controlvar.Ramp_up=0;
    Controlvar.Ramp_down=0;
    Controlvar.Ramp_less=0;
    
    
    Controlvar.start_time=0;
    Controlvar.start_delay=500;
    
    Controlvar.stop_flag=0;
    
    //����
    Controlvar.Round_left=0;
    Controlvar.Round_right=0;
    Controlvar.Round_delay=4;
    Controlvar.Roundleft_keep=0;
    Controlvar.Roundright_keep=0;
    Controlvar.Roundlike_flag=0;
    Controlvar.Roundright_flag=0;
    Controlvar.Roundleft_flag=0;
    Controlvar.RoundEnter=0;
    Controlvar.RoundEnter_left=0;
    Controlvar.RoundEnter_right=0;
    Controlvar.Round_inside=0;
    Controlvar.Round_less=3;
    
    Controlvar.Lostline_left=0;
    Controlvar.Lostline_right=0;
    Controlvar.Lostline_less=2;
    
    //ֱ��
    Controlvar.Straight_keep=0;
    Controlvar.Straight_flag=0;
    Controlvar.Straight_delay=20;
    Controlvar.Straight_plus=0;
    
    Controlvar.Curve_less=0;
    
    //ʮ��
    Controlvar.Cross_flag=0;
    
    //����
    Controlvar.OUT_time=0;
    Controlvar.OUT_time1=80;
    Controlvar.OUT_time2=80;
    Controlvar.OUT_speed=5;
    
    //���
    Controlvar.Warehousing_flag=0;//����־
    Controlvar.Reversing_speed=-5;//�����ٶ�
    Controlvar.Warehousing_speed=10;//����ٶ�
    Controlvar.Warehousing_time=0;
    Controlvar.Warehousing_time1=250;
    Controlvar.Warehousing_time2=10;
}
