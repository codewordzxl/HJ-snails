#include "include.h"

Control Controlvar;

void System_Init()
{
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
    FTM_PwmInit(FTM0, FTM_CH0, 13000, 0);//C1
    FTM_PwmInit(FTM0, FTM_CH1, 13000, 0);//C2
    FTM_PwmInit(FTM0, FTM_CH2, 13000, 0);//C3
    FTM_PwmInit(FTM0, FTM_CH3, 13000, 0);//C4
    
    /************编码器初始化***************/
    FTM_ABInit(FTM1);//A12、A13
    FTM_ABInit(FTM2);//B18、B19
    
    /************舵机初始化******************/
    FTM_PwmInit(FTM3, FTM_CH6, 50,Controlvar.Steer_center);//C10
    
    
    //数据初始化
    Data_Init();
    //摄像头初始化
    LQMT9V034_Init();
    //PIT定时器初始化
    PIT_Init(PIT0,5);

    /* 优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断 */
    NVIC_SetPriority(PIT0_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    			
}

void Data_Init()
{
    Controlvar.Steer_center=670;//舵机中值
    Controlvar.Steer_Left=750;//舵机左限幅
    Controlvar.Steer_Right=590;//舵机右限幅
    
    //查看数据模式
    Controlvar.See_Data=0;//0正常行驶模式    1查看数据模式（由拨码开关5控制  未按下0  按下1）
    
    //延时发车
    Controlvar.Start_time=100;
    Controlvar.Start_count=0;
    Controlvar.Start_flag=0;
    
    //出库
    Controlvar.Out_time1=30;
    Controlvar.Out_time2=80;
    Controlvar.Out_count=0;
    Controlvar.Out_flag=0;
    Controlvar.Out_speed=20;//出库速度
    Controlvar.Out_Direction=1;//出入库方向   0左   1右   （默认右出入库）
    
    //起跑线检测
    Controlvar.StartLine_time=1000;//发车后 StartLine_time * 5ms 开始检测起跑线
    Controlvar.StartLine_count=0;
    Controlvar.StartLine_flag=0;
    
    //入库（停车）
    Controlvar.Stop_flag=0;
    Controlvar.Stop_time1=120;
    Controlvar.Stop_time2=10;
    Controlvar.Stop_count=0;
    Controlvar.Stop_speed=9;//入库速度
    Controlvar.Stop_cs=10;
    
    //速度
    Controlvar.speed=25;//默认速度(保底速度20)
    
    //冲出赛道停车
    Controlvar.Leave_flag=0;
    Controlvar.Leave_time=400;
    Controlvar.Leave_count=0;
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
    Controlvar.Speed_PU=750;
    Controlvar.Speed_IS=18;
    Controlvar.Speed_IX=40;
    Controlvar.Speed_IU=75;
    
    Controlvar.speed1_set=0;
    Controlvar.speed2_set=0;
    /*****************************************************************************/
    
    /************************************舵机PID**********************************/
    
    Controlvar.Duoji_P=1.f;
    Controlvar.Duoji_RD=9.f;
    Controlvar.Erro=0.f;
    
    /*****************************************************************************/
    
    //直道
    Controlvar.Straight_flag=0;
    Controlvar.Straight_keep=0;
    Controlvar.Straight_delay=20;
    Controlvar.Straight_plus=0;//加速
    
    //弯道减速
    Controlvar.Cruve_less=0;
    Controlvar.Cruve_Dif=0.05;
    //坡道
    Controlvar.Ramp_flag=0;
    
    //环岛
    Controlvar.Round_left=0;
    Controlvar.Round_right=0;
    Controlvar.Round_delay=8;
    Controlvar.Roundleft_keep=0;
    Controlvar.Roundright_keep=0;
    Controlvar.Roundleft_flag=0;//左环岛标志
    Controlvar.Roundright_flag=0;//右环岛标志
    Controlvar.RoundEnter=0;
    Controlvar.RoundEnter_left=0;
    Controlvar.RoundEnter_right=0;
    Controlvar.Round_speed=20;//进出环岛速度
    Controlvar.Round_Dif=5;
    Controlvar.Round_coeff=4;
    Controlvar.Round_QD=20;//回正值
    Controlvar.Round_FD=10;
    
    //丢线
    Controlvar.Lostline_left=0;//左转丢线标志
    Controlvar.Lostline_right=0;//右转丢线标志
    //丢线参数
    Controlvar.Lostline_Threshold=400;//丢线阈值
    Controlvar.Lostline_Restore=600;//丢线恢复阈值
    Controlvar.Lostline_Dif=0;
    
    //舵机PID参数
    Controlvar.Straight_P=0.8f;//直道P
    Controlvar.Straight_D=9.f;//直道D
    Controlvar.Curve_P=1.5f;//弯道P
    Controlvar.Curve_D=18.f;//弯道D
    Controlvar.Round_P=1.5f;//环岛P
    Controlvar.Round_D=10.f;//环岛D
}

void Key_set()
{
    FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_center);
    
/***********************************************************************速度档********************************************************************************/
    /********************速度1**31+0**************/
    if(GPIO_Pin_Read(PTB0)==0&&GPIO_Pin_Read(PTB1)==1&&GPIO_Pin_Read(PTB2)==1&&GPIO_Pin_Read(PTB3)==1)
    {
        Controlvar.speed=31;
        
        //舵机PID参数
        Controlvar.Straight_P=0.8f;//直道P
        Controlvar.Straight_D=14.f;//直道D
        Controlvar.Curve_P=1.1f;//弯道P
        Controlvar.Curve_D=15.f;//弯道D
        Controlvar.Round_P=2.f;//环岛P
        Controlvar.Round_D=10.f;//环岛D
        
        Controlvar.Round_QD=40;//回正值
        Controlvar.Round_FD=10;
        
        Controlvar.Round_speed=25;
        //弯道
        Controlvar.Cruve_less=0;
        Controlvar.Cruve_Dif=0.05;
        
        Controlvar.Straight_plus=0;
        //出库
        Controlvar.Out_time1=30;
        Controlvar.Out_time2=80;
        
        //入库
        Controlvar.Stop_time1=120;//打角时间
        Controlvar.Stop_time2=10;//直行时间
        Controlvar.Stop_speed=10;//入库速度
        Controlvar.Stop_cs=10;
        
        Controlvar.Round_coeff=6;//环岛偏差系数
        
        Controlvar.Lostline_Threshold=500;//丢线阈值
        Controlvar.Lostline_Restore=600;//丢线恢复阈值
        Controlvar.Lostline_Dif=7;
        
        //减速PI
        Controlvar.Speed_PU=650;
        Controlvar.Speed_IU=65;
    }
    
    /******************速度2****31+2*************/
    if(GPIO_Pin_Read(PTB0)==1&&GPIO_Pin_Read(PTB1)==0&&GPIO_Pin_Read(PTB2)==1&&GPIO_Pin_Read(PTB3)==1)
    {
        Controlvar.speed=31;
        
        //舵机PID参数
        Controlvar.Straight_P=0.8f;//直道P
        Controlvar.Straight_D=14.f;//直道D
        Controlvar.Curve_P=1.1f;//弯道P
        Controlvar.Curve_D=15.f;//弯道D
        Controlvar.Round_P=2.f;//环岛P
        Controlvar.Round_D=10.f;//环岛D
        
        Controlvar.Round_QD=40;//回正值
        Controlvar.Round_FD=10;
        
        Controlvar.Round_speed=25;
        //弯道
        Controlvar.Cruve_less=0;
        Controlvar.Cruve_Dif=0.05;
        
        Controlvar.Straight_plus=2;
        //出库
        Controlvar.Out_time1=30;
        Controlvar.Out_time2=80;
        
        //入库
        Controlvar.Stop_time1=120;//打角时间
        Controlvar.Stop_time2=10;//直行时间
        Controlvar.Stop_speed=10;//入库速度
        Controlvar.Stop_cs=10;
        
        Controlvar.Round_coeff=6;//环岛偏差系数
        
        Controlvar.Lostline_Threshold=500;//丢线阈值
        Controlvar.Lostline_Restore=600;//丢线恢复阈值
        Controlvar.Lostline_Dif=7;
        
        //减速PI
        Controlvar.Speed_PU=650;
        Controlvar.Speed_IU=65;
    }
    
    /****************速度3*****33+0****************/
    if(GPIO_Pin_Read(PTB0)==0&&GPIO_Pin_Read(PTB1)==0&&GPIO_Pin_Read(PTB2)==1&&GPIO_Pin_Read(PTB3)==1)
    {
        Controlvar.speed=33;
        
        //舵机PID参数
        Controlvar.Straight_P=0.8f;//直道P
        Controlvar.Straight_D=14.f;//直道D
        Controlvar.Curve_P=1.2f;//弯道P
        Controlvar.Curve_D=15.f;//弯道D
        Controlvar.Round_P=3.f;//环岛P
        Controlvar.Round_D=10.f;//环岛D
        
        Controlvar.Round_QD=40;//回正值
        Controlvar.Round_FD=10;
        
        Controlvar.Round_speed=28;
        //弯道
        Controlvar.Cruve_less=0;
        Controlvar.Cruve_Dif=0.05;
        
        Controlvar.Straight_plus=0;
        //出库
        Controlvar.Out_time1=30;
        Controlvar.Out_time2=80;
        
        //入库
        Controlvar.Stop_time1=120;//打角时间
        Controlvar.Stop_time2=10;//直行时间
        Controlvar.Stop_speed=10;//入库速度
        Controlvar.Stop_cs=10;
        
        Controlvar.Round_coeff=6;//环岛偏差系数
        
        Controlvar.Lostline_Threshold=500;//丢线阈值
        Controlvar.Lostline_Restore=600;//丢线恢复阈值
        Controlvar.Lostline_Dif=7;
        
        //减速PI
        Controlvar.Speed_PU=650;
        Controlvar.Speed_IU=65;
    }
    
    /******************速度4***33+2********************/
    if(GPIO_Pin_Read(PTB0)==1&&GPIO_Pin_Read(PTB1)==1&&GPIO_Pin_Read(PTB2)==0&&GPIO_Pin_Read(PTB3)==1)
    {
        Controlvar.speed=33;
        
        //舵机PID参数
        Controlvar.Straight_P=0.8f;//直道P
        Controlvar.Straight_D=14.f;//直道D
        Controlvar.Curve_P=1.2f;//弯道P
        Controlvar.Curve_D=15.f;//弯道D
        Controlvar.Round_P=3.f;//环岛P
        Controlvar.Round_D=10.f;//环岛D
        
        Controlvar.Round_QD=40;//回正值
        Controlvar.Round_FD=10;
        
        Controlvar.Round_speed=28;
        //弯道
        Controlvar.Cruve_less=0;
        Controlvar.Cruve_Dif=0.05;
        
        Controlvar.Straight_plus=2;
        //出库
        Controlvar.Out_time1=30;
        Controlvar.Out_time2=80;
        
        //入库
        Controlvar.Stop_time1=120;//打角时间
        Controlvar.Stop_time2=10;//直行时间
        Controlvar.Stop_speed=10;//入库速度
        Controlvar.Stop_cs=10;
        
        Controlvar.Round_coeff=6;//环岛偏差系数
        
        Controlvar.Lostline_Threshold=500;//丢线阈值
        Controlvar.Lostline_Restore=600;//丢线恢复阈值
        Controlvar.Lostline_Dif=8;
        
        //减速PI
        Controlvar.Speed_PU=650;
        Controlvar.Speed_IU=65;
    }
    
    /**********************速度5*******************/
    if(GPIO_Pin_Read(PTB0)==0&&GPIO_Pin_Read(PTB1)==1&&GPIO_Pin_Read(PTB2)==0&&GPIO_Pin_Read(PTB3)==1)
    {
        Controlvar.speed=35;
        
        //舵机PID参数
        Controlvar.Straight_P=0.8f;//直道P
        Controlvar.Straight_D=14.f;//直道D
        Controlvar.Curve_P=1.3f;//弯道P
        Controlvar.Curve_D=14.f;//弯道D
        Controlvar.Round_P=4.f;//环岛P
        Controlvar.Round_D=10.f;//环岛D
        
        Controlvar.Round_QD=40;//回正值
        Controlvar.Round_FD=10;
        
        Controlvar.Round_speed=28;
        //弯道
        Controlvar.Cruve_less=0;
        Controlvar.Cruve_Dif=0.06f;
        
        Controlvar.Straight_plus=0;
        //出库
        Controlvar.Out_time1=30;
        Controlvar.Out_time2=80;
        
        //入库
        Controlvar.Stop_time1=140;//打角时间
        Controlvar.Stop_time2=10;//直行时间
        Controlvar.Stop_speed=9;//入库速度
        Controlvar.Stop_cs=10;
        
        Controlvar.Round_coeff=6;//环岛偏差系数
        
        Controlvar.Lostline_Threshold=500;//丢线阈值
        Controlvar.Lostline_Restore=600;//丢线恢复阈值
        
        Controlvar.Lostline_Dif=8;
        
        //减速PI
        Controlvar.Speed_PU=750;
        Controlvar.Speed_IU=75;
    }
    
    /******************速度6***********************/
    if(GPIO_Pin_Read(PTB0)==1&&GPIO_Pin_Read(PTB1)==0&&GPIO_Pin_Read(PTB2)==0&&GPIO_Pin_Read(PTB3)==1)
    {
        Controlvar.speed=37;
        
        //舵机PID参数
        Controlvar.Straight_P=0.8f;//直道P
        Controlvar.Straight_D=18.f;//直道D
        Controlvar.Curve_P=1.3f;//弯道P
        Controlvar.Curve_D=18.f;//弯道D
        Controlvar.Round_P=5.f;//环岛P
        Controlvar.Round_D=10.f;//环岛D
        
        Controlvar.Round_QD=40;//回正值
        Controlvar.Round_FD=10;
        
        Controlvar.Round_speed=30;
        //弯道
        Controlvar.Cruve_less=0;
        Controlvar.Cruve_Dif=0.08f;
        
        Controlvar.Straight_plus=0;
        //出库
        Controlvar.Out_time1=30;
        Controlvar.Out_time2=80;
        
        //入库
        Controlvar.Stop_time1=140;//打角时间
        Controlvar.Stop_time2=10;//直行时间
        Controlvar.Stop_speed=9;//入库速度
        Controlvar.Stop_cs=10;
        
        Controlvar.Round_coeff=6;//环岛偏差系数
        
        Controlvar.Lostline_Threshold=500;//丢线阈值
        Controlvar.Lostline_Restore=600;//丢线恢复阈值
        
        Controlvar.Lostline_Dif=8;
        
        //减速PI
        Controlvar.Speed_PU=750;
        Controlvar.Speed_IU=75;
    }
    
/***速度档***************************************************************************************************************************************************/
    

    /*******************按键5**********************/
    //OLED显示ADC值及偏差
    if(GPIO_Pin_Read(PTB4)==0)
    {
        Controlvar.See_Data=1;//查看数据模式
    }
    else
    {
        NVIC_EnableIRQ(PORTD_IRQn);			     //使能PORTD_IRQn的中断 
        
        NVIC_EnableIRQ(PIT0_IRQn);                              //使能PIT0_IRQn的中断
    }
    /*******************按键6**********************/
    //控制出入库方向
    if(GPIO_Pin_Read(PTB5)==0)
    {
        Controlvar.Out_Direction=0;//按下 左出入库
    }
    else
    {
        Controlvar.Out_Direction=1;//未按下 右出入库
    }
}