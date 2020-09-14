#include "include.h"

Control Controlvar;

void System_Init()
{
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
    FTM_PwmInit(FTM0, FTM_CH0, 13000, 0);//C1
    FTM_PwmInit(FTM0, FTM_CH1, 13000, 0);//C2
    FTM_PwmInit(FTM0, FTM_CH2, 13000, 0);//C3
    FTM_PwmInit(FTM0, FTM_CH3, 13000, 0);//C4
    
    /************��������ʼ��***************/
    FTM_ABInit(FTM1);//A12��A13
    FTM_ABInit(FTM2);//B18��B19
    
    /************�����ʼ��******************/
    FTM_PwmInit(FTM3, FTM_CH6, 50,Controlvar.Steer_center);//C10
    
    
    //���ݳ�ʼ��
    Data_Init();
    //����ͷ��ʼ��
    LQMT9V034_Init();
    //PIT��ʱ����ʼ��
    PIT_Init(PIT0,5);

    /* ���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж� */
    NVIC_SetPriority(PIT0_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    			
}

void Data_Init()
{
    Controlvar.Steer_center=670;//�����ֵ
    Controlvar.Steer_Left=750;//������޷�
    Controlvar.Steer_Right=590;//������޷�
    
    //�鿴����ģʽ
    Controlvar.See_Data=0;//0������ʻģʽ    1�鿴����ģʽ���ɲ��뿪��5����  δ����0  ����1��
    
    //��ʱ����
    Controlvar.Start_time=100;
    Controlvar.Start_count=0;
    Controlvar.Start_flag=0;
    
    //����
    Controlvar.Out_time1=30;
    Controlvar.Out_time2=80;
    Controlvar.Out_count=0;
    Controlvar.Out_flag=0;
    Controlvar.Out_speed=20;//�����ٶ�
    Controlvar.Out_Direction=1;//����ⷽ��   0��   1��   ��Ĭ���ҳ���⣩
    
    //�����߼��
    Controlvar.StartLine_time=1000;//������ StartLine_time * 5ms ��ʼ���������
    Controlvar.StartLine_count=0;
    Controlvar.StartLine_flag=0;
    
    //��⣨ͣ����
    Controlvar.Stop_flag=0;
    Controlvar.Stop_time1=120;
    Controlvar.Stop_time2=10;
    Controlvar.Stop_count=0;
    Controlvar.Stop_speed=9;//����ٶ�
    Controlvar.Stop_cs=10;
    
    //�ٶ�
    Controlvar.speed=25;//Ĭ���ٶ�(�����ٶ�20)
    
    //�������ͣ��
    Controlvar.Leave_flag=0;
    Controlvar.Leave_time=400;
    Controlvar.Leave_count=0;
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
    Controlvar.Speed_PU=750;
    Controlvar.Speed_IS=18;
    Controlvar.Speed_IX=40;
    Controlvar.Speed_IU=75;
    
    Controlvar.speed1_set=0;
    Controlvar.speed2_set=0;
    /*****************************************************************************/
    
    /************************************���PID**********************************/
    
    Controlvar.Duoji_P=1.f;
    Controlvar.Duoji_RD=9.f;
    Controlvar.Erro=0.f;
    
    /*****************************************************************************/
    
    //ֱ��
    Controlvar.Straight_flag=0;
    Controlvar.Straight_keep=0;
    Controlvar.Straight_delay=20;
    Controlvar.Straight_plus=0;//����
    
    //�������
    Controlvar.Cruve_less=0;
    Controlvar.Cruve_Dif=0.05;
    //�µ�
    Controlvar.Ramp_flag=0;
    
    //����
    Controlvar.Round_left=0;
    Controlvar.Round_right=0;
    Controlvar.Round_delay=8;
    Controlvar.Roundleft_keep=0;
    Controlvar.Roundright_keep=0;
    Controlvar.Roundleft_flag=0;//�󻷵���־
    Controlvar.Roundright_flag=0;//�һ�����־
    Controlvar.RoundEnter=0;
    Controlvar.RoundEnter_left=0;
    Controlvar.RoundEnter_right=0;
    Controlvar.Round_speed=20;//���������ٶ�
    Controlvar.Round_Dif=5;
    Controlvar.Round_coeff=4;
    Controlvar.Round_QD=20;//����ֵ
    Controlvar.Round_FD=10;
    
    //����
    Controlvar.Lostline_left=0;//��ת���߱�־
    Controlvar.Lostline_right=0;//��ת���߱�־
    //���߲���
    Controlvar.Lostline_Threshold=400;//������ֵ
    Controlvar.Lostline_Restore=600;//���߻ָ���ֵ
    Controlvar.Lostline_Dif=0;
    
    //���PID����
    Controlvar.Straight_P=0.8f;//ֱ��P
    Controlvar.Straight_D=9.f;//ֱ��D
    Controlvar.Curve_P=1.5f;//���P
    Controlvar.Curve_D=18.f;//���D
    Controlvar.Round_P=1.5f;//����P
    Controlvar.Round_D=10.f;//����D
}

void Key_set()
{
    FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_center);
    
/***********************************************************************�ٶȵ�********************************************************************************/
    /********************�ٶ�1**31+0**************/
    if(GPIO_Pin_Read(PTB0)==0&&GPIO_Pin_Read(PTB1)==1&&GPIO_Pin_Read(PTB2)==1&&GPIO_Pin_Read(PTB3)==1)
    {
        Controlvar.speed=31;
        
        //���PID����
        Controlvar.Straight_P=0.8f;//ֱ��P
        Controlvar.Straight_D=14.f;//ֱ��D
        Controlvar.Curve_P=1.1f;//���P
        Controlvar.Curve_D=15.f;//���D
        Controlvar.Round_P=2.f;//����P
        Controlvar.Round_D=10.f;//����D
        
        Controlvar.Round_QD=40;//����ֵ
        Controlvar.Round_FD=10;
        
        Controlvar.Round_speed=25;
        //���
        Controlvar.Cruve_less=0;
        Controlvar.Cruve_Dif=0.05;
        
        Controlvar.Straight_plus=0;
        //����
        Controlvar.Out_time1=30;
        Controlvar.Out_time2=80;
        
        //���
        Controlvar.Stop_time1=120;//���ʱ��
        Controlvar.Stop_time2=10;//ֱ��ʱ��
        Controlvar.Stop_speed=10;//����ٶ�
        Controlvar.Stop_cs=10;
        
        Controlvar.Round_coeff=6;//����ƫ��ϵ��
        
        Controlvar.Lostline_Threshold=500;//������ֵ
        Controlvar.Lostline_Restore=600;//���߻ָ���ֵ
        Controlvar.Lostline_Dif=7;
        
        //����PI
        Controlvar.Speed_PU=650;
        Controlvar.Speed_IU=65;
    }
    
    /******************�ٶ�2****31+2*************/
    if(GPIO_Pin_Read(PTB0)==1&&GPIO_Pin_Read(PTB1)==0&&GPIO_Pin_Read(PTB2)==1&&GPIO_Pin_Read(PTB3)==1)
    {
        Controlvar.speed=31;
        
        //���PID����
        Controlvar.Straight_P=0.8f;//ֱ��P
        Controlvar.Straight_D=14.f;//ֱ��D
        Controlvar.Curve_P=1.1f;//���P
        Controlvar.Curve_D=15.f;//���D
        Controlvar.Round_P=2.f;//����P
        Controlvar.Round_D=10.f;//����D
        
        Controlvar.Round_QD=40;//����ֵ
        Controlvar.Round_FD=10;
        
        Controlvar.Round_speed=25;
        //���
        Controlvar.Cruve_less=0;
        Controlvar.Cruve_Dif=0.05;
        
        Controlvar.Straight_plus=2;
        //����
        Controlvar.Out_time1=30;
        Controlvar.Out_time2=80;
        
        //���
        Controlvar.Stop_time1=120;//���ʱ��
        Controlvar.Stop_time2=10;//ֱ��ʱ��
        Controlvar.Stop_speed=10;//����ٶ�
        Controlvar.Stop_cs=10;
        
        Controlvar.Round_coeff=6;//����ƫ��ϵ��
        
        Controlvar.Lostline_Threshold=500;//������ֵ
        Controlvar.Lostline_Restore=600;//���߻ָ���ֵ
        Controlvar.Lostline_Dif=7;
        
        //����PI
        Controlvar.Speed_PU=650;
        Controlvar.Speed_IU=65;
    }
    
    /****************�ٶ�3*****33+0****************/
    if(GPIO_Pin_Read(PTB0)==0&&GPIO_Pin_Read(PTB1)==0&&GPIO_Pin_Read(PTB2)==1&&GPIO_Pin_Read(PTB3)==1)
    {
        Controlvar.speed=33;
        
        //���PID����
        Controlvar.Straight_P=0.8f;//ֱ��P
        Controlvar.Straight_D=14.f;//ֱ��D
        Controlvar.Curve_P=1.2f;//���P
        Controlvar.Curve_D=15.f;//���D
        Controlvar.Round_P=3.f;//����P
        Controlvar.Round_D=10.f;//����D
        
        Controlvar.Round_QD=40;//����ֵ
        Controlvar.Round_FD=10;
        
        Controlvar.Round_speed=28;
        //���
        Controlvar.Cruve_less=0;
        Controlvar.Cruve_Dif=0.05;
        
        Controlvar.Straight_plus=0;
        //����
        Controlvar.Out_time1=30;
        Controlvar.Out_time2=80;
        
        //���
        Controlvar.Stop_time1=120;//���ʱ��
        Controlvar.Stop_time2=10;//ֱ��ʱ��
        Controlvar.Stop_speed=10;//����ٶ�
        Controlvar.Stop_cs=10;
        
        Controlvar.Round_coeff=6;//����ƫ��ϵ��
        
        Controlvar.Lostline_Threshold=500;//������ֵ
        Controlvar.Lostline_Restore=600;//���߻ָ���ֵ
        Controlvar.Lostline_Dif=7;
        
        //����PI
        Controlvar.Speed_PU=650;
        Controlvar.Speed_IU=65;
    }
    
    /******************�ٶ�4***33+2********************/
    if(GPIO_Pin_Read(PTB0)==1&&GPIO_Pin_Read(PTB1)==1&&GPIO_Pin_Read(PTB2)==0&&GPIO_Pin_Read(PTB3)==1)
    {
        Controlvar.speed=33;
        
        //���PID����
        Controlvar.Straight_P=0.8f;//ֱ��P
        Controlvar.Straight_D=14.f;//ֱ��D
        Controlvar.Curve_P=1.2f;//���P
        Controlvar.Curve_D=15.f;//���D
        Controlvar.Round_P=3.f;//����P
        Controlvar.Round_D=10.f;//����D
        
        Controlvar.Round_QD=40;//����ֵ
        Controlvar.Round_FD=10;
        
        Controlvar.Round_speed=28;
        //���
        Controlvar.Cruve_less=0;
        Controlvar.Cruve_Dif=0.05;
        
        Controlvar.Straight_plus=2;
        //����
        Controlvar.Out_time1=30;
        Controlvar.Out_time2=80;
        
        //���
        Controlvar.Stop_time1=120;//���ʱ��
        Controlvar.Stop_time2=10;//ֱ��ʱ��
        Controlvar.Stop_speed=10;//����ٶ�
        Controlvar.Stop_cs=10;
        
        Controlvar.Round_coeff=6;//����ƫ��ϵ��
        
        Controlvar.Lostline_Threshold=500;//������ֵ
        Controlvar.Lostline_Restore=600;//���߻ָ���ֵ
        Controlvar.Lostline_Dif=8;
        
        //����PI
        Controlvar.Speed_PU=650;
        Controlvar.Speed_IU=65;
    }
    
    /**********************�ٶ�5*******************/
    if(GPIO_Pin_Read(PTB0)==0&&GPIO_Pin_Read(PTB1)==1&&GPIO_Pin_Read(PTB2)==0&&GPIO_Pin_Read(PTB3)==1)
    {
        Controlvar.speed=35;
        
        //���PID����
        Controlvar.Straight_P=0.8f;//ֱ��P
        Controlvar.Straight_D=14.f;//ֱ��D
        Controlvar.Curve_P=1.3f;//���P
        Controlvar.Curve_D=14.f;//���D
        Controlvar.Round_P=4.f;//����P
        Controlvar.Round_D=10.f;//����D
        
        Controlvar.Round_QD=40;//����ֵ
        Controlvar.Round_FD=10;
        
        Controlvar.Round_speed=28;
        //���
        Controlvar.Cruve_less=0;
        Controlvar.Cruve_Dif=0.06f;
        
        Controlvar.Straight_plus=0;
        //����
        Controlvar.Out_time1=30;
        Controlvar.Out_time2=80;
        
        //���
        Controlvar.Stop_time1=140;//���ʱ��
        Controlvar.Stop_time2=10;//ֱ��ʱ��
        Controlvar.Stop_speed=9;//����ٶ�
        Controlvar.Stop_cs=10;
        
        Controlvar.Round_coeff=6;//����ƫ��ϵ��
        
        Controlvar.Lostline_Threshold=500;//������ֵ
        Controlvar.Lostline_Restore=600;//���߻ָ���ֵ
        
        Controlvar.Lostline_Dif=8;
        
        //����PI
        Controlvar.Speed_PU=750;
        Controlvar.Speed_IU=75;
    }
    
    /******************�ٶ�6***********************/
    if(GPIO_Pin_Read(PTB0)==1&&GPIO_Pin_Read(PTB1)==0&&GPIO_Pin_Read(PTB2)==0&&GPIO_Pin_Read(PTB3)==1)
    {
        Controlvar.speed=37;
        
        //���PID����
        Controlvar.Straight_P=0.8f;//ֱ��P
        Controlvar.Straight_D=18.f;//ֱ��D
        Controlvar.Curve_P=1.3f;//���P
        Controlvar.Curve_D=18.f;//���D
        Controlvar.Round_P=5.f;//����P
        Controlvar.Round_D=10.f;//����D
        
        Controlvar.Round_QD=40;//����ֵ
        Controlvar.Round_FD=10;
        
        Controlvar.Round_speed=30;
        //���
        Controlvar.Cruve_less=0;
        Controlvar.Cruve_Dif=0.08f;
        
        Controlvar.Straight_plus=0;
        //����
        Controlvar.Out_time1=30;
        Controlvar.Out_time2=80;
        
        //���
        Controlvar.Stop_time1=140;//���ʱ��
        Controlvar.Stop_time2=10;//ֱ��ʱ��
        Controlvar.Stop_speed=9;//����ٶ�
        Controlvar.Stop_cs=10;
        
        Controlvar.Round_coeff=6;//����ƫ��ϵ��
        
        Controlvar.Lostline_Threshold=500;//������ֵ
        Controlvar.Lostline_Restore=600;//���߻ָ���ֵ
        
        Controlvar.Lostline_Dif=8;
        
        //����PI
        Controlvar.Speed_PU=750;
        Controlvar.Speed_IU=75;
    }
    
/***�ٶȵ�***************************************************************************************************************************************************/
    

    /*******************����5**********************/
    //OLED��ʾADCֵ��ƫ��
    if(GPIO_Pin_Read(PTB4)==0)
    {
        Controlvar.See_Data=1;//�鿴����ģʽ
    }
    else
    {
        NVIC_EnableIRQ(PORTD_IRQn);			     //ʹ��PORTD_IRQn���ж� 
        
        NVIC_EnableIRQ(PIT0_IRQn);                              //ʹ��PIT0_IRQn���ж�
    }
    /*******************����6**********************/
    //���Ƴ���ⷽ��
    if(GPIO_Pin_Read(PTB5)==0)
    {
        Controlvar.Out_Direction=0;//���� ������
    }
    else
    {
        Controlvar.Out_Direction=1;//δ���� �ҳ����
    }
}