#include "include.h"

Control Controlvar;


void system_init(void)
{
    //数据初始化
    data_init();
    
    /************五维按键初始化*************/
    GPIO_Pin_Init(PTA24, GPI, 1);//A24
    GPIO_Pin_Init(PTA25, GPI, 1);//A25
    GPIO_Pin_Init(PTA26, GPI, 1);//A26
    GPIO_Pin_Init(PTA27, GPI, 1);//A27
    GPIO_Pin_Init(PTA28, GPI, 1);//A28
    
    /************拨码开关初始化*************/
    GPIO_Pin_Init(PTB0, GPI, 1);//B0
    GPIO_Pin_Init(PTB1, GPI, 1);//B1
    GPIO_Pin_Init(PTB2, GPI, 1);//B2
    GPIO_Pin_Init(PTB3, GPI, 1);//B3
    GPIO_Pin_Init(PTB4, GPI, 1);//B4
    GPIO_Pin_Init(PTB5, GPI, 1);//B5
    
    /************干簧管*********************/
    GPIO_Pin_Init(PTB6, GPI, 1);//B6
    GPIO_Pin_Init(PTB7, GPI, 1);//B7
    
    /************LED灯初始化****************/
    GPIO_Pin_Init(PTA17, GPO, 1);//A17
    GPIO_Pin_Init(PTC0 , GPO, 1);//C0
    GPIO_Pin_Init(PTD15, GPO, 1);//D15
    GPIO_Pin_Init(PTE29, GPO, 1);//E29
    
    /************OLED初始化*****************/
    OLED_Init();
    //核心板C16、C17、C18、C19
    //主控板C13、C14、C15、C16
    
    /************蓝牙初始化*****************/
    UART4_Init();//E24、E25
    
    /************ADC初始化******************/
    ADC_Init();
    //ADC接口
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
    
    /************电机初始化*****************/
    FTM_PwmInit(FTM0, FTM_CH0, 16000, 0);//C1
    FTM_PwmInit(FTM0, FTM_CH1, 16000, 0);//C2
    FTM_PwmInit(FTM0, FTM_CH2, 16000, 0);//C3
    FTM_PwmInit(FTM0, FTM_CH3, 16000, 0);//C4
    
    /************编码器初始化***************/
    FTM_ABInit(FTM1);//A12、A13
    FTM_ABInit(FTM2);//B18、B19
    
    /************舵机初始化******************/
    FTM_PwmInit(FTM3, FTM_CH6, 50,Controlvar.Steer_center);//C10
    
    
    //PIT定时器初始化
    PIT_Init(PIT0,5);
    //PIT_Init(PIT1, 20);

    /* 优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断 */
    NVIC_SetPriority(PIT0_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    //NVIC_SetPriority(PIT1_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0,2));
    
    //使能PIT0_IRQn的中断
    NVIC_EnableIRQ(PIT0_IRQn);			
    //NVIC_EnableIRQ(PIT1_IRQn);			          //使能PIT1_IRQn的中断
}

void data_init(void)
{
    //归一化阈值
    Controlvar.SPDG_max=1500;//水平max
    Controlvar.XDG_max=1500;//斜max
    Controlvar.ZZ_max=1400;//中间max
    
    //舵机
    Controlvar.Steer_center=670;//舵机中值
    Controlvar.Steer_left=750;
    Controlvar.Steer_right=590;
    
    /*********************************速度PID****************************************/
    Controlvar.Error[0]=0.0;    //误差数组1
    Controlvar.Error[1]=0.0;    //误差数组2
    Controlvar.Error[2]=0.0;    //误差数组3
    Controlvar.Error[3]=0.0;    //误差数组4
    Controlvar.Pwm1_Out_Old=0;    //上一个速度输出
    Controlvar.Pwm1_Out_New=0;    //这一个速度输出
    Controlvar.Pwm1_Out=0;        //输出
    Controlvar.Pid1_Out=0;
    
    Controlvar.Pwm2_Out_Old=0;    //上一个速度输出
    Controlvar.Pwm2_Out_New=0;    //这一个速度输出
    Controlvar.Pwm2_Out=0;        //输出
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
    
    /************************************舵机PID**********************************/
    
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
    
    //环岛
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
    
    //直道
    Controlvar.Straight_keep=0;
    Controlvar.Straight_flag=0;
    Controlvar.Straight_delay=20;
    Controlvar.Straight_plus=0;
    
    Controlvar.Curve_less=0;
    
    //十字
    Controlvar.Cross_flag=0;
    
    //出库
    Controlvar.OUT_time=0;
    Controlvar.OUT_time1=80;
    Controlvar.OUT_time2=80;
    Controlvar.OUT_speed=5;
    
    //入库
    Controlvar.Warehousing_flag=0;//入库标志
    Controlvar.Reversing_speed=-5;//倒车速度
    Controlvar.Warehousing_speed=10;//入库速度
    Controlvar.Warehousing_time=0;
    Controlvar.Warehousing_time1=250;
    Controlvar.Warehousing_time2=10;
}
