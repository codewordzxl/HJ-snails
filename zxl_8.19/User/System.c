#include "include.h"

uint16_t g_ValueOfAD[5];//电感值
int Deviation[3];//偏差
int Level;//水平差比和


short enc1=0;//编码器1的值
short enc2=0;//编码器2的值
 
int count=0;

int Angle_Save[20];
/*---------------------------------------------------------------
【函    数】PIT0_Interrupt
【功    能】PIT0的中断服务函数
【参    数】无
【返 回 值】无
【注意事项】注意进入后要清除中断标志位
----------------------------------------------------------------*/
void PIT0_IRQHandler()
{
    PIT_Flag_Clear(PIT0);       //清中断标志位
    /*用户添加所需代码*/  

    /***********************************************延时发车*****************************************************/
    if(Controlvar.Start_count<1000){Controlvar.Start_count++;}//发车时间计数
    if(Controlvar.Start_count>Controlvar.Start_time){Controlvar.Start_flag=1;}//发车时间计数值达到发车时间要求
    
    if(Controlvar.Start_flag)//达到发车时间
    {
      
        /***************************************起跑线检测计时*************************************************/
      
        if(Controlvar.StartLine_count<1200){Controlvar.StartLine_count++;}
        if(Controlvar.StartLine_count>Controlvar.StartLine_time){Controlvar.StartLine_flag=1;}
        
        /********************************************出库******************************************************/
        if(Controlvar.Out_count<1000){Controlvar.Out_count++;}//出库时间计数
        if(Controlvar.Out_count<Controlvar.Out_time1)//出库1、前进
        {
            FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_center);
            Controlvar.speed1_set=Controlvar.Out_speed;
            Controlvar.speed2_set=Controlvar.Out_speed;
        }
        else if(Controlvar.Out_count<Controlvar.Out_time1+Controlvar.Out_time2)//出库2、转弯
        {
            if(Controlvar.Out_Direction==0)//拨码开关（最后一个）
            {
              FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_Left);//按下左出库

            }
            else
            {
              FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_Right);//未按下右出库
            }
            Controlvar.speed1_set=Controlvar.Out_speed;
            Controlvar.speed2_set=Controlvar.Out_speed;
        }
        else//完成出库
        {
            Controlvar.Out_flag=1;
        }
        
        /******************************************正常寻线*************************************************************/
        if(Controlvar.Out_flag&&Controlvar.Stop_flag==0)
        {
            ADC_Deal();
            Track();
            Steer();
            Spd_set();
        }
        
        /**********************************************入库****************************************************************/
        if(Controlvar.Stop_flag)
        {
            if(Controlvar.Stop_count<1000){Controlvar.Stop_count++;}
            if(Controlvar.Stop_count<Controlvar.Stop_time1)
            {
                /************************************转弯****************************************/
                if(Controlvar.Out_Direction==0)//
                {
                  FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_Left);//按下左入库
                  Controlvar.speed1_set=Controlvar.Stop_speed;
                  Controlvar.speed2_set=Controlvar.Stop_speed+Controlvar.Stop_cs;
                }
                else
                {
                  FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_Right);//未按下右入库
                  Controlvar.speed1_set=Controlvar.Stop_speed+Controlvar.Stop_cs;
                  Controlvar.speed2_set=Controlvar.Stop_speed;
                }
            }
            else if(Controlvar.Stop_count<Controlvar.Stop_time1+Controlvar.Stop_time2)
            {
              
                FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_center);
                Controlvar.speed1_set=1;
                Controlvar.speed2_set=1;
            }
            else
            {
                /************************************停车****************************************/
                Controlvar.speed1_set=0;
                Controlvar.speed2_set=0;
            }
            
        }
        count++;
        if(count>=4)
        {
          count=0;
          Spd_out();
        }
    }
    
    /**********************************************图像采集及起跑线检测********************************************************/
    if(Field_Over_Flag)       //完成一场图像采集
    { 
      Get_Use_Image();      //从采集图像数据中取出自己想要使用的大小
      Get_01_Value(1);      //二值化图像数据
      Draw_Road();          //龙邱OLED模块显示动态图像
      
      if(Controlvar.StartLine_flag)
      {
          Start_Deect();//起跑线检测
      }
      Field_Over_Flag= 0;  
    }
}

void ADC_Deal()//ADC读取及偏差计算
{
     int16_t  i,j,k,temp;
     int16_t  ad_valu[5][5],ad_valu1[5],ad_sum[5];

     for(i=0;i<5;i++)
     {
         ad_valu[0][i]=(uint16_t)(ADC_Ave(ADC0_SE5a, ADC_12bit, 5));//左水平
         ad_valu[1][i]=(uint16_t)(ADC_Ave(ADC0_SE4a, ADC_12bit, 5));//右水平
         ad_valu[2][i]=(uint16_t)(ADC_Ave(ADC0_DP3, ADC_12bit, 5));//左斜
         ad_valu[3][i]=(uint16_t)(ADC_Ave(ADC0_SE11, ADC_12bit, 5));//右斜
		 ad_valu[4][i]=(uint16_t)(ADC_Ave(ADC0_DP2, ADC_12bit, 5));//中
     }
/*=========================冒泡排序升序==========================*///舍弃最大值和最小值
     for(i=0;i<5;i++)
     {
        for(j=0;j<4;j++)
        {
           for(k=0;k<4-j;k++)
           {
              if(ad_valu[i][k] > ad_valu[i][k+1])
              {
                 temp = ad_valu[i][k+1];
                 ad_valu[i][k+1] = ad_valu[i][k];
                 ad_valu[i][k] = temp;
              }
           }
        }
     }
/*===========================中值滤波=================================*/
     for(i=0;i<5;i++)    //求中间三项的和
     {
        ad_sum[i] = ad_valu[i][1] + ad_valu[i][2] + ad_valu[i][3];
        ad_valu1[i] = ad_sum[i] / 3;
        g_ValueOfAD[i] = (int16)(ad_valu1[i]/10*10);//将数值中个位数除掉
     }
     
/***************************偏差计算************************************/
     Deviation[0]=100*(g_ValueOfAD[1]-g_ValueOfAD[0])/(g_ValueOfAD[1]+g_ValueOfAD[0]+1);//水平
     Deviation[1]=100*(g_ValueOfAD[3]-g_ValueOfAD[2])/(g_ValueOfAD[3]+g_ValueOfAD[2]+1);//八字
     Deviation[2]=100*(g_ValueOfAD[3]-g_ValueOfAD[2])/(g_ValueOfAD[0]+g_ValueOfAD[1]+1);
     
     Level=100*(g_ValueOfAD[1]-g_ValueOfAD[0])/(g_ValueOfAD[1]+g_ValueOfAD[0]+g_ValueOfAD[4]+1);
     
}

void Track()//赛道识别
{
/*********************************************冲出赛道判断************************************************************/
    if(Controlvar.Leave_count<1000&&(g_ValueOfAD[0]+g_ValueOfAD[1]+g_ValueOfAD[2]+g_ValueOfAD[3]+g_ValueOfAD[4]==0))
    {
        Controlvar.Leave_count++;
    }
    else
    {
        Controlvar.Leave_count=0;
    }
    if(Controlvar.Leave_count>Controlvar.Leave_time){Controlvar.Leave_flag=1;}
  
  
/***********************************************直道、弯道判断********************************************************/
    if(g_ValueOfAD[4]>1000)//直道判断-----直道上存在十字、环岛、坡道
    {
        Controlvar.Straight_keep++;//持续时间
    }
    else
    {
        Controlvar.Straight_keep=0;
    }
    
    if(Controlvar.Straight_keep>=1000)Controlvar.Straight_keep=1000;//防止溢出
    
    if(Controlvar.Straight_keep>Controlvar.Straight_delay)//时间5*Controlvar.Straight_delay   ms
    {
        Controlvar.Straight_flag=1;//直道
    }
    else
    {
        Controlvar.Straight_flag=0;//弯道
    }
    
/*******************************************环岛识别***************************************************************/
    
    //使用Controlvar.Straight_flag先判断直道
    //可能存在直道不稳，导致未判直道，影响直道上其他元素的判断
    if(Controlvar.Straight_flag)//直道存在十字、环岛、坡道
    {
    /**************************************坡道判断***********************************************/      
        if(g_ValueOfAD[4]>3000)//坡道
        {
          if(g_ValueOfAD[0]+g_ValueOfAD[1]<3500)//环岛正中，中间电感值可能达到3000
          {
            if(Controlvar.Ramp_flag==0)//此前为非坡道
            {
              Controlvar.Ramp_flag=1;//坡道标志
            }
            else
            {
              Controlvar.Ramp_flag=0;
            }
          }
        }
    /*************************************疑似环岛判断*********************************************/
        if(g_ValueOfAD[0]>g_ValueOfAD[1]&&g_ValueOfAD[3]>g_ValueOfAD[2])
        {
          Controlvar.Roundright_keep++;//疑似右环岛保持时间
          Controlvar.Roundleft_keep=0;
        }
        else
        {
          Controlvar.Roundright_keep=0;
        }
        
        if(g_ValueOfAD[1]>g_ValueOfAD[0]&&g_ValueOfAD[2]>g_ValueOfAD[3])
        {
          Controlvar.Roundleft_keep++;//疑似左环岛保持时间
          Controlvar.Roundright_keep=0;
        }
        else
        {
          Controlvar.Roundleft_keep=0;
        }
        
        if(Controlvar.Roundright_keep>=1000)Controlvar.Roundright_keep=1000;//防止溢出
        if(Controlvar.Roundleft_keep>=1000)Controlvar.Roundleft_keep=1000;//防止溢出
        
        
        if(Controlvar.Roundright_keep>Controlvar.Round_delay)//疑似右环岛
        {
          Controlvar.Round_right=1;
          Controlvar.Round_left=0;
        }
        
        if(Controlvar.Roundleft_keep>Controlvar.Round_delay)//疑似左环岛
        {
          Controlvar.Round_left=1;
          Controlvar.Round_right=0;
        }
        /*************************************环岛确认*****************************************************/
        if(Controlvar.Roundleft_flag==0&&Controlvar.Roundright_flag==0&&(Controlvar.Round_left||Controlvar.Round_right))//此前未确认环岛且存在疑似环岛
        {
          if(g_ValueOfAD[4]>2000&&g_ValueOfAD[0]+g_ValueOfAD[1]>3500)//环岛
          {
            if(Controlvar.Round_left)//疑似转为确立环岛存在标志
            {
              Controlvar.Roundleft_flag=1;//左环岛标志
              GPIO_Pin_Write(PTE29,0);
              Controlvar.Round_left=0;
            }
            
            if(Controlvar.Round_right)
            {
              Controlvar.Roundright_flag=1;//右环岛标志
              GPIO_Pin_Write(PTD15,0);
              Controlvar.Round_right=0;
            }
          }
        }
        /******************************************进入环岛****************************************************************/
        if((Controlvar.Roundright_flag||Controlvar.Roundleft_flag)&&g_ValueOfAD[4]<2400)//进入环岛
        {
          Controlvar.RoundEnter=1;
        }
        
        //进入环岛
        if(Controlvar.RoundEnter&&Controlvar.RoundEnter_left==0&&Controlvar.RoundEnter_right==0)
        {
          if(Controlvar.Roundleft_flag)//进入左环岛
          {
            if(g_ValueOfAD[1]>g_ValueOfAD[0]&&g_ValueOfAD[3]>g_ValueOfAD[2])
            {
              Controlvar.RoundEnter_left=1;
            }
          }
          if(Controlvar.Roundright_flag)//进入右环岛
          {
            if(g_ValueOfAD[0]>g_ValueOfAD[1]&&g_ValueOfAD[2]>g_ValueOfAD[3])
            {
              Controlvar.RoundEnter_right=1;
            }
          }
        }
        
    }
    else//弯道
    {
        Controlvar.Round_left=0;
        Controlvar.Round_right=0;
        Controlvar.Roundleft_flag=0;//左环岛标志
        Controlvar.Roundright_flag=0;//右环岛标志
    }
   
    /******************************************进环完成判断**************************************************/
    if(g_ValueOfAD[4]<1500&&(Controlvar.RoundEnter_left||Controlvar.RoundEnter_right))//进环完成或识别错误
    {
        if(Controlvar.RoundEnter_left)
        {
            if(Controlvar.speed==20)
            {
                if(g_ValueOfAD[3]<300||g_ValueOfAD[0]+g_ValueOfAD[1]<2000)//默认速度左进环完成
              {
                Controlvar.Round_left=0;
                Controlvar.Round_right=0;
                Controlvar.Roundleft_keep=0;
                Controlvar.Roundright_keep=0;
                Controlvar.Roundright_flag=0;
                Controlvar.Roundleft_flag=0;
                Controlvar.RoundEnter=0;
                Controlvar.RoundEnter_left=0;
                Controlvar.RoundEnter_right=0;
                
                Controlvar.Lostline_left=0;//进环过程可能出现反向丢线
                Controlvar.Lostline_right=0;
                
                GPIO_Pin_Write(PTE29,1);
                GPIO_Pin_Write(PTD15,1);
                }
            }
            else
            {
                if(g_ValueOfAD[0]+g_ValueOfAD[1]<2200)
              {
                Controlvar.Round_left=0;
                Controlvar.Round_right=0;
                Controlvar.Roundleft_keep=0;
                Controlvar.Roundright_keep=0;
                Controlvar.Roundright_flag=0;
                Controlvar.Roundleft_flag=0;
                Controlvar.RoundEnter=0;
                Controlvar.RoundEnter_left=0;
                Controlvar.RoundEnter_right=0;
                
                Controlvar.Lostline_left=0;//进环过程可能出现反向丢线
                Controlvar.Lostline_right=0;
                
                GPIO_Pin_Write(PTE29,1);
                GPIO_Pin_Write(PTD15,1);
                }
            }
            
        }
        if(Controlvar.RoundEnter_right)
        {
            if(Controlvar.speed==20)
            {
                if(g_ValueOfAD[3]<300||g_ValueOfAD[0]+g_ValueOfAD[1]<2000)//默认速度右进环完成
              {
                Controlvar.Round_left=0;
                Controlvar.Round_right=0;
                Controlvar.Roundleft_keep=0;
                Controlvar.Roundright_keep=0;
                Controlvar.Roundright_flag=0;
                Controlvar.Roundleft_flag=0;
                Controlvar.RoundEnter=0;
                Controlvar.RoundEnter_left=0;
                Controlvar.RoundEnter_right=0;
                
                Controlvar.Lostline_left=0;//进环过程可能出现反向丢线
                Controlvar.Lostline_right=0;
                
                GPIO_Pin_Write(PTE29,1);
                GPIO_Pin_Write(PTD15,1);
                }
            }
            else
            {
                if(g_ValueOfAD[0]+g_ValueOfAD[1]<2200)
              {
                Controlvar.Round_left=0;
                Controlvar.Round_right=0;
                Controlvar.Roundleft_keep=0;
                Controlvar.Roundright_keep=0;
                Controlvar.Roundright_flag=0;
                Controlvar.Roundleft_flag=0;
                Controlvar.RoundEnter=0;
                Controlvar.RoundEnter_left=0;
                Controlvar.RoundEnter_right=0;
                
                Controlvar.Lostline_left=0;//进环过程可能出现反向丢线
                Controlvar.Lostline_right=0;
                
                GPIO_Pin_Write(PTE29,1);
                GPIO_Pin_Write(PTD15,1);
                }
            }
        }
    }
/*环岛识别***************************************************************************************************************************************************************************/
}

void Steer()
{
/******************************************************进入环岛******************************************************/
    if(Controlvar.RoundEnter_left||Controlvar.RoundEnter_right)
    {
          Controlvar.Duoji_P=Controlvar.Round_P;
          Controlvar.Duoji_RD=Controlvar.Round_D;
          Controlvar.Deviation=Deviation[1]*Controlvar.Round_coeff;//斜电感引导入环
          if(Controlvar.RoundEnter_left)
          {
              if(Controlvar.Deviation<10)
              {
                  Controlvar.Deviation=Controlvar.Round_FD;
              }
              if(Controlvar.Deviation<0)
              {
                  Controlvar.Deviation=-(Controlvar.Deviation-Controlvar.Round_QD);
              }
          }
          if(Controlvar.RoundEnter_right)
          {
              if(Controlvar.Deviation>-10)
              {
                  Controlvar.Deviation=-Controlvar.Round_FD;
              }
              if(Controlvar.Deviation>0)
              {
                  Controlvar.Deviation=-(Controlvar.Deviation+Controlvar.Round_QD);
              }
          }
          if(Controlvar.Deviation>70)Controlvar.Deviation=70;
          if(Controlvar.Deviation<-70)Controlvar.Deviation=-70;
          Controlvar.Steer_variety=Steer_PD(Controlvar.Deviation);//用PD计算转角
/*****进入环岛****************************************************************************************************************/
    }
    else
    {
/*******************************************************非环岛******************************************************************/
          if(Controlvar.Straight_flag)//直道
          {
              Controlvar.Duoji_P=Controlvar.Straight_P;
              Controlvar.Duoji_RD=Controlvar.Straight_D;
              Controlvar.Deviation=Level;
              
              if(Controlvar.Deviation>70)Controlvar.Deviation=70;
              if(Controlvar.Deviation<-70)Controlvar.Deviation=-70;
              Controlvar.Steer_variety=Steer_PD(Controlvar.Deviation);
              
          }
          else
          {
              Controlvar.Duoji_P=Controlvar.Curve_P;
              Controlvar.Duoji_RD=Controlvar.Curve_D;
              Controlvar.Deviation=(int)(Level*1.5);
              
              if(Controlvar.Deviation>70)Controlvar.Deviation=70;
              if(Controlvar.Deviation<-70)Controlvar.Deviation=-70;
              Controlvar.Steer_variety=Steer_PD(Controlvar.Deviation);
          }
          
          /*****************************************丢线****************************************/
          if(g_ValueOfAD[4]<Controlvar.Lostline_Threshold)
          {
              if(Controlvar.Lostline_right==0&&Controlvar.Lostline_left==0)//不存在丢线时进入
              {
                  int i,zuo=0,you=0;
                  for(i=0;i<19;i++)
                  {
                    if(Angle_Save[i]>0)zuo++;
                    if(Angle_Save[i]<0)you++;
                  }
                  if(zuo>you)
                  {
                    Controlvar.Lostline_left=1;//左转丢线
                  }
                  if(zuo<you)
                  {
                    Controlvar.Lostline_right=1;//右转丢线
                  }
                  GPIO_Pin_Write(PTC0,0);//丢线指示灯（亮）
              }
          }
          if(g_ValueOfAD[4]>Controlvar.Lostline_Restore)
          {
              GPIO_Pin_Write(PTC0,1);//丢线指示灯（灭）
              Controlvar.Lostline_right=0;
              Controlvar.Lostline_left=0;
          }
          
          if(Controlvar.Lostline_right)
          {
              Controlvar.Steer_variety=-80;
          }
          if(Controlvar.Lostline_left)
          {
              Controlvar.Steer_variety=80;
          }
    }
/**********非环岛*************************************************************************************************/
    
    
    
    /******************************************转角记录************************************************************/
    int i;
    for(i=0;i<19;i++)
    {
        Angle_Save[i]=Angle_Save[i+1];
    }
    Angle_Save[19]=Controlvar.Steer_variety;
    
    /*******************************************舵机输出***********************************************************/
    Controlvar.Steer_out=Controlvar.Steer_center+Controlvar.Steer_variety;//Steer_out舵机PWM输出值
    if(Controlvar.Steer_out>Controlvar.Steer_Left)Controlvar.Steer_out=Controlvar.Steer_Left;//左转限幅
    if(Controlvar.Steer_out<Controlvar.Steer_Right)Controlvar.Steer_out=Controlvar.Steer_Right;//右转限幅
    FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_out);//舵机PWM输出
}

void Spd_set()
{
/************************************************减速入环***************************************************/
    if(Controlvar.Roundright_flag||Controlvar.Roundleft_flag)
    {
        if(Controlvar.speed<25)
        {
            Controlvar.speed1_set=Controlvar.speed;
            Controlvar.speed2_set=Controlvar.speed;
        }
        else
        {
            Controlvar.speed1_set=Controlvar.Round_speed;
            Controlvar.speed2_set=Controlvar.Round_speed;
        }
        
        if(Controlvar.RoundEnter_right)
        {
            Controlvar.speed1_set+=Controlvar.Round_Dif;
        }
        else
        {
            Controlvar.speed2_set+=Controlvar.Round_Dif;
        }
    }
    else
    {
/****************************************************转弯差速*******************************************************/
        if(Controlvar.Lostline_right==0&&Controlvar.Lostline_left==0)
        {
            if(Controlvar.Steer_variety>20)
            {
              Controlvar.speed1_set=Controlvar.speed-(int)(Controlvar.Steer_variety*Controlvar.Cruve_Dif);
              Controlvar.speed2_set=Controlvar.speed;
            }
            else if(Controlvar.Steer_variety<-20)
            {
              Controlvar.speed1_set=Controlvar.speed;
              Controlvar.speed2_set=Controlvar.speed+(int)(Controlvar.Steer_variety*Controlvar.Cruve_Dif);
            }
            else 
            {
              Controlvar.speed1_set=Controlvar.speed+Controlvar.Straight_plus;
              Controlvar.speed2_set=Controlvar.speed+Controlvar.Straight_plus;
            }
        }
        else
        {
        /*********************************丢线差速**************************************/
            if(Controlvar.Lostline_right)
            {
              Controlvar.speed1_set=Controlvar.speed;
              Controlvar.speed2_set=Controlvar.speed-Controlvar.Lostline_Dif;
            }
            if(Controlvar.Lostline_left)
            {
              Controlvar.speed1_set=Controlvar.speed-Controlvar.Lostline_Dif;
              Controlvar.speed2_set=Controlvar.speed;
            }
        }
    }
    
    /*******************************************冲出赛道停车**********************************************/
    if(Controlvar.Leave_flag)//冲出赛道
    {
        Controlvar.speed1_set=0;
        Controlvar.speed2_set=0;
    }
    
}

void Spd_out()
{
    enc1 = FTM_ABGet(FTM1)/10;
    enc2 = -FTM_ABGet(FTM2)/10;
    
    Controlvar.speed1_get=enc1;//speed1_get左轮速度
    PID_left(Controlvar.speed1_set);
    
    Controlvar.speed2_get=enc2;
    PID_right(Controlvar.speed2_set);
    
    if(Controlvar.Pwm1_Out>=0)//左电机
   {
     FTM_PwmDuty(FTM0,FTM_CH0,0);   //电机正转   
     FTM_PwmDuty(FTM0,FTM_CH1,(u16)Controlvar.Pwm1_Out);  
   }
   else
   {
      FTM_PwmDuty(FTM0,FTM_CH1,0);   //电机反转  
      FTM_PwmDuty(FTM0,FTM_CH0,-(u16)Controlvar.Pwm1_Out);  
   }
   
   if(Controlvar.Pwm2_Out>=0)//右电机
   {
     FTM_PwmDuty(FTM0,FTM_CH2,0);   //电机正转   
     FTM_PwmDuty(FTM0,FTM_CH3,(u16)Controlvar.Pwm2_Out);  
   }
   else
   {
      FTM_PwmDuty(FTM0,FTM_CH3,0);   //电机反转  
      FTM_PwmDuty(FTM0,FTM_CH2,-(u16)Controlvar.Pwm2_Out);  
   }
   
   if(Controlvar.speed1_set==0&&Controlvar.speed1_get<=2&&Controlvar.speed2_set==0&&Controlvar.speed2_get<=2)
   {
      FTM_PwmDuty(FTM0,FTM_CH0,0);
      FTM_PwmDuty(FTM0,FTM_CH1,0);
      FTM_PwmDuty(FTM0,FTM_CH2,0);
      FTM_PwmDuty(FTM0,FTM_CH3,0);
   }
}